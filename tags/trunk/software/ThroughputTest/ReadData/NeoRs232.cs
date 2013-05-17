using System;
using System.IO.Ports;
using System.Threading;
using System.Collections;

namespace phonePC
{
	
	//!States of the receiving state machine
	enum MSG_T
	{
	  MSG_NO_INPUT,			/*!<No data has come in.*/
	  MSG_DECODING,			/*!<State machine is decoding data.*/
	  MSG_ACK_RECEIVED,             //!Ack received
	  MSG_RX_OVERFLOW,              //!RX buffer overflowed.
	  MSG_RX_COMPLETE	        /*!<Complete packet received.*/
	};
	
	public enum FRAME_TYPE
	{
	  INFO_FRAME=0,			/*!<Info frame (contains data)*/
	  ACK_FRAME=1			/*!<Acknowledge frame*/
	};
	
	enum FLAGS
	{
	  LEADING_FLAG=0xAA,		/*!<Start flag*/
	  DLE_FLAG=0x10,	        /*!<Bytestuffing flag*/
	  TRAILING_FLAG=0xFF,		/*!<End flag*/
	  ACK0_FLAG=0x30		/*!<Acknowledge flag*/
	};
	
	enum STATE
	{
		WAITING,
		DECODING
	};
	
	public class Frame
	{
		private FRAME_TYPE _frType;
		private byte[] _data;
		
		public Frame(FRAME_TYPE fr)
		{
			_frType=fr;
		}
		
		public Frame(FRAME_TYPE fr, byte[] data):this(fr)
		{
			_data=data;
		}
		
		public FRAME_TYPE Type
		{
			get
			{
				return _frType;
			}
		}
	}

	public class NeoRs232
	{
		private byte[] _frameData;
		private int _pData;
		private const int FRAMESIZE=100;
		private bool _dle_received=false;
		private SerialPortTest _mySerial;
		private Crc16 _crc;
		private STATE _state;
		private Thread _oReceiveThread;
		private ArrayList _aReceivedData;
		
		public NeoRs232 (SerialPortTest mySerial)
		{
			_frameData=new byte[FRAMESIZE];
			_pData=0;
			_mySerial=mySerial;
			_crc=new Crc16();
			_state=STATE.WAITING;
			_aReceivedData=new ArrayList();
			_oReceiveThread = new Thread(new ThreadStart(receiveRun));
			_oReceiveThread.Start();
			while(!_oReceiveThread.IsAlive);
		}    
		
		void receiveRun()
		{
			byte[] aMsg;
			while(true)
			{
	  			switch(FramerReceive()) 
	  			{
	  				case MSG_T.MSG_RX_COMPLETE:
	    				if(_crc.stripCRC(_frameData,_pData, out aMsg))
	    				{
	      					//Frame received correctly
							_aReceivedData.Add(new Frame(FRAME_TYPE.INFO_FRAME, aMsg));
					      	writeString(null,FRAME_TYPE.ACK_FRAME);
							//      //Do something with pFramer
							//      writeFrame(
							//      pFramer->pBegin,pFramer->pData-pFramer->pBegin);
	    				}
	    				break;
					case MSG_T.MSG_ACK_RECEIVED:
						_aReceivedData.Add(new Frame(FRAME_TYPE.ACK_FRAME));
						break;
					case MSG_T.MSG_DECODING:
						//I'm still busy
						break;
					case MSG_T.MSG_NO_INPUT:
						//They're probably sleeping on the other side of the line.
						break;
					case MSG_T.MSG_RX_OVERFLOW:
						//Hey! Take it easy, man!  I can't follow.
						break;
				}
			}
		}//receiveRun
		
		/*! Function that sends a frame.  First a CRC16 is appended.  Then the
		* frame is sent.  After that, a timeout timer is initialized.  The
		* acknowledge must come in before the timeout expires.
		*\param buffer Buffer containing the data to send.
		*\param length Number of bytes that must be sent.
		*\param pFr    Pointer to framer, to check for incoming ack.
		*\return       true when ACK received, else false.
		*/
		public bool txFrameWithAck(byte[] buffer){
			_crc.appendCRC(ref buffer, buffer.Length);
			writeString(buffer, FRAME_TYPE.INFO_FRAME);
            DateTime startTime = DateTime.Now;
            TimeSpan duration;
			do
			{
				for(int i=0;i<_aReceivedData.Count;i++)
				{
					if(((Frame)_aReceivedData[i]).Type==FRAME_TYPE.ACK_FRAME)
					{
						_aReceivedData.RemoveAt(i);
						return true;
					}
				}
				duration = DateTime.Now - startTime;
				Thread.Sleep(1);
			}
			while (duration.TotalMilliseconds < 1000);
			return false;
		}//txFrameWithAck
		
		/*! Function that reads a received character from the Stream using a state machine.
		 *  The databytes are read from the circular buffer, a check if performed 
		 *  if it is a flagbyte.  If it is, then the correct action is undertaken.
		 *  If it is a databyte then that databyte is added to the framebuffer.
		 *  A value is returned that tells us what this function has done with the
		 *  data (if there was any).  
		 *  \param pFr Framer to which the data will be posted.
		 *  \return state of the state machine.
		 */
		MSG_T FramerReceive(){
			byte c=0;
			
			if(!_mySerial.BytesAvailable())
			{
				return MSG_T.MSG_NO_INPUT;
			}
			c=_mySerial.ReadByte();
			
			if(_dle_received)
			{
				if(_state==STATE.DECODING)
				{
					_frameData[_pData]=c;
					_pData++;
					_dle_received=false;
				}
			}
			else
			{
				switch(c)
				{
				case (byte)FLAGS.LEADING_FLAG:
					_pData=0;
					_state=STATE.DECODING;
					break;
				case (byte)FLAGS.TRAILING_FLAG:
					_state=STATE.WAITING;
					return MSG_T.MSG_RX_COMPLETE;
				case (byte)FLAGS.ACK0_FLAG:
					return MSG_T.MSG_ACK_RECEIVED;
				case (byte)FLAGS.DLE_FLAG:
					if(_state==STATE.DECODING)
					{
						_dle_received=true;
					}
					break;
				default:
					if(_state==STATE.DECODING)
					{
						_frameData[_pData]=c;
						_pData++;
					}
					break;
				}//switch
			}
			//RX-Buffer mustn't overflow
			if(_pData==FRAMESIZE-1)
			{
				_pData=0;
				return MSG_T.MSG_RX_OVERFLOW;
			}
			return MSG_T.MSG_DECODING;
		}//FramerReceive
		
		/*! Write a databuffer to the serial port.  No CRC will be appended.
		* This function adds the flags for the protocol and performs the 
		* bytestuffing.  Finally it sends the data to the UART.
		*\param buffer Buffer containing the data.
		*\param length Length of the databuffer
		*\param typeFrame Type of frame (info frame or acknowledge frame)
		*/
		void writeString(byte[] buffer, FRAME_TYPE typeFrame){
			switch(typeFrame)
			{
				case FRAME_TYPE.INFO_FRAME:
					//Leading flag
					_mySerial.Write((byte)FLAGS.LEADING_FLAG);
					/*Data:	
					* if a databyte is equal to a flag character, then prepend
					* a DLE_FLAG to it.
					*/
					for(byte i=0;i<buffer.Length;i++)
					{		
						if(flagCharacterInData(buffer[i]))
						{
							_mySerial.Write((byte)FLAGS.DLE_FLAG);
						}
						_mySerial.Write(buffer,i,1);
					}
					//Trailing flag
					_mySerial.Write((byte)FLAGS.TRAILING_FLAG);
					break;		        	
				case FRAME_TYPE.ACK_FRAME:
					_mySerial.Write((byte)FLAGS.ACK0_FLAG);
					break;		        	
			}    	
		}//writeString

		/*!Check if the character is a flag character.
		 *\param c The character
		 *\return TRUE when c is a flag character else FALSE.
		 */
		bool flagCharacterInData(byte c){
		  return (
				c==(byte)FLAGS.LEADING_FLAG||
				c==(byte)FLAGS.TRAILING_FLAG||
				c==(byte)FLAGS.DLE_FLAG||
				c==(byte)FLAGS.ACK0_FLAG ? true : false);
		}//flagCharacterInData

	}//class
}
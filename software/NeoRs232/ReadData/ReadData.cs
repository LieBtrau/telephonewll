using System;
using System.IO;
using System.IO.Ports;
using System.Threading;

namespace phonePC
{
	
    class Program
    {
		enum PROTOCOL
		{
			REQ_OKI_DATA,
			RESP_OKI_DATA
		}
		
		public static void Main(string[] args)
	    {
	        SerialPortTest myTest = new SerialPortTest();
			NeoRs232 neo=new NeoRs232(myTest);
			//Wait for POTSduino to come online
			Thread.Sleep(3000);
			while(true)
			{
				//neo.txFrameWithAck(new byte[]{(byte)PROTOCOL.REQ_OKI_DATA});
				if(neo.frameAvailable())
				{
					processFrame(neo.getFrame(),neo);
				}else
				{
					System.Threading.Thread.Sleep(1);
				}
			}
	    }
		
		static void processFrame(Frame frame, NeoRs232 neo)
		{
			if(frame.Type==FRAME_TYPE.INFO_FRAME)
			{
				switch((PROTOCOL)frame.Data[0])
				{
				case PROTOCOL.REQ_OKI_DATA:
					byte[] buf=new byte[95];
					buf[0]=(byte)PROTOCOL.RESP_OKI_DATA;
					for(int i=1;i<95;i++)
					{
						buf[i]=(byte)i;
					}
					neo.txFrameWithoutAck(buf);
					break;
				case PROTOCOL.RESP_OKI_DATA:
//					Console.Write("I got this frame: ");
//					for(int i=1;i<frame.Data.Length;i++){
//						Console.Write(frame.Data[i]+" ");
//					}
//					Console.Write(Environment.NewLine);
					break;
				default:
					Console.WriteLine("I got something unknown here");
					break;
				}
			}
		}
		
	}
	
	/// <summary>
	/// Serial port test.
	/// </summary>
	public class SerialPortTest{
		private SerialPort _mySerial;
		
		public SerialPortTest()
		{
	        if (_mySerial != null)
			{
	            if (_mySerial.IsOpen)
				{
	                _mySerial.Close();
				}
			}
			//When using 57.6kbaud, then CTS-is never held and UART can send continuously.
            _mySerial = new SerialPort("/dev/ttyACM0", 115200,Parity.None,8,StopBits.One);
			//By using the request to send handshake, the Write-function can be called as many times as possible.
			//No need to check CTS-holding.  The oscilloscope shows that it really works that way.
	        _mySerial.Handshake = System.IO.Ports.Handshake.None;//.RequestToSend;
	        _mySerial.ReadTimeout = 100;
			_mySerial.WriteTimeout= 100;
			_mySerial.RtsEnable=true;
	        _mySerial.Open();
		}
		
		public bool BytesAvailable()
		{
		return (_mySerial.BytesToRead==0 ? false:true);
		}
		
		public byte ReadByte()
		{
			byte data;
			data=(byte)_mySerial.ReadByte();
//			Console.WriteLine("Received data: "+ (char)(data)+ "\t"+(data));
			return data;
		}
		
		public void Write(byte data)
		{
			_mySerial.Write(new byte[]{data},0,1);			
		}
		
		public void Write(byte[] buffer, int offset, int count)
		{
			_mySerial.Write(buffer,offset,count);
		}
	}
}
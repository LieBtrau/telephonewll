using System;

namespace phonePC
{
	public class Crc16
	{
		public Crc16 ()
		{
		}

		/*!check and strip the CRC16 of the frame.
		*\param pFramer A frame containing the received data.
		*\return TRUE when correct CRC, else return FALSE, when the CRC is correct,
		*	then the CRC will be removed from the frame.
		*/
		public bool stripCRC(byte[] dataPacket, int length, out byte[] aMsg){
			UInt16 crcRxed=0, crcCalculated;
			aMsg=null;
			
			if(length<2){
				return false;
			}
			
			crcCalculated=get_crc(dataPacket,length-2);
			
			//read CRC from received data (little endian volgorde)
			crcRxed=(UInt16) ((dataPacket[length-1]<<8) | dataPacket[length-2]);
			
			if(crcRxed!=crcCalculated)
			{
				return false;
			}
			//Remove the CRC
			aMsg=new byte[length-2];
			Array.Copy(dataPacket,aMsg,aMsg.Length);
			return true;
		}//stripCRC


		/*! Calculate a CRC16 of a buffer and append it to that buffer.
		*\param buffer databuffer of which CRC16 will be calculated and to which
		*		CRC16 will be appended.
		*\param length number of bytes in the buffer
		*\return the same buffer, with CRC-appended (little endian)
		*/
		public void appendCRC(ref byte[] buffer, int length){
			byte[] outbuf=new byte[length+2];
			Array.Copy(buffer,outbuf,length);
			UInt16 crcValue=get_crc(buffer,length);
			outbuf[length]=(byte)(crcValue&0xFF);
			outbuf[length+1]=(byte)((crcValue>>8)&0xFF);
			buffer=outbuf;
		}//appendCRC


		/*!Get the CRC16 of a databuffer.
		*\param buf containing data of which the CRC will be calculated
		*\param length total length of the buffer
		*\return CRC16
		*/
		UInt16 get_crc(byte[] buf, int length){
			UInt16 crc=0xFFFF;
			for(byte i=0;i<length;i++)
			{
				crc=_crc_ccitt_update(crc,buf[i]);
			}
			return crc;
		}//get_crc
		
		//http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
	    UInt16 _crc_ccitt_update(UInt16 crc, byte data)
    	{
        	data ^= (byte)(crc & 0xFF);
        	data ^= (byte)(data << 4);
        	return (UInt16)(((((UInt16)data << 8) | ((crc>>8) & 0xFF)) ^ (byte)(data >> 4) ^ ((UInt16)data << 3)));
    	}
	}
}


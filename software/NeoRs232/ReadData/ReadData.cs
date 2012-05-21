using System;
using System.IO;
using System.IO.Ports;
using System.Threading;

namespace phonePC
{
	
    class Program
    {
		
		public static void Main(string[] args)
	    {
	        SerialPortTest myTest = new SerialPortTest();
			NeoRs232 neo=new NeoRs232(myTest);
			//Wait for POTSduino to come online
			Thread.Sleep(3000);
			if(neo.txFrameWithAck(new byte[]{4,5,6}))
			{
				Console.WriteLine("O yeah, I got it.");
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
			Console.Write((char)data);
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
using System;
using System.IO.Ports;
using System.Threading;

namespace phonePC
{
	
    class Program
    {
		public static void Main(string[] args)
	    {
	        SerialPortTest myTest = new SerialPortTest();
			myTest.read();
	    }
	}
	
	/// <summary>
	/// Serial port test.
	/// </summary>
	class SerialPortTest{
		private SerialPort mySerial;
		public const int BYTECOUNT=10000;//Maximum 100bytes/package.  More bytes will be lost when in loopback mode.
		byte[] buff=new byte[1000];
		
		public SerialPortTest()
		{
	        if (mySerial != null)
			{
	            if (mySerial.IsOpen)
				{
	                mySerial.Close();
				}
			}
			//When using 57.6kbaud, then CTS-is never held and UART can send continuously.
            mySerial = new SerialPort("/dev/ttyUSB1", 115200,Parity.None,8,StopBits.One);
	        mySerial.Open();
			//By using the request to send handshake, the Write-function can be called as many times as possible.
			//No need to check CTS-holding.  The oscilloscope shows that it really works that way.
	        mySerial.Handshake = System.IO.Ports.Handshake.RequestToSend;
	        mySerial.ReadTimeout = 1000;
			mySerial.WriteTimeout= 1000;
			mySerial.RtsEnable=true;
			for(int i=0;i<buff.Length;i++)
			{
				buff[i]=(byte)i;
			}
		}
		
		public void read()
		{
			const int TIMELAPSE=10;
			int i=0;
			DateTime departure = DateTime.Now;
			Console.WriteLine("Receiving data during " + TIMELAPSE + " seconds to calculate throughput.");
			while(true)
			{
				try{
					mySerial.ReadByte();
					i++;
				}catch(Exception e){
					Console.WriteLine(i + Environment.NewLine + e.Message);
				}
				TimeSpan ts=DateTime.Now-departure;
				if(ts.TotalSeconds>TIMELAPSE)break;
			}
			Console.WriteLine("Throughput = " + i/TIMELAPSE*8+"bps");
		}
		
	}
}
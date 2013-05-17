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
			System.Threading.Thread.Sleep(15000);
			myTest.write();
	    }
	}
	
	/// <summary>
	/// Serial port test.
	/// </summary>
	class SerialPortTest{
		private SerialPort mySerial;
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
            mySerial = new SerialPort("/dev/ttyACM0", 115200,Parity.None,8,StopBits.One);
	        mySerial.Open();
//			//By using the request to send handshake, the Write-function can be called as many times as possible.
//			//No need to check CTS-holding.  The oscilloscope shows that it really works that way.
	        mySerial.Handshake = System.IO.Ports.Handshake.RequestToSend;
//	        mySerial.Handshake=System.IO.Ports.Handshake.None;
			mySerial.ReadTimeout = 1000;
			mySerial.WriteTimeout= 10000;
			mySerial.RtsEnable=true;
//			for(int i=0;i<buff.Length;i++)
//			{
//				buff[i]=(byte)i;
//			}
		}
		
	    public void write()
	    {
		    try
		    {
				byte[] buff = File.ReadAllBytes("TalkTherm.la");		    
			}
		    catch 
		    {
		    }
			Console.WriteLine("Infinitely sending data");
			while(true){
				mySerial.Write(buff,0,buff.Length);
			}
	    }
		
	}
}

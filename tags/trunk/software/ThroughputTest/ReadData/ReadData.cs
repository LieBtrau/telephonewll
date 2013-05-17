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
			myTest.read();
			
	    }
	}
	
	/// <summary>
	/// Serial port test.
	/// </summary>
	class SerialPortTest{
		private SerialPort mySerial;
		public const int BYTECOUNT=40000;//Maximum 100bytes/package.  More bytes will be lost when in loopback mode.
		byte[] buff=new byte[BYTECOUNT];
		
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
			//By using the request to send handshake, the Write-function can be called as many times as possible.
			//No need to check CTS-holding.  The oscilloscope shows that it really works that way.
	        mySerial.Handshake = System.IO.Ports.Handshake.RequestToSend;
	        mySerial.ReadTimeout = 5000;
			mySerial.WriteTimeout= 1000;
			mySerial.RtsEnable=true;
	        mySerial.Open();
		}
		
		public void read()
		{
			int i=0;
			byte state=0;
			byte data1, data2=0;
			while(i<BYTECOUNT)
			{
				try{
					switch(state)
					{
					case 0:
						data1=data2;
						data2=(byte)mySerial.ReadByte();
						Console.Write((char)data1);
						if(data1==0x08 && data2==0x08)
						{
							i++;
						}
						if(i==23)
						{
							state=1;
							i=0;
							data2=(byte)mySerial.ReadByte();
						}
						break;
					case 1:
						buff[i++]=(byte)mySerial.ReadByte();
//						i++;
//						data1=data2;
//						data2=(byte)mySerial.ReadByte();
//						if(data2!=data1+1 && data2!=0)
//						{
//							Console.WriteLine(i);
//							return;
//						}
						break;
					}
				}catch(Exception e){
					Console.WriteLine(i + Environment.NewLine + e.Message);
				}
			}
			Console.WriteLine("Data received, now saving to file");
		    BinaryWriter Writer = null;
		    string Name = "wavedata.la";
		    try
		    {
		        // Create a new stream to write to the file
		        Writer = new BinaryWriter(File.OpenWrite(Name));
		
		        // Writer raw data                
		        Writer.Write(buff);
		        Writer.Flush();
		        Writer.Close();
		    }
		    catch 
		    {
		    }
		}
		
	}
}

/*************************************************************************
**  Device: MCP4921                                                 	**
**  File:   AH_MCP4921.h - Library for get out analog voltage          	**
**								    	**
**  Created by A. Hinkel 2012-12-24                                 	**
**  download from "http://www.arduino-projekte.de"  			**
**									**
**  Based on Code from:							**
**  http://www.sinneb.net/2010/06/mcp4921-12bit-dac-voltage-controller/ **
**									**
**  Released into the public domain.  		                    	**
**                                                                  	**
**                                                                  	**
**************************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AH_MCP4921.h"
#include "SPI.h"

//*************************************************************************
AH_MCP4921::AH_MCP4921(int SDI, int SCK,int CS)
{
 _DATAOUT = SDI;
 _CLOCK = SCK;
 _SLAVESELECT = CS;
  _MODE = 0; //PIN mode

 pinMode(_DATAOUT,OUTPUT);
 pinMode(_CLOCK,OUTPUT);
 pinMode(_SLAVESELECT,OUTPUT);
 digitalWrite(_SLAVESELECT,HIGH);

// setValue(0);    //Set output voltage

}
//*************************************************************************
AH_MCP4921::AH_MCP4921(int CS)  //SPI mode
{
 _SLAVESELECT = CS;
 _MODE = 1; //SPI mode

 pinMode(_SLAVESELECT,OUTPUT);
 digitalWrite(_SLAVESELECT,HIGH);

 SPI.begin();
 SPI.setClockDivider(SPI_CLOCK_DIV2 );//LieBtrau: faster SPI needed, update rate >30KHz, old value = SPI_CLOCK_DIV16
 setValue(0);    //Set output voltage

}
//************************************************************************

AH_MCP4921::AH_MCP4921(AH_MCP4921 *X)
{
  AH_MCP4921(X->_DATAOUT, X->_CLOCK,X->_SLAVESELECT);
}


//************************************************************************

void AH_MCP4921::setValue(int Value) {
  if (_MODE == 0)
    {sendValue_PIN(Value);}
  else
    {sendValue_SPI(Value);}  
}

//************************************************************************ 

void AH_MCP4921::Clock() {
  digitalWrite(_CLOCK, HIGH);
  digitalWrite(_CLOCK, LOW);
//  delay(1);
}

//************************************************************************ 
void AH_MCP4921::sendHeader() {
  					// bit 15
					// 0 write to DAC
					// 1 ignore command
  digitalWrite(_DATAOUT,LOW);
  Clock();
					// bit 14 Vref input buffer control
					// 0 unbuffered 
					// 1 buffered
  digitalWrite(_DATAOUT,LOW);
  Clock();
					// bit 13 Output Gain selection
					// 0 2x
					// 1 1x
  digitalWrite(_DATAOUT,HIGH);
  Clock();
					// bit 12 Output shutdown control bit
					// 0 Shutdown the device
					// 1 Active mode operation
  digitalWrite(_DATAOUT,HIGH);
  Clock();
}
 
//************************************************************************
void AH_MCP4921::sendValue_PIN(int value) {

 digitalWrite(_SLAVESELECT,LOW);             // initiate data transfer with 4921
   sendHeader();			     // send 4 bit of header
   for(int i=11;i>=0;i--){
     digitalWrite(_DATAOUT,((value&(1<<i)))>>i);   // send data
     Clock();
   }
 digitalWrite(_SLAVESELECT,HIGH);     // finish data transfer
} 
 
 
//************************************************************************
void AH_MCP4921::sendValue_SPI(int value) {

 byte msb = 0;
 byte lsb = 0;
 int dac = 0;			//DAC_A  at MCP4921 have only once DAC
 int input_buffer = 0;		//unbuffered
 int gain = 1; 			//gain 1x
 int active_mode = 1;		//active

 byte header = dac<<7 | input_buffer<<6 |  gain<<5 | active_mode<<4;   //B00110000

 digitalWrite(_SLAVESELECT,LOW);      // initiate data transfer with 4921

   msb = (header & 0xf0) | ((value>>8) & 0x0f);
   lsb = (byte) (value & 0xff);

   SPI.transfer(msb);			//send 1st byte
   SPI.transfer(lsb);			//send 2nd byte

//   Serial.print("msb:"); Serial.println(msb);
//   Serial.print("lsb:"); Serial.println(lsb);

 digitalWrite(_SLAVESELECT,HIGH);   // finish data transfer

} 
 
 


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

#ifndef AH_MCP4921_h
#define AH_MCP4921_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class AH_MCP4921
{
  public:
    AH_MCP4921(int SDI, int SCK,int CS);  //PIN mode
    AH_MCP4921(int CS); 		  //SPI mode	
    AH_MCP4921(AH_MCP4921 *X);
    void setValue(int Value);

  private:
    int _DATAOUT;
    int _CLOCK;
    int _SLAVESELECT;
    int _MODE;

    void sendHeader();
    void sendValue_PIN(int value);
    void sendValue_SPI(int value);
    void Clock();
};

#endif
//For debugging purposes:
//  Linux stores the build output in /tmp/buildxxxxxxxx.tmp and deletes it after closing Arduino

#include <SPI.h>
#include "slic.h"
#include "pcm.h"
#include "g711.h"
#include "oki_adpcm.h"
#include "neoRs232.h"

int aData[50];
/*!The framer contains pointers to work with the incoming data.*/
FRAMER framer;

/*!Buffer that will contain the incoming frames*/
byte frameData[FRAMESIZE];

void setup() {
  Serial.begin(115200);
 Serial.println("Welcome to POTSduino!");
  pinMode(PIN_RESET,OUTPUT);
  pinMode(PIN_FSYNC,OUTPUT);                 //OC1A-pin set as output 
  pinMode(PIN_PCLK,OUTPUT);                  //OC2B-pin set as output
  pinMode(PIN_nCS,OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_DRX, OUTPUT);
  pinMode(PIN_DTX, INPUT);
  pinMode(6,OUTPUT);//debugging
  digitalWrite(PIN_DRX, LOW);

  digitalWrite(PIN_RESET, LOW);                //Put SLIC into reset
  
//  setupPcmGateway();
//  digitalWrite(PIN_nCS, HIGH);
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setDataMode(SPI_MODE3);                //data change on falling clock edge, clock high when inactive
//  SPI.setClockDivider(SPI_CLOCK_DIV128);     //Only for easier debugging with my slow scope. 
//  SPI.begin();                               //Start the SPI library
//  digitalWrite(PIN_RESET, HIGH);             //Put board into operating mode
//  delay(10);                                 //Wait 10ms.
//  if(!slicInit(10)){
//    Serial.println("Initialisation failed");
//    return;
//  }
//  Serial.println("Initialisation success");
  //ringing(true);
  
  byte data[3]={1,2,3};
  receiveInit(&framer, frameData);
//  txFrameWithAck(data,3,&framer);
}//setup

void loop() {
  receiveRun(&framer);
//  static byte stateSM=0;
//  static byte i=0;
//  static oki_adpcm_state_t okiState;
//  int sample;
//  byte aLaw;
//  
//  switch(stateSM)
//  {
//  case 0:
//    if(offHook())
//    {
//      enableDiallingTone();
//      stateSM=1;
//    }
//    break;
//  case 1:
//    controlPcmGateway(true);
//    okiState.last=0;
//    okiState.step_index=0;
//    for(i=0;i<24;i++)                                  //Reset ADPCM state machine
//    {
//      Serial.write(0x08);
//    }
//    stateSM=2;
//    break;
//  case 2:
//    if(i++<50)
//    {
//          if (Serial.available() > 0) {
//                // read the incoming byte:
//                i = Serial.read();
//          }
//        aLaw=PCMcomm(0);                              //Get sample N
//        Serial.write(aLaw);
//        aData[i]=aLaw;
//        //PORTD|=0x40;//debug
//        sample=aLawDecode(aLaw);
//        aData[i]=oki_encode(&okiState,sample)<<4;
//        aLaw=PCMcomm(0x00);                              //Get sample N+1
//        sample=aLawDecode(aLaw);
//        aData[i]|= oki_encode(&okiState,sample);
        //Serial.write(aLaw);
        //PORTD&=0xBF;//debug
//    }else
//    {
//      stateSM=3;
//    }
//    break;
//  case 3:
//    controlPcmGateway(false);
//    for(int j=0;j<50;j++)
//    {
//      Serial.print("0x");
//      Serial.println(aData[j],HEX);
//    }
//    stateSM=4;
//  default:
//    break;
//  }
}//loop









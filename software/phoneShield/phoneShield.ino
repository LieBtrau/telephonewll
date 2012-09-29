//For debugging purposes:
//  Linux stores the build output in /tmp/buildxxxxxxxx.tmp and deletes it after closing Arduino

#include <SPI.h>
#include "slic.h"
#include "pcm.h"
#include "g711.h"
#include "oki_adpcm.h"
#include "neoRs232.h"
#include "RingBuffer.h"
#include "RotaryDialer.h"

typedef enum{
  REQ_OKI_DATA,
  RESP_OKI_DATA
}
PROTOCOL;

/*!The framer contains pointers to work with the incoming data.*/
FRAMER framer;

/*!Buffer that will contain the incoming frame*/
byte frameData[FRAMESIZE];
//Buffer that contains data that must be sent
byte sendData[FRAMESIZE];
RingBuffer rb(100);
RotaryDialer rd;

void setup() {
  rd=RotaryDialer();
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

  setupPcmGateway();
  digitalWrite(PIN_nCS, HIGH);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);                //data change on falling clock edge, clock high when inactive
  SPI.setClockDivider(SPI_CLOCK_DIV128);     //Only for easier debugging with my slow scope. 
  SPI.begin();                               //Start the SPI library
  digitalWrite(PIN_RESET, HIGH);             //Put board into operating mode
  delay(10);                                 //Wait 10ms.
  if(!slicInit(10)){
    Serial.println("Initialisation failed");
    return;
  }
  Serial.println("Initialisation success");

  receiveInit(&framer, frameData);
  rd.setup();
  //ringing(true);
}//setup

void loop() {
  byte yDialednumber;
  receiveRun(&framer);
  if(rd.getNextNumber(yDialednumber))
  {
    Serial.print(yDialednumber,DEC);
  }
//  static byte stateSM=0;
//  static byte i=0;
//  static oki_adpcm_state_t okiState;
//  byte okiSample;
//  int sample;
//
//  switch(stateSM)
//  {
//  case 0:
//  if(offHook())
//  {
//    enableDiallingTone();
//    stateSM=1;
//  }
//  break;
//  case 1:
//  controlPcmGateway(true);
  //    okiState.last=0;
  //    okiState.step_index=0;
  //    for(i=0;i<24;i++)                                  //Reset ADPCM state machine
  //    {
  //      push(0x08);
  //    }
//      stateSM=2;
//      break;
//    case 2:
//static byte sample;
//PCMcomm(0);
//rb.push(sample++);
//if(rb.count()>50)
//{
//    sendData[0]=RESP_OKI_DATA;
//    for(int i=1;i<50;i++)
//    {
//      sendData[i]=rb.pop();
//    }  
//    txFrameWithoutAck(sendData, 50, &framer);
//}

  //        sample=aLawDecode(PCMcomm(0));                //Get sample N
  //        okiSample = oki_encode(&okiState,sample)<<4;
  //        sample=aLawDecode(PCMcomm(0));                //Get sample N+1
  //        okiSample |= oki_encode(&okiState,sample);
  //        push(okiSample);
  //  default:
  //    break;
  //  }
}//loop

void processFrame(byte* buf, byte length){
  byte c;
  byte i=1;
  //Serial.println("got it");//debug
  switch(buf[0])
  {
  case REQ_OKI_DATA:
    //Respond with OKI data
    sendData[0]=RESP_OKI_DATA;
    //    while(pop(&c))
    //    {
    //      sendData[i++]=c;
    //    }
    for(int i=1;i<95;i++){
      sendData[i]=i;
    }
    while(true){
      txFrameWithoutAck(sendData, 79, &framer);
      delay(11);
      //7=23K/23K
      //8=27K/28K
      //9=30K/31K
      //10=29K/31K
      //28K/29K
    }
    break;
  case RESP_OKI_DATA:
    //Peer sends us OKI data
    //    for(int i=1;i<length;i++){
    //      Serial.println(buf[i],DEC);
    //    }
  default:
    break;
  }
}











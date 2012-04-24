//For debugging purposes:
//  Linux stores the build output in /tmp/buildxxxxxxxx.tmp and deletes it after closing Arduino

#include <SPI.h>
#include "slic.h"
#include "pcm.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to POTSduino!");
  pinMode(PIN_RESET,OUTPUT);
  pinMode(PIN_FSYNC,OUTPUT);                 //OC1A-pin set as output 
  pinMode(PIN_PCLK,OUTPUT);                  //OC2B-pin set as output
  pinMode(PIN_nCS,OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_DRX, OUTPUT);
  pinMode(6,OUTPUT);//debugging
  digitalWrite(PIN_DRX, LOW);

  digitalWrite(PIN_RESET, LOW);              //Put board into reset
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
  ringing(true);

}//setup

void loop() {
  static byte state=0;
  switch(state){
  case 0:
    if(offHook()){
      enableDiallingTone();
      state=1;
    }
    break;
  case 1:
    setupVoiceChannel();
    enablePcmGateway();
    state=2;
    break;
  case 2:
    break;
  }
}//loop






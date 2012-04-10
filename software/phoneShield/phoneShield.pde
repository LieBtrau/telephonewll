/*Timer2 generates PCLK at 256kHz.  That's the lowest permissible rate.
 *The original idea was to generate the FSYNC signal in the ISR of timer2 compare, but
 *There are only 64 cycles available as the ISR runs at 256kHz.  Besides, there would be 
 *hardly time left to do other useful work.
 *So the alternative is to generate the 7.8kHz FSYNC with timer1.
 */

#include <SPI.h>
#include "slic.h"
#include "pcm.h"

void setup() {
  noInterrupts();
  Serial.begin(9600);
  Serial.println("Welcome to POTSduino!");
  pinMode(PIN_RESET,OUTPUT);
  pinMode(PIN_FSYNC,OUTPUT);                 //OC1A-pin set as output 
  pinMode(PIN_PCLK,OUTPUT);                  //OC2B-pin set as output
  pinMode(PIN_nCS,OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_DRX, OUTPUT);
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
  interrupts();                              //Enable interrupts again.
  ringing(true);

}//setup

void loop() {
  if(offHook()){
    enableDiallingTone();
  }
}//loop





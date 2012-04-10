#include "WProgram.h"  //contains Arduino constants

//set up PCLK=256kHz, duty cycle 50% (4 timeslots/frame)
//set up FS=7.8kHz, duty cycle=1/32, synchronous rising edge with PCLK.
void setupPcmGateway(){
  //http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM

  //Setting up PCLK
  bitClear(PRR,PRTIM2);            //Power up Timer2
  bitSet(TCCR2B,WGM22);            //Put timer 2 in fast PWM-mode with top at OCRA
  bitSet(TCCR2A,WGM21);
  bitSet(TCCR2A,WGM20);
  bitSet(TCCR2A, COM2B1);          //Set OC2B-pin upon compare match
  bitSet(TCCR2A, COM2B0);
  bitClear(TCCR2B, CS22);          //No prescaler, fTimer = fclk = 16MHz
  bitClear(TCCR2B, CS21);
  bitSet(TCCR2B, CS20);
  OCR2A=63;                        //In fast PWM-mode, timer will be cleared when it reaches OCR2A value.
  //So the timer frequency becomes: fclk/(N*(1+OCR2A)) = 16M/(1*(1+63))=256kHz
  OCR2B=31;                        //50% dutycycle clock on OC2B-pin.

  //Setting up FSYNC: 
  bitClear(PRR,PRTIM1);            //Power up Timer1
  bitSet(TCCR1A,COM1A1);           //Clear OC1A-pin upon compare match 
  bitClear(TCCR1A,COM1A0);
  bitClear(TCCR1A,WGM11);          //Use fast PWM-mode 8bit
  bitSet(TCCR1A,WGM10);
  bitSet(TCCR1B,WGM12);
  bitClear(TCCR1B,WGM13);
  bitClear(TCCR1B,CS12);           //Set prescaler /8, so the FSYNC frequency becomes: 16MHz/8/256 = 7.8kHz
  bitSet(TCCR1B,CS11);
  bitClear(TCCR1B,CS10);
  OCR1A=7;                         //Set duty cycle to 1/32

  //Synchronized timer1 & timer2 (using only an old HM412 scope)
  TCNT1=0;
  TCNT2=43;
}


/*Timer2 generates PCLK at 256kHz.  That's the lowest permissible rate.
 *The original idea was to generate the FSYNC signal in the ISR of timer2 compare, but
 *There are only 64 cycles available as the ISR runs at 256kHz.  Besides, there would be 
 *hardly time left to do other useful work.
 *So the alternative is to generate the 7.8kHz FSYNC with timer1.
 *
 *MCU Register definitions can be found on: /usr/lib/avr/include/avr
 */
 
#include "WProgram.h"              //contains Arduino constants
byte yBitCtr=0;
byte yDTX=0;                       //Data generated by SLIC
byte yDRX;

void enablePcmGateway(){
  bitSet(TIMSK1,TOIE1);            //Enable interrupts when timer1 reaches TOP of counter.
}

void setupPcmGateway(){
  //http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM

  //Setting up PCLK: 256kHz, duty cycle 50% (4 timeslots/frame)
  bitClear(PRR,PRTIM2);            //Power up Timer2
  bitSet(TCCR2B,WGM22);            //Put timer 2 in fast PWM-mode with top at OCR2A
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

  //Setting up FSYNC: 7.8kHz, duty cycle=1/32, synchronous rising edge with PCLK.
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

  //Synchronizing timer1 & timer2 (using only an old HM412 scope)
  TCNT1=0;
  TCNT2=43;
}

//For every databyte shifted out on DTX, this ISR will be called 10 times.  The data shifted in during the first two 
//calls will be rejected.
//The two extra calls were necessary to be sure that the DTX data is sampled in the middle of the bit.
ISR(TIMER1_OVF_vect)
{
  TIMSK0&=0xFE;                    //Disable interrupts on timer0, because it causes jitter on TIMER2-interrupts.
  bitSet(TIMSK2,OCIE2A);           //Enable interrupt on OCR2B compare match
  yDRX=0xA5;//debug
}

//Timer2 runs away with quite some processing resources, so it gets only enabled by timer1 when data is actually 
//to be transferred over the PCM-gateway.
ISR(TIMER2_COMPA_vect){
  PORTD|=0x40;//debug
  
  yDTX<<=1;                        //get DTX data and shift it in
  yDTX|= PINB & 0x01;              //DTX connected to Arduino.D8 = PORTB0
  
  if(yDRX & 0x80){                  //shift out DRX data
    PORTD|=0x80;
  }else{
    PORTD&=0x7F;
  }
  yDRX<<=1;
  
  yBitCtr++;
  if(yBitCtr==10){
    bitClear(TIMSK2,OCIE2A);
    TIMSK0|=1;                      //Enable timer0 again, so that standard Arduino timing functions work again.
    yBitCtr=0;
  }
  PORTD&=0xBF;//debug
}


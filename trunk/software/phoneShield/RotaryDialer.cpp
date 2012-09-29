//Code adapted from: http://www.markfickett.com/stuff/artPage.php?id=377

#include "RotaryDialer.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

RotaryDialer::RotaryDialer() : 
_tPulseState(WAITING_FOR_LOOP_OPENING),_yNumber(0),_uiTimeOfLastPulse(0),_bFinishedNumberRead(false)
{ 
}

void RotaryDialer::setup() {
  bitClear(EICRA,ISC00);//low level on INT0 generates interrupt
  bitClear(EICRA,ISC01);//low level on INT0 generates interrupt
  bitSet(EIMSK,INT0);//enable interrupts on INT0
  bitSet(SREG,SREG_I);//global interrupt enable
}

//Function that will be called when the loop status changes.
//\param yLoopStatus  Current status of the telephone loop: 1=closed (horn picked up), 0=open (horn down)
void RotaryDialer::updatePulseState(byte yLoopStatus) 
{
  static unsigned long lastStateChangeMillis;
  unsigned long currentMillis = millis();
  unsigned long uiPulseWidth;
  
  if (currentMillis < lastStateChangeMillis) 
  {
    // clock wrapped; ignore (but could figure it out in this case)
    lastStateChangeMillis = currentMillis;
    return;
  } 
  else
  {
    uiPulseWidth=currentMillis - lastStateChangeMillis;
    lastStateChangeMillis = currentMillis;
  }
  
  //Belgacom spec.
  switch(_tPulseState)
  {
  case WAITING_FOR_LOOP_OPENING:
    //minimum closure=25ms, 
    if(yLoopStatus==0 && uiPulseWidth>25)
    {    
      _tPulseState=WAITING_FOR_LOOP_CLOSURE;
    }
    break;
  case WAITING_FOR_LOOP_CLOSURE:
    //minimum opening=53ms, slowest pulse rate = 8Hz = 125ms
    if(yLoopStatus==1 && uiPulseWidth>53 && uiPulseWidth<125)
    {
     updatePulseCounter();
    }
     _tPulseState=WAITING_FOR_LOOP_OPENING;
     break;
  }
}

boolean RotaryDialer::getPulsePeriod(unsigned long& uiPulsePeriod) 
{
  unsigned long currentMillis = millis();
  if (currentMillis < _uiTimeOfLastPulse) 
  {
    // clock wrapped; ignore (but could figure it out in this case)
    _uiTimeOfLastPulse = currentMillis;
    return false;
  } 
  else
  {
    uiPulsePeriod=currentMillis - _uiTimeOfLastPulse;
    _uiTimeOfLastPulse = currentMillis;
    return true;
  }
}

//Counts the number of pulses per digit
void RotaryDialer::updatePulseCounter()
{
  unsigned long uiPulsePeriod;
  
  if(!getPulsePeriod(uiPulsePeriod))
  {
    _yNumber=0;
    Serial.println("Can't get pulse period");
    return;
  }
  //pulse cadence: 8 to 12 Hz (125ms to 84ms from pulse to pulse)
  if(uiPulsePeriod<84)
  {
    //too fast pulse
    return;
  }
  if(uiPulsePeriod>125 || _yNumber>10)
  {
    //too long ago since the last digit or too many pulses per digit, reset pulse counter
    _yNumber=0;
    _bFinishedNumberRead=false;
  }
  _yNumber++;
}

boolean RotaryDialer::getNextNumber(byte& yDialedNumber) 
{
  static unsigned long uLastDigitTime;
  unsigned long currentMillis = millis();
  if (currentMillis < uLastDigitTime) 
  {
    // clock wrapped; ignore (but could figure it out in this case)
    uLastDigitTime = currentMillis;
    return false;
  } 
  //interdigit time: min=400ms, max=12s
  if(currentMillis<uLastDigitTime+400)
  {
    return false;
  }
  uLastDigitTime = currentMillis;

  if ((currentMillis < _uiTimeOfLastPulse)) 
  {
    // clock wrapped; ignore (but could figure it out in this case)
    return false;
  } 
  if(_yNumber==0 || (currentMillis - _uiTimeOfLastPulse < 500) || _bFinishedNumberRead)
  {
    return false;
  }
  yDialedNumber=_yNumber % 10;//convert 10 to 0
  _bFinishedNumberRead=true;
  return true;
}





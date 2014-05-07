#include "RotaryDialer.h"
#include "ttimer.h"

RotaryDialer::RotaryDialer(byte ySwitchHookPin, InfoTone& dt) :
    _oHookSwitch(ySwitchHookPin,25)//minimum closure=25ms, 1=off hook, 0=on hook
{
    _pDialTone=&dt;
}

bool RotaryDialer::HornPickedUpEdge(){
   return(_oHookSwitch.update()==1 && _oHookSwitch.risingEdge());
}

bool RotaryDialer::HornPutDownEdge(){
   return(_oHookSwitch.update()==1 && _oHookSwitch.fallingEdge());
}

bool RotaryDialer::HornUp(){
    _oHookSwitch.update();
    return (_oHookSwitch.read()==HIGH);
}

//!Get number dialed by rotary pulses
//!\return When horn has been put on hook again = false, or 4s after last dialed digit = true
bool RotaryDialer::getDialedNumber(String& sNumber){
    sNumber="";
    TTimer t(4000);//return dialed number 4s after dialing the last digit
    t.start();
    //start dialing tone
    _pDialTone->start();
    while(!t.runout()){
        byte yDialedNumber=getNrOfRotaryPulses();
        if(yDialedNumber>0){
            sNumber+=(yDialedNumber%10);//10 pulses must be converted to digit 0
            t.start();
            yDialedNumber=0;
        }
        if(sNumber.length()==0){
            //as long as no pulses have been received, stay in this state
            t.start();
        }
        if(_oHookSwitch.read()==LOW){
            //when phone is on-hook, stop dialing
            return false;
        }
    }
    return true;
}

byte RotaryDialer::getNrOfRotaryPulses(){
    TTimer t1(84);//pulse cadence: 8 to 12 Hz (125ms to 84ms from pulse to pulse)
    byte yPulseCtr=0;
    while(true){
        t1.start();
        //Initial state: horn is off-hook for dialing, waiting for falling edge on pin SHK
        while(_oHookSwitch.update()==0 && !t1.runout());
        if(!_oHookSwitch.fallingEdge()){
            //No falling edge detected, is no new pulse detected, so end of pulse train.
            //Return the number of pulses that have been read.
            return yPulseCtr;
        }
        //Falling edge detected, now wait for rising edge.
        //stop dialing tone
        _pDialTone->stop();
        t1.start();
        while(_oHookSwitch.update()==0 && !t1.runout());
        if(!_oHookSwitch.risingEdge()){
            Serial.println("Wrong state");//can be generated when horn is put on hook again.
            return 0;
        }
        //Rising edge detected, so a full pulse has been detected.  Add it to the number of pulses already received.
        yPulseCtr++;
    }
}




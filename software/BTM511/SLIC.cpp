#include "SLIC.h"

Slic::Slic(byte pinFr, byte pinRm, byte pinShk, byte pinPd, byte pinDialtone):
    _dDialTone(-1, -1, pinDialtone),
    _rd(pinShk, _dDialTone),
    _timer(-1){
    _ringRmState=PAUSE;
    _bRingingBusy=false;
    _pinFR=pinFr;
    _pinRM=pinRm;
    _pinSHK=pinShk;
    _pinPD=pinPd;
}

void Slic::init(){
    pinMode(_pinFR, OUTPUT);
    pinMode(_pinRM, OUTPUT);
    pinMode(_pinSHK, INPUT);
    pinMode(_pinPD, OUTPUT);
    digitalWrite(_pinFR, LOW);
    digitalWrite(_pinRM, LOW);
    powerDown();
}

void Slic::powerDown(){
    digitalWrite(_pinPD,HIGH);
}

void Slic::powerUp(){
    digitalWrite(_pinPD, LOW);
}

bool Slic::HornPickedUpEdge(){
    return _rd.HornPickedUpEdge();
}

bool Slic::HornUp(){
    return _rd.HornUp();
}

bool Slic::HornPutDownEdge(){
    return _rd.HornPutDownEdge();
}

bool Slic::getDialedNumber(String& sNumber){
    return _rd.getDialedNumber(sNumber);
}

void Slic::startRinging(){
    _ringRmState=START_RINGING;
    _bRingingBusy=true;
}

void Slic::stopRinging(){
    _bRingingBusy=false;
    _ringRmState=BEGIN_PAUSE;
}

void Slic::update(){
    if(_rd.HornUp()){
        stopRinging();
    }
    _ringRmState=updateRinging(_ringRmState);
}

//Ringing state machine
Slic::RINGING_STATE Slic::updateRinging(RINGING_STATE state){
    switch(state)
    {
    case START_RINGING:
        digitalWrite(_pinRM, HIGH);
        digitalWrite(_pinFR, LOW);
        _timer.start(25);
        _yPeriodCounter=0;
        return FR_LOW_LEVEL;
        break;
    case FR_LOW_LEVEL:
        if(_timer.runout())
        {
            digitalWrite(_pinFR, HIGH);
            if(_yPeriodCounter++<20){
                _timer.start(25);
                return FR_HIGH_LEVEL;
            }else{
                return BEGIN_PAUSE;
            }
        }
        return FR_LOW_LEVEL;
        break;
    case FR_HIGH_LEVEL:
        if(_timer.runout()){
            digitalWrite(_pinFR, LOW);
            _timer.start(25);
            return FR_LOW_LEVEL;
        }
        return FR_HIGH_LEVEL;
        break;
    case BEGIN_PAUSE:
        digitalWrite(_pinFR, HIGH);
        _timer.start(10);
        return CONTINUE_PAUSE;
        break;
    case CONTINUE_PAUSE:
        if(_timer.runout()){
            digitalWrite(_pinRM, LOW);
            _timer.start(3000);
            return PAUSE;
        }
        return CONTINUE_PAUSE;
        break;
    case PAUSE:
        if(_timer.runout() && _bRingingBusy){
            return START_RINGING;
        }
        return PAUSE;
    }
    return state;
}

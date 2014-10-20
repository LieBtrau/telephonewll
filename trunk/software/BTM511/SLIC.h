#ifndef SLIC_H
#define SLIC_H

#include "Arduino.h"
#include "RotaryDialer.h"
#include "Infotone.h"
#include "ttimer.h"

class Slic{
public:
    typedef enum{
        START_RINGING,
        FR_LOW_LEVEL,
        FR_HIGH_LEVEL,
        BEGIN_PAUSE,
        CONTINUE_PAUSE,
        PAUSE
    } RINGING_STATE;
    Slic(byte pinFr, byte pinRm, byte pinShk, byte pinPd, byte pinDialtone);
    void init();
    bool HornPickedUpEdge();
    bool HornUp();
    bool HornPutDownEdge();
    bool getDialedNumber(String& sNumber);
    void startRinging();
    void stopRinging();
    void update();
    void powerDown();
    void powerUp();
private:
    RINGING_STATE updateRinging(RINGING_STATE state);
    InfoTone _dDialTone;
    RotaryDialer _rd;
    TTimer _timer;
    RINGING_STATE _ringRmState;
    bool _bRingingBusy;
    byte _pinFR;//forward reverse loop voltage switcher
    byte _pinRM;//enable voltage source for ringing generator
    byte _pinSHK;//high=off hook, low=on hook
    byte _pinPD;//power down
    byte _yPeriodCounter;
};

#endif // SLIC_H

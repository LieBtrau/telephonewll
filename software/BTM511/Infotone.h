#ifndef INFOTONE_H
#define INFOTONE_H

#include "ttimer.h"

class InfoTone
{
public:
    InfoTone(int iPulseLength, int iPauseLength, byte yPinDialTone);
    void start();
    void run();
    void stop();
private:
    enum{PULSESTART, PULSE, PAUSE, PAUSESTART} _state;
    void init();
    int _iPulseLength;
    int _iPauseLength;
    bool _bInitialized;
    byte _yPinDialTone;
    TTimer _t;
};

#endif //INFOTONE_H

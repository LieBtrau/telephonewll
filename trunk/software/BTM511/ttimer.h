#ifndef TTIMER_H
#define TTIMER_H
#include "Arduino.h"

class TTimer
{
public:
    TTimer(unsigned long ulTimeout_ms);
    void start(void);
    void start(unsigned long ulTimeout_ms);
    boolean runout(void);
private:
    unsigned long _max_delay;
    unsigned long _last_set;
};

#endif // TTIMER_H

#include "ttimer.h"

TTimer::TTimer(unsigned long ulTimeout_ms)
{
    _max_delay = ulTimeout_ms;
}

void TTimer::start()
{
    _last_set = millis();
}

void TTimer::start(unsigned long ulTimeout_ms)
{
    _max_delay = ulTimeout_ms;
    start();
}

boolean TTimer::runout()
{
    return (millis() - _last_set > _max_delay);
}


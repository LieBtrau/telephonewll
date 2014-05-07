/*References:
 *
 *https://github.com/ivanseidel/DueTimer/blob/master/TimerCounter.md
 *https://github.com/ivanseidel/DueTimer/releases
 *https://github.com/arduino/Arduino/blob/ide-1.5.x/hardware/arduino/sam/cores/arduino/wiring_analog.c#L329
 *http://forum.arduino.cc/index.php?topic=136500.0
 *http://forum.arduino.cc/index.php?topic=146947.0
 *http://interface.khm.de/index.php/lab/experiments/arduino-dds-sinewave-generator/
 */

#include "Arduino.h"
#include "Infotone.h"

// table of 256 sine values / one sine period
static byte sine256[256];
static volatile unsigned long tword_m;  // dds tuning word m

InfoTone::InfoTone(int iPulseLength, int iPauseLength, byte yPinDialTone): _t(-1)
{
    _iPulseLength=iPulseLength;
    _iPauseLength=iPauseLength;
    _bInitialized=false;
    _yPinDialTone=yPinDialTone;
}

void InfoTone::init(){
    const double DFREQ=425.0;        //output frequency
    const double REFCLK=20588;       // DDS clock
    for(int nIndex = 0;nIndex < 256;nIndex++)
    {
        sine256[nIndex] = (byte)((sin(PI/128.0*nIndex)*110)+128);
    }
    PIO_Configure(g_APinDescription[_yPinDialTone].pPort,
            g_APinDescription[_yPinDialTone].ulPinType,
            g_APinDescription[_yPinDialTone].ulPin,
            g_APinDescription[_yPinDialTone].ulPinConfiguration);
    pmc_set_writeprotect(false);
    pmc_enable_periph_clk((uint32_t)TC8_IRQn);
    TC_Configure (TC2, 2,
                  TC_CMR_WAVE |
                  TC_CMR_WAVSEL_UPDOWN_RC | //phase correct PWM
                  TC_CMR_TCCLKS_TIMER_CLOCK2 |
                  TC_CMR_ACPA_TOGGLE); //toggle RA on compare match
    TC_SetRC(TC2, 2, 256);
    NVIC_ClearPendingIRQ(TC8_IRQn);
    TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;
    TC2->TC_CHANNEL[2].TC_IDR = ~TC_IER_CPCS;
    NVIC_EnableIRQ(TC8_IRQn);
    tword_m=pow(2,32)*DFREQ/REFCLK;  // calulate DDS new tuning word
    _bInitialized=true;
}

void InfoTone::start(){
    if(!_bInitialized){
        init();
    }
    TC_Start (TC2, 2);
}

void InfoTone::stop(){
    TC_Stop (TC2, 2);
}

void InfoTone::run(){
    switch(_state){
    case PULSESTART:
        start();
        if(_iPulseLength>-1){
            _t.start(_iPulseLength);
            _state=PULSE;
        }
        break;
    case PULSE:
        if(_t.runout()){
            _state=PAUSESTART;
        }
        break;
    case PAUSESTART:
        stop();
        if(_iPauseLength>-1){
            _t.start(_iPauseLength);
            _state=PAUSE;
        }
        break;
    case PAUSE:
        if(_t.runout()){
            _state=PULSESTART;
        }
        break;
    default:
        _state=PULSESTART;
    }
}

void TC8_Handler(){
    TC_GetStatus(TC2, 2);
    static unsigned long phaccu;   // phase accumulator
    phaccu+=tword_m;
    TC_SetRA (TC2, 2, sine256[phaccu >> 24]);
}

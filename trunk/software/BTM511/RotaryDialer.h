#include "Bounce.h"
#include "Infotone.h"
#include "Arduino.h"

/**
 * Read numbers from a rotary dial (or otherwise using pulse dialing; see
 * http://en.wikipedia.org/wiki/Rotary_dial ).
 *
 * See the README for further documentation.
 */
class RotaryDialer {
private:
    Bounce _oHookSwitch;
    InfoTone* _pDialTone;
    byte getNrOfRotaryPulses();
public:
    RotaryDialer(byte ySwitchHookPin, InfoTone& dt);
    bool getDialedNumber(String& sNumber);
    bool HornPickedUpEdge();
    bool HornPutDownEdge();
    bool HornUp();
};




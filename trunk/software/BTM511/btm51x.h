#ifndef btm51x_h
#define btm51x_h
#include "Arduino.h"

class BTM51X{
public:
    typedef enum {
        IDLE,
        STARTHDR,
        STARTHDR2,
        PACKET_STARTED,
        FOOTER,
        FOOTER2
    }PACKET_STATE;
    typedef enum {
        NO_EVENT,
        UNKNOWN_EVENT,
        PAIRING_EVENT,
        SLC_EVENT,
        CALL_EVENT,
        CALLSETUP_EVENT //HFP.SPEC 1.5, section 4.33.2
    } EVENTS;
    typedef enum{
        UNKNOWN,
        FAILED,
        SUCCEEDED
    }PAIRING_STATE;
    typedef enum{
        UNKNOWN1,
        INITIALIZED,
        RELEASED
    }SLC_STATE;
    typedef enum{
        NOPE,
        PACKET,
        EVENT
    }SM_STATE;
    typedef enum{    //3.6.6.1.21 Hands-free status
        NOT_CONNECTED=0,
        SLC_CONNECTED=1,
        AUDIO_CONNECTED=2,
        IN_CALL_SLC_CONNECTED=3,
        IN_CALL_AUDIO_CONNECTED=4,
        CALL_SETUP_SLC_CONNECTED=5,
        CALL_SETUP_AUDIO_CONNECTED=6
    }HFP_STATE;
    typedef enum{
        LED_OFF,
        LED_ON,
        LED_BLINKING,
        LED_PULSING
    }LED_STATE;
    BTM51X(USARTClass& ser);
    void begin();
    bool phoneConnectionState(HFP_STATE& hfp);
    bool getEvents(EVENTS& event);
    SLC_STATE getSlcState();
    byte getCallSetupState();
    byte getCallState();
    bool call(const String& sNumber);
    bool answerCall();
    bool terminateCall();
    void send(const String& buffer);
    void setLedState(LED_STATE state);
private:
    bool readPacket();
    bool parseEvents(EVENTS& event);
    SM_STATE incomingDataStateMachine(char c);
    bool requestRespond(const String& inBuffer);
    USARTClass* _ser;
    String _strRespBufferBT;
    PACKET_STATE _ps;
    PAIRING_STATE _pairState;
    SLC_STATE _slcState;
    byte _callSetupState;
    byte _callState;
    LED_STATE _ledState;
    SM_STATE readLine();
};

#endif

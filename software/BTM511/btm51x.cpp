//Table 4-2: BTM ATI Commands
//ATI65: HF connection status

#include "btm51x.h"
#include "Arduino.h"
#include "ttimer.h"

#if defined(__arm__)
BTM51X::BTM51X(USARTClass& ser){
#elif defined(__AVR__)
BTM51X::BTM51X(AltSoftSerial& ser){
#endif    
    _ser=&ser;
    _ps=STARTHDR;
    _slcState=UNKNOWN1;
    _pairState=UNKNOWN;
    _callSetupState=0;
    _ledState=LED_OFF;
}

void BTM51X::begin(){
    _ser->begin(9600);
}

void BTM51X::setLedState(LED_STATE state){
    if(state==_ledState){
        return;
    }
    _ledState=state;
    switch(state){
    case LED_OFF:
        send("ats335=0");
        break;
    case LED_ON:
        send("ats335=1");
        break;
    case LED_BLINKING:
        send("ats335=2");
        break;
    case LED_PULSING:
        send("ats335=3");
        break;
    }
}


bool BTM51X::answerCall(){
    Serial.println("Answering call");
    return requestRespond("AT+HFCA");
}

bool BTM51X::terminateCall(){
    Serial.println("Terminate call");
    return requestRespond("AT+HFCH");
}

BTM51X::SM_STATE BTM51X::readLine(){
    while (_ser->available())
    {
        switch(incomingDataStateMachine(_ser->read())){
        case EVENT:
            return EVENT;
        case PACKET:
            return PACKET;
        default:
            return NOPE;
        }
    }
    return NOPE;
}

BTM51X::SM_STATE BTM51X::incomingDataStateMachine(char c){
    static SM_STATE datagram=NOPE;
    static PACKET_STATE state=IDLE;
    Serial.print(c,DEC);
    Serial.print(" (");
    if(c!='\n' && c!='\r'){
        Serial.print(c);
    }
    Serial.print(") ");
    switch(state){
    case IDLE:
        if(c==13){
            Serial.println("Going to STARTHDR");
            state = STARTHDR;
            return NOPE;
        }
        state = IDLE;
        return NOPE;
    case STARTHDR:
        _strRespBufferBT="";
        if(c==13){
            //Packet receiving
            datagram=PACKET;
            Serial.println("Going to STARTHDR2");
            state = STARTHDR2;
            return NOPE;
        }
        if(c==10){
            //Event receiving
            datagram=EVENT;
            Serial.println("Going to PACKET_STARTED");
            state = PACKET_STARTED;
            return NOPE;
        }
        state = IDLE;
        return NOPE;
    case STARTHDR2:
        if(c==10){
            Serial.println("Going to PACKET_STARTED");
            state = PACKET_STARTED;
            return NOPE;
        }
        Serial.println("Unexpected character received, going back to idle.");
        state = IDLE;
        return NOPE;
    case PACKET_STARTED:
        switch(c){
        case 13:
            if(_strRespBufferBT.length()>0){
                if(datagram==PACKET){
                    Serial.println("Going to FOOTER");
                    _strRespBufferBT+='\n';
                    state = FOOTER;
                }
                if(datagram==EVENT){
                    Serial.println("Going to FOOTER2");
                    state = FOOTER2;
                }
            }else{
                Serial.println("Buffer empty.  Going back to starthdr");
                state = STARTHDR;
            }
            break;
        case 10:
            Serial.println("Corrupt data. Going back to idle");
            state=IDLE;
            break;
        default:
            _strRespBufferBT+=c;
            state = PACKET_STARTED;
        }
        return NOPE;
    case FOOTER:
        if(c==10){
            Serial.println("Going to FOOTER2");
            state = FOOTER2;
            return NOPE;
        }
        Serial.println("Going from FOOTER to PACKET_STARTED");
        _strRespBufferBT+=c;
        state = PACKET_STARTED;
        return NOPE;
    case FOOTER2:
        if(c==10){
            switch (datagram) {
            case EVENT:
                Serial.println("Event received");
                break;
            case PACKET:
                Serial.println("Packet received");
                break;
            default:
                Serial.println("nope received");
                break;
            }
            //        Serial.println(_strCmdBufferBT);
            //        for(int i=0;i<_strCmdBufferBT.length();i++){
            //            Serial.print(_strCmdBufferBT.charAt(i), DEC);
            //            Serial.print(" ");
            //        }
            state = IDLE;
            return datagram;
        }
        Serial.println("Going from FOOTER2 to PACKET_STARTED");
        _strRespBufferBT+=c;
        state = PACKET_STARTED;
        return NOPE;
    }//switch
    return NOPE;
}


bool BTM51X::phoneConnectionState(HFP_STATE& hfp){
    if (!requestRespond("ATI65")){
        return false;
    }
    switch(_strRespBufferBT.charAt(0)){
    case '0':
        hfp=NOT_CONNECTED;
        return true;
    case '1':
        hfp=SLC_CONNECTED;
        return true;
    case '2':
        hfp=AUDIO_CONNECTED;
        return true;
    case '3':
        hfp=IN_CALL_SLC_CONNECTED;
        return true;
    case '4':
        hfp=IN_CALL_AUDIO_CONNECTED;
        return true;
    case '5':
        hfp=CALL_SETUP_SLC_CONNECTED;
        return true;
    case '6':
        hfp=CALL_SETUP_AUDIO_CONNECTED;
        return true;
    default:
        return false;
    }
}

bool BTM51X::getEvents(EVENTS& event){
    event=NO_EVENT;
    if(!readLine()){
        //no event received
        return false;
    }
    return parseEvents(event);
}

bool BTM51X::parseEvents(EVENTS& event){
    Serial.print("Packet: ");
    Serial.println(_strRespBufferBT);

    //Process pairing status
    if(_strRespBufferBT.startsWith("PAIR "))
    {
        event=PAIRING_EVENT;
        //Evaluate pairing status: see manual Table 3.7: Asynchronous messages for SSP
        _strRespBufferBT=_strRespBufferBT.substring(5);
        Serial.print("Pairing message: ");
        switch(_strRespBufferBT.substring(0,0).toInt()){
        case 0:
            Serial.println("Pairing OK.");
            _pairState=SUCCEEDED;
            break;
        default:
            Serial.println("Pairing failed." + _strRespBufferBT.substring(0,0));
            _pairState=FAILED;
            break;
        }
        return true;
    }
    if(_strRespBufferBT.startsWith("CONNECT ")){
        event=SLC_EVENT;
        _slcState=INITIALIZED;
        return true;
    }
    if(_strRespBufferBT.startsWith("HF\"+BSIR:")){
        //"Connect" message is not always shown when initializing SLC, so trigger event on receiving BSIR
        event=SLC_EVENT;
        _slcState=INITIALIZED;
        return true;
    }

    if(_strRespBufferBT.startsWith("NO CARRIER ")){
        event=SLC_EVENT;
        _slcState=RELEASED;
        return true;
    }
    if(_strRespBufferBT.startsWith("HFI\"")){
        int iValue=0;
        int iPos=_strRespBufferBT.lastIndexOf('"');
        String strEventType=_strRespBufferBT.substring(4,iPos);
        iPos=_strRespBufferBT.lastIndexOf(',');
        String strValue=_strRespBufferBT.substring(iPos+1);
        if(strValue.length()>0){
            iValue=strValue.toInt();
        }
        if(strEventType.equalsIgnoreCase("callsetup")){
            _callSetupState=iValue;
            event=CALLSETUP_EVENT;
            return true;
        }
        if(strEventType.equalsIgnoreCase("call")){
            _callState=iValue;
            event=CALL_EVENT;
            return true;
        }
    }
    event=UNKNOWN_EVENT;
    return true;
}

byte BTM51X::getCallState(){
    return _callState;
}


BTM51X::SLC_STATE BTM51X::getSlcState(){
    return _slcState;
}

byte BTM51X::getCallSetupState(){
    //HFP Spec. 1.5
    //    <value>=0 means not currently in call set up.
    //    <value>=1 means an incoming call process ongoing.
    //    <value>=2 means an outgoing call set up is ongoing.
    //    <value>=3 means remote party being alerted in an outgoing call.
    return _callSetupState;
}

bool BTM51X::requestRespond(const String& inBuffer){
    send(inBuffer);
    return readPacket();
}

bool BTM51X::call(const String& sNumber){
    //Call number
    if(requestRespond("AT+HFC\""+sNumber+"\"")){
        if(_strRespBufferBT.startsWith("OK")){
            Serial.println("true");
            return true;
        }
        for(byte i=0;i<_strRespBufferBT.length();i++){
            Serial.print(_strRespBufferBT.charAt(i), DEC);
            Serial.print(" ");
            Serial.println("false");
        }
    }
    return false;
}

void BTM51X::send(const String& buffer){
    EVENTS event=NO_EVENT;
    //wait until the module is no longer sending data;
    while(getEvents(event));
    _ser->println(buffer);
}

bool BTM51X::readPacket(){
    EVENTS event1;
    TTimer t(3000);
    t.start();
    Serial.println("Waiting for packet");
    while(!t.runout()){
        switch(readLine())
        {
        case PACKET:
            Serial.println("Packet found");
            return true;
        case EVENT:
            parseEvents(event1);
            break;
        default:
            break;
        }
    }
    Serial.println("No packet found");
    return false;
}

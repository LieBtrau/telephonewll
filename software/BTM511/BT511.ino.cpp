/* References:
 *Ericofon BT (Bluetooth) by Santiago Quijano (copyleft) 2009.
 */

#include "Infotone.h"
#include "btm51x.h"
#include "SLIC.h"

// Phone states
enum {
    OFFLINE,        //no phone connected to Bluetooth module
    IDLE,           //phone is idle (hanging = on-hook)
    SETTING_UP_CALL,//process of setting up the call and waiting for caller to pick up.
    TALKING,        //talking
    RINGING,        //ringing (incoming call)
    BUSY_TONE,      //generating busy tone for caller
    RINGING_TONE    //generating ringing tone for caller
} state;

const byte pinFR=2;//forward|reverse loop voltage switcher
const byte pinRM=3;//enable voltage source for ringing generator
const byte pinSHK=4;//high=horn off hook, low=horn on hook
const byte pinPD=8;//power down
const byte pinDialTone=11;//pin which will generate dial tone

Slic slic(pinFR,pinRM,pinSHK,pinPD, pinDialTone);
InfoTone dRingingTone(1000, 3000, pinDialTone);
InfoTone dBusyTone(500, 500, pinDialTone);
String sNumber;
BTM51X bt(Serial1);//BTM511 module connected to serial1 on Arduino Due
String readline="";

bool readLine(String& s){
    char c;
    while(Serial.available()){
        c=Serial.read();
        switch (c){
        case '\n':
            return false;
        case '\r':
            return true;
        default:
            s+=c;
            return false;
        }
    }
}

void setup()  
{
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }
    Serial.println("Hello, ready now!");
    slic.init();
    slic.powerUp();
    // Open serial communications and wait for port to open:
    bt.begin();
    delay(1000);
    BTM51X::HFP_STATE hfp;
    if(bt.phoneConnectionState(hfp))
    {
        Serial.println(hfp,DEC);
    }
}

//Arduino simply feeds through data to and from BTM511.
//Don't use Serial Monitor in Arduino IDE, because that one doesn't send the CR, LF.
//Alternative: gtkterm in Linux.
void loop()
{
    BTM51X::EVENTS event;
    bt.getEvents(event);
    slic.update();

    if(readLine(readline)){
        bt.send(readline);
        readline="";
    }
    //Leave this state when phone is disconnected
    if(event==BTM51X::SLC_EVENT && bt.getSlcState()==BTM51X::RELEASED){
        Serial.println("Going to OFFLINE");
        slic.powerDown();
        state=OFFLINE;
    }

    switch(state){
    case OFFLINE:
        bt.setLedState(BTM51X::LED_BLINKING);
        if(event==BTM51X::SLC_EVENT &&  bt.getSlcState()==BTM51X::INITIALIZED){
            Serial.println("Going from OFFLINE to IDLE");
            state=IDLE;
            slic.powerUp();
        }
        break;
    case IDLE:
        bt.setLedState(BTM51X::LED_PULSING);
        if(slic.HornPickedUpEdge() || slic.HornUp()){
            //If a number has been dialed, then show it.
            Serial.println("Getting dialed number.");
            if(slic.getDialedNumber(sNumber)){
                Serial.println("Calling " + sNumber);
                bt.call(sNumber);
                state=SETTING_UP_CALL;
            }
        }
        //Leave this state when there's an incoming call
        if(event==BTM51X::CALLSETUP_EVENT && bt.getCallSetupState()==1){
            //Makes this phone ring to advert the user that there's an incoming call.
            slic.startRinging();
            Serial.println("Incoming call...Going to ring state");
            state=RINGING;
        }
        break;
    case SETTING_UP_CALL:
        bt.setLedState(BTM51X::LED_PULSING);
        //Leave this state when user puts down the horn onto the hook.
        if(slic.HornPutDownEdge() || !slic.HornUp()){
            Serial.println("Going back to idle");
            bt.terminateCall();
            state=IDLE;
        }
        //Leave this state when caller is being adverted about the incoming call.
        if(event==BTM51X::CALLSETUP_EVENT && bt.getCallSetupState()==3){
            Serial.println("remote party being alerted in an outgoing call");
            state=RINGING_TONE;
        }
        //Leave this state when caller wants to stop calling
        if(event==BTM51X::CALLSETUP_EVENT && bt.getCallSetupState()==0){
            Serial.println("local party wants to stop calling");
            state=IDLE;
        }
        break;
    case RINGING_TONE:
        bt.setLedState(BTM51X::LED_PULSING);
        //Phone on the other end of the line is ringing.
        dRingingTone.run();
        //Leave this state when user puts down the horn onto the hook.
        if(slic.HornPutDownEdge() || !slic.HornUp()){
            dRingingTone.stop();
            bt.terminateCall();
            Serial.println("Going back to idle");
            state=IDLE;
        }
        //Leave this state when caller wants to stop calling
        if(event==BTM51X::CALLSETUP_EVENT && bt.getCallSetupState()==0){
            Serial.println("local party wants to stop calling");
            state=IDLE;
        }
        //Leave this state when the remote party has answered the call.
        if(event==BTM51X::CALL_EVENT && bt.getCallState()==1){
            Serial.println("Remote party has answered the call.");
            state=TALKING;
        }
        break;
    case BUSY_TONE:
        bt.setLedState(BTM51X::LED_PULSING);
        //the called party is busy
        dBusyTone.run();
        //Leave this state when user puts down the horn onto the hook.
        if(slic.HornPutDownEdge() || !slic.HornUp()){
            dBusyTone.stop();
            bt.terminateCall();
            Serial.println("Going back to idle");
            state=IDLE;
        }
        //Leave this state when caller wants to stop calling
        if(event==BTM51X::CALLSETUP_EVENT && bt.getCallSetupState()==0){
            Serial.println("local party wants to stop calling");
            state=IDLE;
        }
        break;
    case TALKING:
        bt.setLedState(BTM51X::LED_ON);
        //Leave this state when user puts down the horn onto the hook.
        if(slic.HornPutDownEdge() || !slic.HornUp()){
            Serial.println("Going back to idle");
            bt.terminateCall();
            state=IDLE;
        }
        //Leave this state when remote party has dropped the call.
        if(event==BTM51X::CALL_EVENT && bt.getCallState()==0){
            Serial.println("Remote party has dropped the call.");
            state=IDLE;
        }
        break;
    case RINGING:
        bt.setLedState(BTM51X::LED_PULSING);
        //Leave this state when the user picks up the hook to answer the call.
        if(slic.HornPickedUpEdge() || slic.HornUp()){
            bt.answerCall();
            slic.stopRinging();
            Serial.println("You're answering an incoming call now.");
            state=TALKING;
        }
        //Leave this state when the incoming call has stopped.
        if(event==BTM51X::CALLSETUP_EVENT && bt.getCallSetupState()==0){
            Serial.println("incoming call stopped");
            slic.stopRinging();
            state=IDLE;
        }
        break;
    }

}


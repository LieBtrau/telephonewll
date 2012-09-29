//Initialisation routines for SLIC
//Code based on code from OpenUsbFxs: http://code.google.com/p/openusbfxs/source/browse/trunk/LinuxDriver/openusbfxs/openusbfxs.c

#include <SPI.h>
#include "slic.h"
#include "RotaryDialer.h"

boolean inDirectAccessFree();
boolean writeVerifyIndirectReg(INDIRECT_REGISTERS yAddress, word wValue);
byte readSPI(DIRECT_REGISTERS yAddress);
void writeSPI(DIRECT_REGISTERS yAddress, byte yData);
boolean writeVerifySPI(DIRECT_REGISTERS yAddress, byte yData);
boolean readIndirectReg(INDIRECT_REGISTERS yAddress, word& wValue);
boolean writeIndirectReg(INDIRECT_REGISTERS yAddress, word wValue);
boolean writeVerifyIndirectReg(INDIRECT_REGISTERS yAddress, word wValue);

const byte VBATL=40;
extern RotaryDialer rd;

ISR(INT0_vect)
{
  byte yVal=readSPI(INT_STAT_2);
  if(yVal & 0x2){                                            //Check if Loop closure-opening happened
    writeSPI(INT_STAT_2,2);                                  //Clear the pending interrupt in the SLIC
    
    yVal=readSPI(LOOP_COND_IND);                             //LCR-bit (bit0) will be one when loop closure has been detected.
    rd.updatePulseState(yVal & 0x01);
  }
  writeSPI(INT_STAT_2,0xFF);                                //Clear all pending interrupts for interrupt register 2
  writeSPI(INT_STAT_3,0x01);                                //Clear pending DTMF interrupts
}

boolean setupVoiceChannel(){
  if(!writeVerifySPI(PCM_TX_STARTL,0x02))return false;      //delay TXD by two PCLKs
}

boolean enableDiallingTone(){
  writeSPI(LINE_FEED_CTRL,2);
  if(!writeVerifySPI(PCM_MODE_SEL,0x28))return false;
  if(!writeVerifyIndirectReg(OSC1,30959))return false;      //OSC1=425Hz (Belgacom spec)
  if(!writeVerifyIndirectReg(OSC1X,171))return false;       //171 = 1243 * amplitude[V] (for 425Hz) = 1243 * 0.138 [= -15dBm on 600ohm)
  if(!writeVerifyIndirectReg(OSC1Y,0))return false;
  writeSPI(OSC1CTRL,0x06);                                  //Assign dial tone to RX path (will be audible on phone)
}

boolean offHook(){
  byte yVal=readSPI(LOOP_COND_IND);
  if(yVal!=4){
    Serial.println(yVal,HEX);
  }
  if(yVal & 0x01){
    return true;
  }else{
    return false;
  }
}

void ringing(boolean bOn){
  if(bOn){
    writeSPI(LINE_FEED_CTRL,4);
  }else{
    writeSPI(LINE_FEED_CTRL,1);
  }
}//ringing

boolean slicInit(unsigned long timeout_s){
  byte val;
  unsigned long startTime;
  
  //Test SPI-communication with SLIC
  Serial.println("Testing SPI communication");
  if(readSPI(AUDIO_LOOPBACK)!=0x02){
    return false;
  }
  if(readSPI(TRANSHYBRID_BALANCE)!=0x33){
    return false;
  }
  if(readSPI(LINE_FEED_CTRL)!=0x00){
    return false;
  }
  
  //Initialize indirect registers
  Serial.println("Initializing indirect registers");
  if(!writeVerifyIndirectReg(DTMF_ROW_0_PEAK,0x55C2))return false; 
  if(!writeVerifyIndirectReg(DTMF_ROW_1_PEAK,0x51E6))return false; 
  if(!writeVerifyIndirectReg(DTMF_ROW_2_PEAK,0x4B85))return false; 
  if(!writeVerifyIndirectReg(DTMF_ROW_3_PEAK,0x4937))return false;
  if(!writeVerifyIndirectReg(DTMF_COL_1_PEAK,0x3333))return false;
  if(!writeVerifyIndirectReg(DTMF_FWD_TWIST,0x0202))return false;
  if(!writeVerifyIndirectReg(DTMF_RVS_TWIST,0x0202))return false;
  if(!writeVerifyIndirectReg(DTMF_ROW_RATIO_TRES,0x0198))return false;
  if(!writeVerifyIndirectReg(DTMF_COL_RATIO_TRES,0x0198))return false;
  if(!writeVerifyIndirectReg(DTMF_ROW_2ND_ARM,0x0611))return false;
  if(!writeVerifyIndirectReg(DTMF_COL_2ND_ARM,0x0202))return false;
  if(!writeVerifyIndirectReg(DTMF_PWR_MIN_TRES,0x00E5))return false;
  if(!writeVerifyIndirectReg(DTMF_OT_LIM_TRES,0x0A1C))return false;
  if(!writeVerifyIndirectReg(OSC1,0x7B30))return false;
  if(!writeVerifyIndirectReg(OSC1X,0x0063))return false;
  if(!writeVerifyIndirectReg(OSC1Y,0x0000))return false;
  if(!writeVerifyIndirectReg(OSC2_COEF,0x7870))return false;
  if(!writeVerifyIndirectReg(OSC2X,0x007D))return false;
  if(!writeVerifyIndirectReg(OSC2Y,0x0000))return false;
  if(!writeVerifyIndirectReg(RING_V_OFF,0x0000))return false;
  if(!writeVerifyIndirectReg(RING_OSC,0x7E6C))return false;                //25Hz ringing frequency (Belgacom spec.)
  if(!writeVerifyIndirectReg(RNGX,0x160))return false;                     //RNGX = 645 * Vpk / 96 -> Vpk= 52Vp=37Vrms (Belgacom spec. 38Vrms to 90Vrms)
  if(!writeVerifyIndirectReg(RING_Y,0x0000))return false;
  if(!writeVerifyIndirectReg(PULSE_ENVEL,0x2000))return false;
  if(!writeVerifyIndirectReg(PULSE_X,0x2000))return false;
  if(!writeVerifyIndirectReg(PULSE_Y,0x0000))return false;
  if(!writeVerifyIndirectReg(RECV_DIGITAL_GAIN,0x4000))return false;       //(playback volume set lower: 0x2000)
  if(!writeVerifyIndirectReg(XMIT_DIGITAL_GAIN,0x4000))return false;
  if(!writeVerifyIndirectReg(LOOP_CLOSE_TRES,0x1000))return false;
  if(!writeVerifyIndirectReg(RPTP,0x3600))return false;                    //Ring trip threshold = 34mA (see datasheet)
  if(!writeVerifyIndirectReg(COMMON_MIN_TRES,0x1000))return false;
  if(!writeVerifyIndirectReg(COMMON_MAX_TRES,0x0200))return false;         //AN35: 0x0080 
  if(!writeVerifyIndirectReg(PWR_ALARM_Q1Q2,0x0FF0))return false;          //-> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0FF0
  if(!writeVerifyIndirectReg(PWR_ALARM_Q3Q4,0x7F80))return false;          //-> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x7F80
  if(!writeVerifyIndirectReg(PWR_ALARM_Q5Q6,0xFF0))return false;           //-> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0FF0
  
  // AN35 advises to set IRs 35--39 to 0x8000 now and then set them to their desired values much later
  if(!writeVerifyIndirectReg(LOOP_CLOSURE_FILTER,0x8000))return false;
  if(!writeVerifyIndirectReg(NRTP,0x8000))return false;         //0x0320
  if(!writeVerifyIndirectReg(TERM_LP_POLE_Q1Q2,0x8000))return false;        //0x008C -> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0010
  if(!writeVerifyIndirectReg(TERM_LP_POLE_Q3Q4,0x8000))return false;        //0x0100 -> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0010
  if(!writeVerifyIndirectReg(TERM_LP_POLE_Q5Q6,0x8000))return false;        //0x0010 -> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0010
  
  if(!writeVerifyIndirectReg(CM_BIAS_RINGING,0))return false;               //Vcm during ringing (Excel DC-DC converter parameter calculation)
  if(!writeVerifyIndirectReg(DCDC_MIN_V,0x0C00))return false;
  if(!writeVerifyIndirectReg(LOOP_CLOSE_TRES_LOW,0x1000))return false;

  //Setting DC-parameters
  Serial.println("Setting DC-parameters");
  if(!writeVerifySPI(AUDIO_LOOPBACK,0x00))return false;                //take SLIC out of "digital loopback" mode
  if(!writeVerifySPI(ENHANCE_ENABLE,0xEB))return false;                //turn on Rev E. features
  if(!writeVerifySPI(BATT_FEED_CTRL,0x01))return false;                //keep Vov low, let Vbat track Vring
  if(!writeVerifySPI(DCDC_PWM_PERIOD,202))return false;                //DC-DC converter PWM period=12.33us ~ 81,109kHz (Excel DC-DC converter parameter calculation)
  if(!writeVerifySPI(DCDC_SW_DELAY,12))return false;                   //DC-DC converter min off time=732ns  (Excel DC-DC converter parameter calculation)
  if(!writeVerifySPI(HI_BATT_VOLT,44))return false;                    //high battery voltage = 66V  (Excel DC-DC converter parameter calculation)

  if(!writeVerifySPI(LO_BATT_VOLT,VBATL))return false;                 //low battery voltage = 60V (need to write this?  tracking is used)
  if(!writeVerifySPI(LOOP_CURR_LIM,0))return false;                    //max allowed loop current = 20mA [default value]

  //power up DCDC-converter
  Serial.println("Powering up DC/DC-converter");
  startTime=millis();
  if(!writeVerifySPI(PWR_DWN_CRTL1,0x00))return false;                 //this should bring the DC-DC converter up
  do{
    val=readSPI(VBAT_SNS_1);                                           //Poll the battery voltage and wait until it reaches the desired value
    delay(10);
    if(millis()-startTime>1000)return false;
  }while((val>>2)<VBATL);                                              //battery voltage is sensed in 0.376V steps, while VBATL is set in 1.5V steps (= about 4*0.376)

  val=readSPI(DCDC_SW_DELAY);
  writeSPI(DCDC_SW_DELAY,val | 0X80);                                  //Perform DCDC-calibration
  startTime=millis();
  do{
    val=readSPI(DCDC_SW_DELAY);                                        
    delay(10);
    if(millis()-startTime>1000)return false;
  }while(val & 0x80);                                                  //Poll the DCCAL-bit

  //Calibrate SLIC
  Serial.println("Calibrating Slic");
  if(!writeVerifySPI(INT_EN_1,0x00))return false;                      //disable interrupts
  if(!writeVerifySPI(INT_EN_2,0x00))return false;                      //disable interrupts
  if(!writeVerifySPI(INT_EN_3,0x00))return false;                      //disable interrupts
  if(!writeVerifySPI(LINE_FEED_CTRL,0))return false;                   //set linefeed to Open
  if(!writeVerifySPI(CAL_CTRL_SR2,0x1E))return false;                  //monitor ADC calibration 1&2, DAC & ADC, but don't do balance calibration
  if(!writeVerifySPI(CAL_CTRL_SR1,0x47))return false;                  //monitor differential DAC, common-mode DAC and I_LIM calibrations + start calibration
  startTime=millis();
  do{
    val=readSPI(CAL_CTRL_SR1);                                        
    delay(10);
    if(millis()-startTime>1000)return false;
  }while(val);

  // manual calibration (Q5 current)
  for (byte j = 0x1f; j > 0; j--) {
    if (!writeVerifySPI(RING_CAL_G,j))return false;
    delay(40);
    if (!readSPI(Q5_CUR_SENS)) break;
  }
  if(readSPI(Q5_CUR_SENS))return false;
  // manual calibration (Q6 current)
  for (byte j = 0x1f; j > 0; j--) {
    if (!writeVerifySPI(TIP_CAL_G,j))return false;
    delay(40);
    if (!readSPI(Q6_CUR_SENS))break;
  }
  if (readSPI(Q6_CUR_SENS))return false;
  
  //Wait for user to put the phone on hook
  Serial.println("Waiting for user to put phone on hook");
  startTime=millis();
  do{
    writeSPI(LINE_FEED_CTRL,1);                                           // set linefeed (DR64) to 0x01 (forward active mode)
    delay(1000);                                                          // give ciruitry time to settle
    val=readSPI(LOOP_COND_IND);                                           // Poll the loop condition
    if(!(val & 0x01))break;                                               // 0 when no loop closure (= off-hook) detected
    if(!writeVerifySPI(LINE_FEED_CTRL,0))return false;                    // set linefeed (DR64) back to 0x00 (open mode)
  }while((millis()-startTime) < 1000*timeout_s);
  if((val & 0x01))return false;    

  //Do longitudinal balance calibration
  Serial.println("Performing longitudinal balance calibration");
  if (!writeVerifySPI(CAL_CTRL_SR2,1))return false;
  if (!writeVerifySPI(CAL_CTRL_SR1,0x40))return false;
  startTime=millis();
  do{
    val=readSPI(CAL_CTRL_SR1);                                        
    delay(10);
    if(millis()-startTime>1000)return false;
  }while(val);
  
  //Miscellaneous initialisations
  Serial.println("Miscellaneous initialisations");
  //Flush energy accumulators (undocumented registers)
  for(byte i=88;i<=95;i++){
    if(!writeIndirectReg((INDIRECT_REGISTERS)i,0))return false;
  }
  if(!writeIndirectReg((INDIRECT_REGISTERS)97,0))return false;
  for(byte i=193;i<=211;i++){
    if(!writeIndirectReg((INDIRECT_REGISTERS)i,0))return false;
  }

  //Clear pending interrupts
  writeSPI(INT_STAT_1,0xFF);
  writeSPI(INT_STAT_2,0xFF);
  writeSPI(INT_STAT_3,0xFF);  
  //Enable interrupts
  if(!writeVerifySPI(INT_EN_1,0))return false;                                         //disable timer interrupts
  if(!writeVerifySPI(INT_EN_2,0xFF))return false;                                      //enable power alarm + loop closure transition (on<->off hook)
                                                                                       // + ring trip (off-hook during ringing)
  if(!writeVerifySPI(INT_EN_3,1))return false;                                         //enable DTMF-interrupts
  //set up PCM clock slot assignment
  if(!writeVerifySPI(PCM_TX_STARTL,0))return false; 
  if(!writeVerifySPI(PCM_TX_STARTH,0))return false; 
  if(!writeVerifySPI(PCM_RX_STARTL,0))return false; 
  if(!writeVerifySPI(PCM_RX_STARTH,0))return false;
  //Initialise voice path
  //Nothing to do?
  //Initialize OSC1, OSC2, Ringing, Pulse Metering
  if(!writeVerifySPI(RATH,0x1F))return false;                                          //Belgacom ringing cadence: ringing= 1s = 8000* 125µs = 0x1F40 * 125µs
  if(!writeVerifySPI(RATL,0x40))return false;
  if(!writeVerifySPI(RITH,0x5D))return false;                                          //Belgacom ringing cadence: pause = 3s = 24000* 125µs = 0x5DC0 * 125µs
  if(!writeVerifySPI(RITL,0xC0))return false; 
  if(!writeVerifySPI(ROC,0x18))return false;                                           //Enable ringing timers
  //Write detect thresholds and filters
  if(!writeVerifySPI(LOOP_CLOSURE_DEBOUNCE_AR, 0x54))return false;                     //loop closure debounce interval for ringing silent period: 105 ms
  if(!writeVerifySPI(AUTO_MAN_CTRL, 0x1f))return false;	                               //auto-manual control: all auto
  //Belgacom pulse dialing: 
  //timeout before first digit reception=30s
  if(!writeVerifySPI(LOOP_CLOSURE_DEBOUNCE, 0x0a))return false;                        //loop closure debounce interval): 12.5 ms
  if(!writeVerifySPI(RTDI, 0x0a))return false;                                         //ring trip debounce interval: 12.5 ms
  //write DC feed parameters
  if(!writeVerifySPI(EXT_TRS_CTRL,0x61))return false;                                  //external bipolar transistor control left to default 0x61
  // 66 (battery feed control) Vov/Track set during DC-DC converter powerup
  // 71 (loop current limit) left to default 0x00 (20mA)
  if(!writeVerifySPI(ON_HOOK_LINE_VOLT,0x20))return false;                             //on-hook line voltage left to default 0x20 (48V)
  if(!writeVerifySPI(CM_VOLT,0x02))return false;                                       //common-mode voltage left to default 0x02 (3V) [6V in Zaptel?] */
  //Setting indirect registers 35-39 to their final values.
  if(!writeVerifyIndirectReg(LOOP_CLOSURE_FILTER,0x8000))return false;
  if(!writeVerifyIndirectReg(NRTP,0x0350))return false;                     //Digital filter coeff. for ring-trip detection for 25Hz ringing
  if(!writeVerifyIndirectReg(TERM_LP_POLE_Q1Q2,0x0010))return false;        //0x008C -> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0010
  if(!writeVerifyIndirectReg(TERM_LP_POLE_Q3Q4,0x0010))return false;        //0x0100 -> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0010
  if(!writeVerifyIndirectReg(TERM_LP_POLE_Q5Q6,0x0010))return false;        //0x0010 -> as per AN47 (Si3201 chip -> use SOT89 settings), p.4: 0x0010
  if(!writeVerifySPI(PCM_MODE_SEL,0x28))return false;                       //Enable PCM in µ-law mode
  return true;
}

byte readSPI(DIRECT_REGISTERS yAddress){
  byte yVal=0;
  digitalWrite(PIN_nCS,LOW);
  SPI.transfer(0X80 | yAddress);
  yVal=SPI.transfer(0);
  digitalWrite(PIN_nCS,HIGH);
  return yVal;
}

void writeSPI(DIRECT_REGISTERS yAddress, byte yData){
  digitalWrite(PIN_nCS,LOW);
  SPI.transfer(0X7F & yAddress);
  SPI.transfer(yData);
  digitalWrite(PIN_nCS,HIGH);
}

boolean writeVerifySPI(DIRECT_REGISTERS yAddress, byte yData){
  byte yCheck;
  writeSPI(yAddress,yData);
  yCheck=readSPI(yAddress);
  if(yCheck!=yData){
    Serial.print("Write direct register failed: address: ");
    Serial.print(yAddress, DEC);
    Serial.print(" Expected value: 0x");
    Serial.print(yData, HEX);
    Serial.print(" Read value: 0x");
    Serial.println(yCheck, HEX);
    return false;
  }
  else{
    return true;
  }
}

boolean readIndirectReg(INDIRECT_REGISTERS yAddress, word& wValue){
  if(!inDirectAccessFree)return false;
  writeSPI(IND_ADDR, yAddress);
  if(!inDirectAccessFree)return false;
  wValue=readSPI(IND_DAT_ACC_H)<<8 | readSPI(IND_DAT_ACC_L);
  return true;
}

boolean writeIndirectReg(INDIRECT_REGISTERS yAddress, word wValue){
  if(!inDirectAccessFree)return false;
  writeSPI(IND_DAT_ACC_L,(byte)(wValue & 0xFF));
  writeSPI(IND_DAT_ACC_H,(byte)(wValue>>8));
  writeSPI(IND_ADDR, yAddress);
  return true;
}

boolean writeVerifyIndirectReg(INDIRECT_REGISTERS yAddress, word wValue){
  word wCheck;
  if(!writeIndirectReg(yAddress, wValue))return false;
  if(!readIndirectReg(yAddress, wCheck))return false;
  if(wCheck!=wValue){
    Serial.print("Write indirect register failed: address: ");
    Serial.print(yAddress, DEC);
    Serial.print("\tExpected value: 0x");
    Serial.print(wValue, HEX);
    Serial.print("\tRead value: 0x");
    Serial.println(wCheck, HEX);
    return false;
  }
  return true;
}

boolean inDirectAccessFree(){
  for(int i=0;i<1000;i++){
    if(readSPI(IND_ADDR_STATUS)==0){
      return true;
    }
  }
  Serial.println("Indirect access not free");
  return false;
}

#ifndef SLIC_H
#define SLIC_H

typedef enum{
  PCM_MODE_SEL=1,
  PCM_TX_STARTL=2,
  PCM_TX_STARTH=3,
  PCM_RX_STARTL=4,
  PCM_RX_STARTH=5,
  AUDIO_LOOPBACK=8,
  AUDIO_GAIN=9,
  TRANSHYBRID_BALANCE=11,
  PWR_DWN_CRTL1=14,
  INT_STAT_1=18,
  INT_STAT_2=19,
  INT_STAT_3=20,
  INT_EN_1=21,
  INT_EN_2=22,
  INT_EN_3=23,
  DTMF_DECODE=24,
  IND_DAT_ACC_L=28,
  IND_DAT_ACC_H=29,
  IND_ADDR=30,
  IND_ADDR_STATUS=31,
  OSC1CTRL=32,
  ROC=34,
  RATL=48,
  RATH=49,
  RITL=50,
  RITH=51,
  LOOP_CLOSURE_DEBOUNCE_AR=63,
  LINE_FEED_CTRL=64,
  EXT_TRS_CTRL=65,
  BATT_FEED_CTRL=66,
  AUTO_MAN_CTRL=67,
  LOOP_COND_IND=68,
  LOOP_CLOSURE_DEBOUNCE=69,
  RTDI=70,
  LOOP_CURR_LIM=71,
  ON_HOOK_LINE_VOLT=72,
  CM_VOLT=73,
  HI_BATT_VOLT=74,
  LO_BATT_VOLT=75,
  VBAT_SNS_1=82,
  Q5_CUR_SENS=88,
  Q6_CUR_SENS=89,
  DCDC_PWM_PERIOD=92,
  DCDC_SW_DELAY=93,
  CAL_CTRL_SR1=96,
  CAL_CTRL_SR2=97,
  RING_CAL_G=98,
  TIP_CAL_G=99,
  ENHANCE_ENABLE=108
} 
DIRECT_REGISTERS;

typedef enum{
  DTMF_ROW_0_PEAK=0,//0x55C2 
  DTMF_ROW_1_PEAK=1,//0x51E6 
  DTMF_ROW_2_PEAK=2,//0x4B85 
  DTMF_ROW_3_PEAK=3,//0x4937
  DTMF_COL_1_PEAK=4,//0x3333
  DTMF_FWD_TWIST=5,//0x0202
  DTMF_RVS_TWIST=6,//0x0202
  DTMF_ROW_RATIO_TRES=7,//0x0198
  DTMF_COL_RATIO_TRES=8,//0x0198
  DTMF_ROW_2ND_ARM=9,//0x0611
  DTMF_COL_2ND_ARM=10,//0x0202
  DTMF_PWR_MIN_TRES=11,//0x00E5
  DTMF_OT_LIM_TRES=12,//0x0A1C
  OSC1=13,//0x7B30
  OSC1X=14,//0x0063
  OSC1Y=15,//0x0000
  OSC2_COEF=16,//0x7870
  OSC2X=17,//0x007D
  OSC2Y=18,//0x0000
  RING_V_OFF=19,//0x0000
  RING_OSC=20,//0x7EF0
  RNGX=21,//0x0160
  RING_Y=22,//0x0000
  PULSE_ENVEL=23,//0x2000
  PULSE_X=24,//0x2000
  PULSE_Y=25,//0x0000
  RECV_DIGITAL_GAIN=26,//0x4000    (playback volume set lower: 0x2000)
  XMIT_DIGITAL_GAIN=27,//0x4000
  LOOP_CLOSE_TRES=28,//0x1000
  RPTP=29,//0x3600
  COMMON_MIN_TRES=30,//0x1000
  COMMON_MAX_TRES=31,//0x0200
  PWR_ALARM_Q1Q2=32,//0x07C0  -> as per AN47, p.4: 0x0FF0
  PWR_ALARM_Q3Q4=33,//0x2600  -> as per AN47, p.4: 0x7F80
  PWR_ALARM_Q5Q6=34,//0x1B80  -> as per AN47, p.4: 0x0FF0
  LOOP_CLOSURE_FILTER=35,//0x8000    // AN35 advises to set IRs 35--39 to 0x8000 now and then set them to their desired 
    //values much later
  NRTP=36,//0x0320
  TERM_LP_POLE_Q1Q2=37,//0x008C -> as per AN47, p.4: 0x0010
  TERM_LP_POLE_Q3Q4=38,//0x0100 -> as per AN47, p.4: 0x0010
  TERM_LP_POLE_Q5Q6=39,//0x0010 -> as per AN47, p.4: 0x0010
  CM_BIAS_RINGING=40,//0x0C00  set elsewhere to 0 RegIndex = 10000;
  DCDC_MIN_V=41,//0x0C00
  LOOP_CLOSE_TRES_LOW=43,// 0x1000
}
INDIRECT_REGISTERS;

//Pin definitions
const byte PIN_PCLK=3;
const byte PIN_RESET=5;
const byte PIN_DRX=7;
const byte PIN_DTX=8;
const byte PIN_FSYNC=9;
const byte PIN_nCS=10;
const byte PIN_MOSI=11;

boolean setupVoiceChannel(void);
boolean slicInit(unsigned long timeout_s);
void ringing(boolean bOn);
boolean offHook(void);
boolean enableDiallingTone(void);
#endif


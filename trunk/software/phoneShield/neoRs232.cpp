#include "crc16.h"

typedef enum
{
  LEADING_FLAG=0xAA,		/*!<Start flag*/
  DLE_FLAG=0x10,	        /*!<Bytestuffing flag*/
  TRAILING_FLAG=0xFF,		/*!<End flag*/
  ACK0_FLAG=0x30		/*!<Acknowledge flag*/
}
FLAGS;

//!The types of frames that can occur in the protocol
typedef enum
{
  INFO_FRAME=0,			/*!<Info frame (contains data)*/
  ACK_FRAME=1			/*!<Acknowledge frame*/
}
FRAME_TYPE;

//!States of the receiving state machine
typedef enum
{
  MSG_NO_INPUT,			/*!<No data has come in.*/
  MSG_DECODING,			/*!<State machine is decoding data.*/
  MSG_ACK_RECEIVED,             //!Ack received
  MSG_RX_OVERFLOW,              //!RX buffer overflowed.
  MSG_RX_COMPLETE	        /*!<Complete packet received.*/
}
MSG_T;

typedef enum
{
  WAITING,
  DECODING
}
STATE;

boolean flagCharacterInData(byte c);
void writeString(byte* buffer, byte length, FRAME_TYPE typeFrame);
MSG_T FramerReceive(FRAMER* pFr);
extern void processFrame(byte* buf, byte length);
static volatile boolean bAckReceived;

/*! Initialize the frame so that it is ready to receive data
 *\param pFr Pointer to the frame that will be initialized.
 */
void receiveInit(FRAMER* pFr, byte* frameData){
  pFr->pData=pFr->pBegin=frameData;
  pFr->pEnd=pFr->pBegin+FRAMESIZE-1;
}//receiveInit


/*!Receive run routine: checks if data has come in and act accordingly.
 *\param pFramer Check if data has come in on this framer
 */
void receiveRun(FRAMER* pFramer){
  switch(FramerReceive(pFramer))
  {
  case MSG_RX_COMPLETE:
    if(stripCRC(pFramer))
    {
      //Frame received correctly
      //todo
      //writeString(0,0,ACK_FRAME);
      processFrame(pFramer->pBegin,pFramer->pData-pFramer->pBegin);
    }
    break;
  case MSG_ACK_RECEIVED:
    bAckReceived=true;
    break;
  case MSG_DECODING:
    //I'm still busy
    break;
  case MSG_NO_INPUT:
    //They're probably sleeping on the other side of the line.
    break;
  case MSG_RX_OVERFLOW:
    //Hey! Take it easy, man!  I can't follow.
    break;
  }
}//receiveRun

void txFrameWithoutAck(byte* buffer, byte length, FRAMER* pFr)
{
  if(length+2>FRAMESIZE)return;//prevent buffer overflow
  appendCRC(buffer, length);
  writeString(buffer, length+2, INFO_FRAME);
}//txFrameWithoutAck


/*! Function that sends a frame.  First a CRC16 is appended.  Then the
 * frame is sent.  After that, a timeout timer is initialized.  The
 * acknowledge must come in before the timeout expires.
 *\param buffer Buffer containing the data to send.
 *\param length Number of bytes that must be sent.
 *\param pFr    Pointer to framer, to check for incoming ack.
 *\return       true when ACK received, else false.
 */
boolean txFrameWithAck(byte* buffer, byte length, FRAMER* pFr){
  if(length+2>FRAMESIZE)return false;//prevent buffer overflow
  appendCRC(buffer, length);
  writeString(buffer, length+2, INFO_FRAME);
  bAckReceived=false;
  unsigned long lStartTime=millis();
  do
  {
    receiveRun(pFr);
    if(bAckReceived)
    {
      return true;
    }
  }
  while(millis()<lStartTime+5000);
  return false;
}//txFrameWithAck


/*! Function that reads a received character from the Stream using a state machine.
 *  The databytes are read from the circular buffer, a check if performed 
 *  if it is a flagbyte.  If it is, then the correct action is undertaken.
 *  If it is a databyte then that databyte is added to the framebuffer.
 *  A value is returned that tells us what this function has done with the
 *  data (if there was any).  
 *  \param pFr Framer to which the data will be posted.
 *  \return state of the state machine.
 */
MSG_T FramerReceive(FRAMER* pFr){
  static boolean dle_received=false;
  static STATE state=WAITING;
  byte c=0;
  MSG_T ret = MSG_NO_INPUT;

  while(Serial.available()>0)
  {
    c=Serial.read();
    ret=MSG_DECODING;
    if(dle_received)
    {
      if(state==DECODING)
      {
        *pFr->pData=c;
        pFr->pData++;
      }
      dle_received=false;
    }
    else
    {
      switch(c)
      {
      case LEADING_FLAG:
        pFr->pData=pFr->pBegin;
        state=DECODING;
        break;
      case TRAILING_FLAG:
        state=WAITING;
        return MSG_RX_COMPLETE;
        break;
      case ACK0_FLAG:
        return MSG_ACK_RECEIVED;
        break;
      case DLE_FLAG:
        if(state==DECODING)
        {
          dle_received=true;
        }
        break;
      default:
        if(state==DECODING)
        {
          *(pFr->pData)=c;
          (pFr->pData)++;
        }
      }//switch
    }
    //RX-Buffer mustn't overflow
    if(pFr->pData>=pFr->pEnd)
    {
      pFr->pData=pFr->pBegin;
      return MSG_RX_OVERFLOW;
    }
  }
  return ret;
}//FramerReceive


/*! Write a databuffer to the serial port.  No CRC will be appended.
 * This function adds the flags for the protocol and performs the 
 * bytestuffing.  Finally it sends the data to the UART.
 *\param buffer Buffer containing the data.
 *\param length Length of the databuffer
 *\param typeFrame Type of frame (info frame or acknowledge frame)
 */
void writeString(byte* buffer, byte length, FRAME_TYPE typeFrame){
  byte i=0;
  switch(typeFrame){
  case INFO_FRAME:
    //Leading flag
    Serial.write(LEADING_FLAG);
    /*Data:	
     * if a databyte is equal to a flag character, then prepend
     * a DLE_FLAG to it.
     */
    for(i=0;i<length;i++)
    {		
      if(flagCharacterInData(buffer[i]))
      {
        Serial.write(DLE_FLAG);
      }
      Serial.write(buffer[i]);
      Serial.flush();
    }
    //Trailing flag
    Serial.write(TRAILING_FLAG);
    break;		        	
  case ACK_FRAME:
    Serial.write(ACK0_FLAG);
    break;		        	
  }

}//writeString


/*!Check if the character is a flag character.
 *\param c The character
 *\return TRUE when c is a flag character else FALSE.
 */
boolean flagCharacterInData(byte c){
  return (c==LEADING_FLAG||c==TRAILING_FLAG||c==DLE_FLAG||c==ACK0_FLAG ? true : false);
}//flagCharacterInData














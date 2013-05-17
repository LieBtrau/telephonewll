//!NeoRs232 protocol constants
//This protocol does error correcting.  When you're using IEEE802.15.4, then you can
//do without error correcting.  The most likely cause of errors is in the wireless link,
//not in the serial connection between the processor and the module.
//IEEE802.15.4 has built-in error control already.

#ifndef NEORS232_H
#define NEORS232_H

#include <Arduino.h>

/*! Struct that will contain the received frame.
 */
typedef struct{
  /*! Points to the first character of the received frame*/
  byte* pBegin;
  /*! Points to one position after the last received frame.
   *  So the number of received bytes = pData - pBegin
   */
  byte* pData;
  /*! Points to the last position of the buffer.
   *  This variable defines the maximum size of the buffer.
   */
  byte* pEnd;
}
FRAMER;

const byte FRAMESIZE=81;

void receiveInit(FRAMER* pFr, byte* frameData);
void receiveRun(FRAMER* pFramer);
boolean txFrameWithAck(byte* buffer, byte length, FRAMER* pFr);
void txFrameWithoutAck(byte* buffer, byte length, FRAMER* pFr);


#endif





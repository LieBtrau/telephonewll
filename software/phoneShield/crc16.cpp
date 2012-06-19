/*!\file CRC_Test.c
 *CRC16 bit implementation
 */

#include "neoRs232.h"
#include <avr/crc16.h>

word get_crc(byte* buf, byte length);
word _crc_ccitt_update(word crc, byte data);

/*!check and strip the CRC16 of the frame.
 *\param pFramer A frame containing the received data.
 *\return TRUE when correct CRC, else return FALSE, when the CRC is correct,
 *	then the CRC will be removed from the frame.
 */
boolean stripCRC(FRAMER* pFramer){
  word crcRxed=0, crcCalculated;
  byte length;

  length= pFramer->pData - pFramer->pBegin - 2;
  crcCalculated=get_crc(pFramer->pBegin,length);

  //read CRC from received data (little endian volgorde)
  crcRxed=(*(pFramer->pData-1)<<8)|*(pFramer->pData-2);

  if(crcRxed!=crcCalculated)
  {
    return false;
  }
  //Remove the CRC
  pFramer->pData-=2;
  return true;
}//stripCRC


/*! Calculate a CRC16 of a buffer and append it to that buffer.
 *\param buffer databuffer of which CRC16 will be calculated and to which
 *		CRC16 will be appended.
 *\param length number of bytes in the buffer
 *\return the same buffer, with CRC-appended (little endian)
 */
void appendCRC(byte* buffer, int length){
  word crc=get_crc(buffer,length);
  buffer[length]=crc & 0xFF;
  buffer[length+1]=crc >> 8;
}//appendCRC


/*!Get the CRC16 of a databuffer.
 *\param buf containing data of which the CRC will be calculated
 *\param length total length of the buffer
 *\return CRC16
 */
word get_crc(byte* buf, byte length){
  word crc=0xFFFF;
  for(byte i=0;i<length;i++)
  {
    crc=_crc_ccitt_update(crc,buf[i]);
  }
  return crc;
}//get_crc

//http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
//word _crc_ccitt_update(word crc, byte data)
//{
//  data ^= (byte)(crc & 0xFF);
//  data ^= (byte)(data << 4);
//  return (word)(((((word)data << 8) | ((crc>>8) & 0xFF)) ^ (byte)(data >> 4) ^ ((word)data << 3)));
//}




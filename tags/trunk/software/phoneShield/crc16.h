#ifndef CRC16_H
#define CRC16_H

#include <Arduino.h>
#include "neoRs232.h"

void appendCRC(byte* buffer, int length);
boolean stripCRC(FRAMER* pFramer);

#endif

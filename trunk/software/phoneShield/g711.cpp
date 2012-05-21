/*The following code does the de-/companding of PCM-data.
 *
 *It's a pity that the PCM-transfer takes so much resources away from the MCU.
 *PCM data transfers on that bus must need to be kept to a minimum.  So 8-bit transfer (G.711 companded data) and
 *not the uncompanded 16-bit data format is used.
 *Using companded data has the disadvantage that it must be decompanded before other algorithms can process the data.
 *However in this MCU, the decompanding/companding calculation is still much more efficient than using 16-bit
 *communication.
 *
 *Performance can be tested by converting wave files to A-law and sending them to the telephone:
 *  sox TalkTherm.wav -e a-law -r 8000 TalkTherm.la
 *  play TalkTherm.la -r 8000
 *
 *The following code has been taken from: 
 *https://github.com/jart/freeswitch/blob/master/libs/spandsp/src/spandsp/g711.h
 */

#include <Arduino.h>               //contains Arduino constants (Arduino 1.0)

/*! The A-law alternate mark inversion mask */
#define ALAW_AMI_MASK       0x55

//Find the bit position of the highest set bit in a word. 
//http://spandsp.sourcearchive.com/documentation/0.0.6~pre12/bit__operations_8h_c07a2098a9bc756b4fb41de0505849a7.html
static __inline__ int top_bit(unsigned int bits){
  if (bits == 0)
  {
    return -1;
  }
  int res = 0;
  if (bits & 0xFF00)
  {
    bits &= 0xFF00;
    res += 8;
  }
  if (bits & 0xF0F0)
  {
    bits &= 0xF0F0;
    res += 4;
  }
  if (bits & 0xCCCC)
  {
    bits &= 0xCCCC;
    res += 2;
  }
  if (bits & 0xAAAA)
  {
    bits &= 0xAAAA;
    res += 1;
  }
  return res;
}

/*! \brief Encode a linear sample to A-law
 \param linear The sample to encode.
 \return The A-law value.
 */
byte aLawEncode(int linear)
{
  int mask;
  int seg;

  if (linear >= 0)
  {
    /* Sign (bit 7) bit = 1 */
    mask = ALAW_AMI_MASK | 0x80;
  }
  else
  {
    /* Sign (bit 7) bit = 0 */
    mask = ALAW_AMI_MASK;
    linear = -linear - 1;
  }

  /* Convert the scaled magnitude to segment number. */
  seg = top_bit(linear | 0xFF) - 7;
  if (seg >= 8)
  {
    if (linear >= 0)
    {
      /* Out of range. Return maximum value. */
      return (uint8_t) (0x7F ^ mask);
    }
    /* We must be just a tiny step below zero */
    return (uint8_t) (0x00 ^ mask);
  }
  /* Combine the sign, segment, and quantization bits. */
  return (uint8_t) (((seg << 4) | ((linear >> ((seg)  ?  (seg + 3)  :  4)) & 0x0F)) ^ mask);
}
/*- End of function --------------------------------------------------------*/


/*! \brief Decode an A-law sample to a linear value.
 \param alaw The A-law sample to decode.
 \return The linear value.
 */
int16_t aLawDecode(byte alaw)
{
  int i;
  int seg;

  alaw ^= ALAW_AMI_MASK;
  i = ((alaw & 0x0F) << 4);
  seg = (((int) alaw & 0x70) >> 4);
  if (seg)
    i = (i + 0x108) << (seg - 1);
  else
    i += 8;
  return (int16_t) ((alaw & 0x80)  ?  i  :  -i);
}
/*- End of function --------------------------------------------------------*/



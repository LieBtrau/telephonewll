/*
 * SpanDSP - a series of DSP components for telephony
 *
 * oki_adpcm.c - Conversion routines between linear 16 bit PCM data and
 *               OKI (Dialogic) ADPCM format. Supports with the 32kbps
 *               and 24kbps variants used by Dialogic.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001, 2004 Steve Underwood
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * The actual OKI ADPCM encode and decode method is derived from freely
 * available code, whose exact origins seem uncertain.
 */

/*! \file */

#include "oki_adpcm.h"

/* Routines to convert 12 bit linear samples to the Oki ADPCM coding format,
   widely used in CTI, because Dialogic use it. */

/* OKI ADPCM step variation table */
static const int16_t step_size[49] =
{
       16,    17,    19,    21,    23,    25,    28,    31,
       34,    37,    41,    45,    50,    55,    60,    66,
       73,    80,    88,    97,   107,   118,   130,   143,
      157,   173,   190,   209,   230,   253,   279,   307,
      337,   371,   408,   449,   494,   544,   598,   658,
      724,   796,   876,   963,  1060,  1166,  1282,  1411,
     1552
};

static const int16_t step_adjustment[8] =
{
    -1, -1, -1, -1, 2, 4, 6, 8
};

//Convert 4-bit adpcm in lowest nibble to 12-bit linear value stored in bits 12-0.
int16_t oki_decode(oki_adpcm_state_t *s, uint8_t adpcm)
{
    int16_t d;
    int16_t ss;
    int16_t linear;

    /* Doing the next part as follows:
     *
     * x = adpcm & 0x07;
     * e = (step_size[s->step_index]*(x + x + 1)) >> 3;
     * 
     * Seems an obvious improvement on a modern machine, but remember
     * the truncation errors do not come out the same. It would
     * not, therefore, be an exact match for what this code is doing.
     *
     * Just what a Dialogic card does, I do not know!
     */

    ss = step_size[s->step_index];
    d = ss >> 3;
    if (adpcm & 0x01)
        d += (ss >> 2);
    /*endif*/
    if (adpcm & 0x02)
        d += (ss >> 1);
    /*endif*/
    if (adpcm & 0x04)
        d += ss;
    /*endif*/
    if (adpcm & 0x08)
        d = -d;
    /*endif*/
    linear = s->last + d;

    /* Saturate the values to +/- 2^11 (supposed to be 12 bits) */
    if (linear > 2047)
        linear = 2047;
    else if (linear < -2048)
        linear = -2048;
    /*endif*/

    s->last = linear;
    s->step_index += step_adjustment[adpcm & 0x07];
    if (s->step_index < 0)
        s->step_index = 0;
    else if (s->step_index > 48)
        s->step_index = 48;
    /*endif*/
    /* Note: the result here is a 12 bit value */
    return linear;
}
/*- End of function --------------------------------------------------------*/

//Convert 12-bit signed linear value (stored in bits 15-4) to 4-bit adpcm in lowest nibble
uint8_t oki_encode(oki_adpcm_state_t *s, int16_t linear)
{
    int16_t d;
    int16_t ss;
    uint8_t adpcm;

    ss = step_size[s->step_index];
    d = (linear >> 4) - s->last;
    adpcm = (uint8_t) 0x00;
    if (d < 0)
    {
        adpcm = (uint8_t) 0x08;
        d = -d;
    }
    /*endif*/
    if (d >= ss)
    {
        adpcm |= (uint8_t) 0x04;
        d -= ss;
    }
    /*endif*/
    if (d >= (ss >> 1))
    {
        adpcm |= (uint8_t) 0x02;
        d -= (ss >> 1);
    }
    /*endif*/
    if (d >= (ss >> 2))
        adpcm |= (uint8_t) 0x01;
    /*endif*/

    /* Use the decoder to set the estimate of the last sample. */
    /* It also will adjust the step_index for us. */
    s->last = oki_decode(s, adpcm);
    return adpcm;
}
/*- End of function --------------------------------------------------------*/

/*- End of file ------------------------------------------------------------*/

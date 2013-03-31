/*
 * SpanDSP - a series of DSP components for telephony
 *
 * private/oki_adpcm.h - Conversion routines between linear 16 bit PCM data
 *		                 and OKI (Dialogic) ADPCM format.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001 Steve Underwood
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
 * $Id: oki_adpcm.h,v 1.1 2008/11/30 10:17:31 steveu Exp $
 */

/*! \file */

#ifndef _SPANDSP_PRIVATE_OKI_ADPCM_H_
#define _SPANDSP_PRIVATE_OKI_ADPCM_H_

/*! \page okiadpcm_page OKI (Dialogic) ADPCM encoding and decoding
\section okiadpcm_page_sec_1 What does it do?
OKI ADPCM is widely used in the CTI industry because it is the principal format
supported by Dialogic. As the market leader, they tend to define "common
practice". It offers a good balance of simplicity and quality at rates of
24kbps or 32kbps. 32kbps is obtained by ADPCM compressing 8k samples/second linear
PCM. 24kbps is obtained by resampling to 6k samples/second and using the same ADPCM
compression algorithm on the slower samples.

The algorithms for this ADPCM codec can be found in "PC Telephony - The complete guide
to designing, building and programming systems using Dialogic and Related Hardware"
by Bob Edgar. pg 272-276. */
//#include <Arduino.h>               //contains Arduino constants (Arduino 1.0)
typedef short int16_t;
typedef unsigned char uint8_t;

/*!
    Oki (Dialogic) ADPCM conversion state descriptor. This defines the state of
    a single working instance of the Oki ADPCM converter. This is used for
    either linear to ADPCM or ADPCM to linear conversion.
*/
struct oki_adpcm_state_t
{
    /*! \brief The last state of the ADPCM algorithm. */
    int16_t last;
    /*! \brief Current index into the step size table. */
    int16_t step_index;
};

/*! Decode a buffer of Oki ADPCM data to linear PCM.
    \param s The Oki ADPCM context.
    \param adpcm an 8-bit OKI-ADPCM value
    \return The number of samples returned. */
int16_t oki_decode(oki_adpcm_state_t *s, uint8_t adpcm);
/*! Encode a buffer of linear PCM data to Oki ADPCM.
    \param s The Oki ADPCM context.
    \param linear the linear PCM value that must be encoded.
    \return The number of bytes of Oki ADPCM data produced. */
uint8_t oki_encode(oki_adpcm_state_t *s, int16_t linear);

#endif
/*- End of file ------------------------------------------------------------*/

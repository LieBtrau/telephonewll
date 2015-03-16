## CODEC ##
The voice information can be sent in an analog or a digital version to the communicating devices.  The choice is made to use digital transmission, because it's easier then to implement control codes in the data.  Embedding control codes in analog signals can be quite complicated (e.g. the analog television signal).
This decision has the drawback that an audio CODEC is needed to convert the analog audio signal to its digital version and vice versa.

There is also need of a compression algorithm.  If voice is digitized at 8kHz (8bit), this will give an audio stream of 64kbps.  When both the caller and callee are speaking this will be doubled.  So it's simply too much to send over a wireless channel.

## Modules ##
  * Jennic has its JN5319 that transfers speech over an IEEE802.15.4.  The sampling frequency and resolution are selectable.  A sampling rate of 8kHz at a 16bit resolution lead to a raw bit rate of 128kbps.  By use of ADPCM this is reduced by a factor four to 32kbps.  A WM8510 audio codec is used for that job.

## Chips ##
  * Si3050 includes a [ITU-T G.711](http://en.wikipedia.org/wiki/G.711) codec.
  * AD73311: not chosen.  It delivers 16-bit data.  There is not enough bandwidth to send 16bit data over the wireless channel at the desired sampling rate.
  * TP3054/TP3057 include a [ITU-T G.711](http://en.wikipedia.org/wiki/G.711) codec.
    * The companding algorithm works using µ-law for the TP3054 and A-law for the TP3057.
  * [WM8510](http://www.wolfsonmicro.com/products/codecs/WM8510/)

## Algorithms ##
  * [ITU-T Rec.G.191 Software tools for speech and audio coding standardization](http://www.itu.int/rec/T-REC-G.191/en)
  * [Several audio codecs on Wikipedia](http://en.wikipedia.org/wiki/Comparison_of_audio_codecs)

### ITU-T G.711 ###
  * PCM encoding of 8KHz signals with µ-Law or A-Law results in 64kbps stream.
  * Compressing 13-bits (A-Law) or 14-bits (µ-Law) data to 8-bit.
  * In telephony, this standard is used together with ITU-T G.726.
  * Source:
    * [SpanDSP](https://github.com/jart/freeswitch/blob/master/libs/spandsp/src/spandsp/g711.h)
    * [CodeProject](http://www.codeproject.com/Articles/14237/Using-the-G711-standard): This decoder stores the decoded bits on bits 15-3 of a 16-bit word.  So bits 2-0 will always be 0.
### ITU-T G.726 ###
  * [ITU-T G.726 on Wikipedia](http://en.wikipedia.org/wiki/G.726)
  * ADPCM speech codec
  * G.727 is a specialized version of the ITU-T G.726 protocol that is intended for packet-based systems using the Packetized Voice Protocol (PVP)
  * This codec can only work with plain PCM-samples, so ITU-T G.711 compressed samples will have to be decompressed first.

### IMA ADPCM ###
  * Fixed point calculation instead of floating points in ITU-T G.726
  * [ST-implementation](http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/APPLICATION_NOTE/CD00275263.pdf)
  * [Microchip Audio Libraries](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=2680&dDocName=en554168)
  * [Atmel AVR336: ADPCM decoder (simplified version of G.726)](http://www.atmel.com/Images/doc2572.pdf)
  * User implementations:
    * [Voice Compression using ADPCM algorithm](https://instruct1.cit.cornell.edu/courses/ee476/FinalProjects/s2006/wc227_rks33/wc227_rks33/index.htm)
    * [Automatic phone answering machine](http://ethesis.nitrkl.ac.in/2057/1/Thesis_final.pdf)
  * Supported by SoX
### Dialogic/OKI ADPCM ###
  * Industry standard in CTI industry
  * .VOX is standard extension for files on PC.
  * Supported by SoX
  * For communication over wireless networks, 16kbps full duplex is the maximum.  I saw this on my own tests as well as on: [Voice Over Sensor Networks](http://repository.upenn.edu/cgi/viewcontent.cgi?article=1002&context=mlab_papers)
    * Drop each second sample, use 4bit ADPCM
    * Take all samples but use 2bit ADPCM (requires changing the codec)
  * ADPCM is very sensitive to packet loss.  It has difficulties finding back the original amplitude of the signal.  Sometimes it gets back to some steady-state amplitude that is a factor of the original amplitude.
  * When using ADPCM, you should send the state of the codec with the sound data.
  * [Standard document](http://www.sal.ksu.edu/faculty/tim/vox/dialogic_adpcm.pdf)
  * [Source code from HackChina](http://www.hackchina.com/en/r/10118/vox-_-vox.c__html)
  * [Source code (not fully according to spec)](http://www.sal.ksu.edu/faculty/tim/vox/)
  * [Source Code from SpanDSP](https://github.com/jart/freeswitch/blob/master/libs/spandsp/src/oki_adpcm.c)
### ITU-T G.729a ###
  * [ITU-T G.729a(8kbps)](http://en.wikipedia.org/wiki/G.729) which is used most in VoIP.
  * Based on ACELP, very complex codec
### Speex ###
  * [Microchip Audio Libraries](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=2680&dDocName=en554168)
    * Narrowband (8KHz) and wideband (16KHz) sampling
    * Only distributed in library format, no source code for Speex codec available.
  * For this application, Speex should be implemented with: sud-mode 4 and 11 kbps data rate to provide the best tradeoff between speech quality and computational complexity.
  * [Voice Transmission over Wireless Sensor Networks](http://onlinepresent.org/proceedings/vol7_2012/25.pdf).
  * [Voice Over Zigbee](http://voz.sourceforge.net/)
  * [ST Vocoder demonstration using Speex](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/CD00204907.pdf)
    * 8KHz sampling (=narrow band), bitrate 8kbps (quality parameter = 4) -> 16:1 compression ratio
    * compexity parameter <= 2

### Codec2 ###
  * http://codec2.org/

### CVSD ###
  * Very simple algorithm, looks very attractive to implement
  * encodes each soundsample to 1 databit
  * Oversampling needed to improve sound quality.  Unluckily, the SLIC sampling frequency is fixed to 7.8KHz.  So CVSD can't be used.
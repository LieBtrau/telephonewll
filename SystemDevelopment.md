# System Development #

  * [PhonePart](PhonePart.md)
  * [WirelessImplementation](WirelessImplementation.md)

## Base Station part ##
### Hardware ###
Search on the internet for: [DAA-module](http://lmgtfy.com/?q=DAA+Module).
The components that have been investigated can be found below:
  * AS2523:
    * some problems to be expected with galvanic isolation to telephone network.
    * not easily interfaceable by MCU.
  * AS2533: not chosen, has serial interface and hands free features, but no DTMF-signalling
  * U3716MB-T, U32001, U3900BM, TEA7092, TEA7540, TEA5400, TEA1098: some of these are interesting, but none of them is available.
  * Dry or wet transformer for connecting to telephone line?
    * Dry transformer can't handle DC-voltages, DC-blocking capacitors needed.
    * Wet transformers can handle DC-voltages without saturating.  They are bigger and waste that DC-current off course.
  * AS2540:
    * Created especially for remote control by MCU.
    * Automatic ring detection to MCU
    * SOIC28: easily solderable.
    * keyboard scanning interface
    * no access to digitized voice, no PCM-interface
  * IA3223 + IA3222
    * 56K-modem IC's.
    * Interesting for data applications, not really for voice application.
  * Si3056 + Si3018/19/10
    * 56K-modem IC's.
    * Interesting for data applications, not really for voice application.
  * Si2400 + Si3015
    * 1200bps or slower modem IC.
    * It has a voice codec on board, but it's 14bit at 9600kHz.
  * **[Si3050 + Si3019](http://www.silabs.com/Marcom%20Documents/Resources/Si3050_PB.pdf)**
    * PCM interface ([ITU-T G.711](http://en.wikipedia.org/wiki/G.711) codec on board: 8bit/8kHz, supports µ-law and A-law)
    * No isolation transfo needed
    * No optocoupler needed
    * Ring Detector output (can trigger interrupt on MCU)
    * Caller ID detection included
    * Available at Mouser Electronics
  * [73M1966B = 73M1906B + 73M1916](http://www.maxim-ic.com/datasheet/index.mvp/id/6806)
    * Isolation transfo needed
    * Caller ID detection included
    * Available at Mouser Electronics
    * PCM-interface

## Operating System ##
  * TinyOS targets wireless sensor networks, such as IEEE802.15.4
Below you can find the wireless protocols and modules I have been looking into.
As it seems now, the simplest implementation will be using a Bluetooth audio module.
These modules allow to simply connect the analog audio lines from the SLIC to the module (maybe using some analog intermediate circuitry).
All the other modules only accept digital data, so they need an extra ADC/DAC and CODEC stage.

  * IEEE802.15.4
    * Low cost modules available
    * Low power (only 1/3 of Bluetooth)
    * Fast wake-up from sleep (tens of milliseconds)
    * Non-beacon & beacon mode (non-beacon mode seems to be faster)
      * Beacon: routers send beacon signal at regular intervals.  All nodes must be awake at that instant in order to receive it.  Precise timing is needed for intervals.
      * Non-beacon: no device needs to be awake regularly.
    * 81 bytes/packet available for upper OSI-layers
    * 2.4GHz-band -> 250kbps (over the air rate)
    * 868MHz-band -> 20kbps (over the air rate)

  * Bluetooth Multimedia modules:
    * A SCO/eSCO link is intended for bidirectional transmission of speech. The sampling rate is fixed to 8 kHz, meaning a usable bandwidth of 3.5 kHz: BTM511, BISM2, AMB2300, LM071, BT730-SA (PCM-only), BTM411 (PCM-only)
    * A2DP: streaming audio profile: some modules can be coupled so that one serves as an audio source and the other one as an audio slave.
    * HFP/HSP: The modules here below only work as a Bluetooth slave.  You need a Bluetooth master to set up a connection (e.g. a Bluetooth-enabled smart phone).  This means that it's not possible with the modules below to set up a connection from one Bluetooth module to another one of the same type.
    * The modules below have been developed to function as a "wireless remote" for your smartphone: making phone calls, streaming music, ...
    * [LairdTech BTM511](http://www.digikey.be/search/en/RF-IF-and-RFID/RF-Transceivers/4174?keywords=BTM511)
      * integrated antenna
      * has an analog audio interface
      * 4dBm output power / -86dBm sensitivity
      * 70mA current consumption max.
      * CSR's apt-X codec
      * Supported Bluetooth profiles:
        * HSP: Headset profile
        * HFP: Handsfree profile: gives more features than HSP
        * A2DP: audio distribution profile: for connection of stereo headset: both sourcing and sinking audio supported: BTM5xx to BTM5xx application supported.
        * AVRCP: Audio/Video Remote Control Profile
        * SPP: Serial port profile)
      * [Info](http://www.lairdtech.com/Products/Embedded-Wireless-Solutions/Bluetooth-Radio-Modules/BTM51x-Modules/)
      * Prices BTM511 (integrated antenna): Farnell: €30.48, Mouser €21.91, Digikey: €21.91
    * [OmniVision OVC3860](http://www.electrodragon.com/w/images/4/4b/BLK-MD-SPK-B_AT_Commands_OVC3860_List.pdf)
      * [Preliminary datasheet](http://avrproject.ru/ovc3860/OVC3860.pdf)
      * [Bluetooth SIG Member Website](https://www.bluetooth.org/tpg/QLI_viewQDL.cfm?qid=18689)
      * [Bang good distributor](http://www.banggood.com/SPK-B-OVC3860-Stereo-Audio-Bluetooth-Speaker-Module-A2DP-AVRCP-p-91851.html)
    * Panasonic PAN13xx modules with chips from TI and Bluetooth software stack: Bluetopia.  This software stack must be run on the controlling processor.  Support only for MSP430 and Stellaris MCUs.
    * [RN52 Bluetooth Audio Module](http://www.mouser.com/ds/2/349/RN52%20DATASHEET-1.0r-221751.pdf)
      * integrated antenna
      * has an analog audio interface
      * Class 2 device: 4dBm / -85dBm sensitivity
      * 30mA typical
      * Supported Bluetooth profiles:
        * HSP: Headset profile
        * HFP: Handsfree profile: gives more features than HSP
        * A2DP: audio distribution profile: for connection of stereo headset: only one way audio supported, so no module to module communication possible.
        * AVRCP: Audio/Video Remote Control Profile
        * SPP: Serial port profile
        * [Microchip support for this module](http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en560765#documentation)
        * [Development kit with schematic](http://docs-europe.electrocomponents.com/webdocs/1200/0900766b81200c45.pdf)
        * Farnell: €21.68, RS: €20.03, Mouser: €18.11
    * [LM-Techologies LM740: Bluetooth Multimedia module](http://be.farnell.com/lm-technologies/lm740/module-bt2-1-class1-csr-bc05/dp/2102737)
      * external antenna
      * limited range: 25m open space
      * 4dBm=2.5mW output power (Class 2)/ -82dBm sensitivity
      * 30mA-40mA current consumption
      * analog audio interface
    * [BLUEGIGA - WT32-A-AI - BLUETOOTH MODULE, WT32, AUDIO, IWRAP](http://be.farnell.com/bluegiga/wt32-a-ai/bluetooth-module-wt32-audio-iwrap/dp/1656897)
      * Expensive: €70
      * 7dBM output power / -86dBm sensitivity
      * 7-70mA current consumption.
      * has analog audio interface
      * Supported profiles:
        * SPP
        * DUN: Dial up networking
        * OBEX: Object exchange
        * OPP:Object Push Profile
        * HFP (v.1.5)
        * A2DP
        * AVRCP
        * DID (?)
        * HID: Human Interface Device Profile
    * [KC-Wirefree modules](http://www.kcwirefree.com/audio.html)
      * Where can they be bought?
  * 2.4GHz proprietary protocol
    * [ANAREN - A8520E24A91GM: Bluetooth Audio module](http://be.farnell.com/anaren/a8520e24a91gm/radio-module-txrx-cc8520-int-ant/dp/2096157)
      * internal antenna
      * 150m range outside, 30m range inside
      * 16dBm=40mW output power (Class 1)/ -87dBm sensitivity
      * 48mA current consumption
      * I²S interface
      * To check if TI-PurePath supports two way audio transport for CC8520
      * Evaluation board available: [A8520E24A91-EM2](http://be.farnell.com/anaren/a8520e24a91-em2/eval-module-a8520e24a91-audio/dp/2096139?in_merch=true&MER=i-9b10-00001460)

  * Zigbee
    * Uses IEEE802.15.4 for the lower OSI-layers
    * Three types of devices: coordinators, routers & endpoints
    * [Jennic JN513x](http://be.farnell.com/jennic/jn5139-z01-m01r1v/zigbee-mod-2-4ghz-uart-sma-conn/dp/1500176)
      * There is a [wireless audio reference design](http://www.jennic.com/support/support_files/jn-rd-6004_wireless_audio_reference_design) for this
      * I²S audio interface

  * Zigbee Pro
    * Zigbee pro offers multicasting, sharing secret keys.
    * Zigbee pro is backwards compatible with Zigbee

  * RF4CE
    * Uses IEEE802.15.4 for the lower OSI-layers
    * Designed to replace remote controls, but it has already been used to make phone calls with it.  See [YouTube](http://www.google.be/url?sa=t&source=web&cd=1&ved=0CB4QtwIwAA&url=http%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3DNhzwm5vgUPc&ei=KADcTYyNNcqeOuTuyfcO&usg=AFQjCNHdgcaPxPAbvCjx5rINE4rpEvgiiw).
    * Supported by different manufacturers: TI (RemoTI), FreeScale (Beestack Consumer), Renesas, [ST](http://www.st.com/internet/evalboard/product/247094.jsp), ...
    * Smaller code size than Zigbee (because mesh-networking is not needed)

  * DASH7
    * Very new, nothing available yet (anno 2011)
    * DASH7 module 500kbps over-the-air at 433MHz (only one module found)

  * Z-Wave
    * Too slow (only 40kbps)

  * ISM-Band sub 1GHz: Proprietary protocols
    * [Ciseco ARF](http://shop.ciseco.co.uk/arf-high-power-radio-transceiver/):
      * ISM Band (868MHz & 915MHz)
    * [Ciseco XRF](http://shop.ciseco.co.uk/xrf-wireless-rf-radio-uart-rs232-serial-data-module-xbee-shape-arduino-pic-etc/)
      * TI CC1110 Radio
      * 10dBm output power /
    * [Amber Wireless AMB8425](http://be.farnell.com/amber-wireless/amb8425/rf-module-txrx-cc1101-ext-ant/dp/1749514)
      * Comparable to Ciseco, but not configurable, only 868MHz band.
      * External antenna, the AMB8420 has an internal antenna.

## Wireless modules ##

Microchip IEEE802.15.4 modules:

|Product Family	|Sensitivity dBm	|Power Output dBm	|RSSI	|TX Power Consumption mA	|RX Power Consumption mA	|Clock	|Sleep	|MAC	|MAC Features	|Encryption	|Interface	|Pin Count	|
|:--------------|:---------------|:----------------|:----|:-----------------------|:-----------------------|:-----|:-----|:---|:------------|:----------|:---------|:---------|
|MRF24J40MA	|-95.00		|0.00			|Yes	|23.00			|19.00			|20 MHz	|Yes	|Yes	|CSMA-CA		|AES128		|4-wire SPI	|12		|
|MRF24J40MB	|-102.00		|20.00			|Yes	|120.00			|25.00			|20 MHz	|Yes	|Yes	|CSMA-CA		|AES128		|4-wire SPI	|12		|

These modules can handle Zigbee, but that's not necessary.  Because some people find Zigbee too complex, Microchip has its own lightweight (small) protocol stack (MiWi).

RS: overview of Zigbee modules (sorting ascending on price):
  * Microchip's modules are cheapest, no stack included, it's only a radio -> Use MiWi (30kbps) or Zigbee or ...
  * Telegesis: stack included datarate: 20kbps.
  * Atmel: zigbit: only a radio: 2 protocol stacks available
  * XBee: protocol stack included. (cheapest bij Mouser)

### Modules verification ###
What's the throughput of serial wireless modules?  A solution in C# has been developed to test this.  It consists of two projects, one for sending bytes and one for receiving bytes.
The sending application opens the serial port with RTS/CTS flow control to prevent overflow of buffers in the transmitting module.  The application sends bytes as long as the transmitting module is requesting them by holding /CTS-low.

The receiving module starts a timer, then receives all the bytes until the timer runs out, typically after ten seconds.  The number of bytes received is multiplied by eight to get the number of received bits and afterwards divided by the timer period to get the throughput baudrate.

  * XBee Pro modules: 86kbps - 89kbps (<0.5m interdistance).
  * Radiotronix modules: NT
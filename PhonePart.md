# Shield #
## Component selection ##
### No telephony hardware ###
  * [Bluetooth Portable Rotary Phone - Black](https://www.sparkfun.com/products/9803)
    * Needs significant changes to the original telephone hardware.
### Modules solutions ###
  * [Silvertel SLICs](http://www.silvertel.com/tel_products.htm)
    * Some have integrated ringing generator, no HV needed.
    * No DTMF decoding
    * ADC/DAC needed (maybe MCU peripheral)
    * Hard/Expensive to get (shipping costs are a multiple of the value of the module)
  * E-tross China copies
    * QCX601 =? AG1170
    * MX801 =? AG1171
    * QCX2102
  * R-tone China copies
    * [KS0835, KS0805, ...](http://www.r-tone.com/en/ProductList.asp?SortID=19)
    * [I ordered KS0835F product here (cheap & fast delivery)](http://www.aliexpress.com/store/product/RING-SLIC-module-KS0835F-same-as-AG1171-AG1170/203008_535930990.html)
    * [Datasheet of KS0835F](http://www.r-tone.com/Upload/DownFiles/KS0835F_SLIC_SPEC_V1_1.pdf)
      * KS0835F = AG1171
      * KS0835F is much cheaper than SI3210 + SI3201
      * Much easier to implement
      * It's a separate module, so maybe you'll need a bigger housing.
### Single chip solutions ###
Search on the internet for: [SLIC-interface](http://lmgtfy.com/?q=SLIC+interface)
  * Intersil HC55185:
    * Yet to be investigated.
    * Available at [Mouser](http://be.mouser.com/ProductDetail/Intersil/HC55185DIMZ/?qs=%2fha2pyFaduh7RyBEUrtKaUaC43acXUnqGr8WYToSvGQ%3d)
    * €6.83/pce
  * STLC3080: not chosen, can't create ringing signal automatically
  * STLC3075:
    * created especially for wireless local loops
    * it's hard to get (not available at RS, Farnell, Mouser, Digikey)
    * ST states it as an obsolete component
  * **[Si3210-FT](http://www.silabs.com/Marcom%20Documents/Resources/Si3210ProductBrief.pdf)**
    * active high for enabling DC for ringer
    * € 7,84 at Mouser
    * Extra Si3201-GS (€1.83) needed, or using lots of discrete components
  * [SI3210-GT](http://www.silabs.com/Marcom%20Documents/Resources/Si3210ProductBrief.pdf)
    * active low for enabling DC for ringer
    * € 7,03 at Mouser
    * Extra Si3201-GS (€1.83) needed, or using lots of discrete components
    * When ringer is needed, this circuit can not work on 3.3V (datasheet table 21)
  * Si3215 & Si3216 are harder to get (non-stock at Mouser)


The design of [OpenUsbFxs](http://code.google.com/p/openusbfxs/), as well as the numerous Silicon Labs application notes, were a good starting point for me.
I calculated some component values for the interface myself, but I'll start by using the OpenUsbFxs schematic.  Just to be sure.

## Hardware design ##

In order to create a more open hardware platform, I decided to design the POTS-interface as an Arduino-shield.
It could also be used for Arduino users (if they want to create software drivers for it).  Maybe an idea for [POTSduino](http://code.google.com/p/potsduino/)?

![![](http://telephonewll.googlecode.com/files/POTSduino_small_R0.png)](http://telephonewll.googlecode.com/files/POTSduino_large_R0.png)

Revision_0_
  * [PCB-data](http://telephonewll.googlecode.com/files/BRD110731_0.zip)
  * [PDF-data](http://telephonewll.googlecode.com/files/POTSduino_0.PDF)

Revision_1_
  * [PCB-data](http://telephonewll.googlecode.com/files/BRD111107_1.zip)
  * [PDF-data](http://telephonewll.googlecode.com/files/BRD111107_R1.00.PDF)

Revision_2_
  * [Picture](http://telephonewll.googlecode.com/files/BRD111107_2.JPG)
  * [Design data](http://telephonewll.googlecode.com/files/BRD111107_R2.00.PDF)

Revision_3_
  * [Design data](http://telephonewll.googlecode.com/files/BRD111107_3.PDF)

# Baseboard #
Several options possible:
  * [Arduino](http://www.arduino.cc/)
  * [chipKIT](chipkitInstallation.md)
  * [Netduino](http://netduino.com/netduino/)
  * Creating a baseboard with [LPCXpresso](HardwareDevelopment.md)
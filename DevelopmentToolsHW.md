Open source debuggers work with GDB and OpenOCD.  Different hardware solutions are possible (for a more complete list, see [OpenOCD debuggers](http://openocd.sourceforge.net/doc/html/Debug-Adapter-Hardware.html#Debug-Adapter-Hardware)):

  * [JTAGkey-Tiny](http://www.amontec.com/jtagkey-tiny.shtml): supports Cortex M3, 6MHz, Linux support, Cost=€29
  * [arm-usb-tiny-h](http://www.olimex.com/dev/arm-usb-tiny-h.html), 30MHz, should be possible to use on Linux, but problems have been reported, Cost=€40
  * [CoLink](http://www.coocox.org/Colink.htm): supports Cortex M3, DIY, only for Windows.
  * [J-Link Edu](http://be.farnell.com/segger/j-link-edu/debugger-arm-jtag-usb-educational/dp/2061343): seems to be possible to use it on Linux, but no official support, Cost=€47
  * [OOCDLink](http://www.joernonline.de/):DIY
  * [USB Wiggler](http://www.macraigor.com/usbWiggler.htm): Linux support, Cortex M3 supported, USB Hi-speed, doesn't use openOCD, but OCDRemote, Cost=$250
  * [Flyswatter](http://www.tincantools.com/product.php?productid=16134): Linux support, Cortex M3, Cost=$50

# Datasheets #
  * [LPCXpresso LPC1769 complete schematic](http://ics.nxp.com/support/documents/microcontrollers/pdf/lpcxpresso.lpc1769.schematic.pdf)
  * [LPC1769 datasheet](http://ics.nxp.com/products/lpc1000/datasheet/lpc1763.lpc1764.lpc1765.lpc1766.lpc1767.lpc1768.lpc1769.pdf)
  * [LPC1769 user manual](http://ics.nxp.com/support/documents/microcontrollers/pdf/user.manual.lpc17xx.pdf)

# Connecting debugger #
If you don't want to use the LPC-Link, you have two options:
  1. Use a saw and separate the LPC-Link from the LPCXpresso.  In this case, you'll have to provide a 3V3-supply to the VIO\_3V3X pin of the LPCXpresso, because you can't use the 3V3-supply on the LPC-Link (chip U10) any longer.
  1. If you have more space in your application, you can leave the LPC-Link on, but you have to open all solder jumpers on J4, except 1-2, 13-14 and 15-16.  So your LPCXpresso will still get it's 3V3 power from the LPC-Link.
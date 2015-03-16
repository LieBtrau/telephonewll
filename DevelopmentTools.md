# What development tool to use?
  * [Crosstool-NG](http://robovero.org/index.php/tutorials/toolchain)
  * [Fun-tech toolchain](http://fun-tech.se/stm32/gcc/index.php)
  * [MacRaigor toolchain](http://www.macraigor.com/Eclipse/index.htm)
    * Not up to date with newest versions of Ubuntu.  Install will fail.  You'll have to use "dpkg -P" to get rid of it properly.
  * [Code Red](http://www.code-red-tech.com/lpcxpresso.php)
    * Even the "getting started" didn't work without problems
      * Can't find project in zip-files
      * Examples don't debug (header files not found)
    * Not open source
    * Code limit on debugger, can't debug NuttX/nsh application.
  * [YAGARTO](http://www.yagarto.de/)
    * Doesn't contain IDE, but Eclipse is proposed.
    * The "Howto" actually works!
    * A lot of steps to take, but it's explained well.
    * Eclipse not supported on Windows Vista.
  * [devkitARM](http://www.devkit.tk/)
  * [CoIDE](http://www.coocox.org/CooCox_CoIDE.htm)
    * Can use LPC-link: see [Coocox Forum](http://www.coocox.org/Forum/topic.php?id=741)
  * [WinARM](http://gandalf.arubi.uni-kl.de/avr_projects/arm_projects/#winarm)
    * Last update from 2006.
  * [GNUARM](http://www.gnuarm.com/)
    * Seems to be dead.  No more updates after 2006.

# What kernel to use?
  * uKOS
  * chibiOS
  * TNKernel
  * Segger embOS
  * Micriµm µC
  * Ethernut
  * [NuttX (OABI toolchain)](http://nuttx.sourceforge.net/)
    * GDB version delivered with NuttX is outdated, and can't be built.
    * On Windows, Cygwin packages need to be installed -> time consuming.
    * [How to build NuttX using Linux and Buildroot](HowtoNuttxWindowsBuildroot.md)
    * [How to build NuttX using Linux and Code Red](BuildNuttxLinux.md)
    * [How to build NuttX using Windows](HowtoNuttxWindowsBuildroot.md)
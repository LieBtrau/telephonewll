Make it easy for yourself.  Install Linux (you'll be amazed how easy that is) and compile your code.

# Compiling using Buildroot for Linux #
  1. Open the package manager of your Linux installation and install:
    1. libgmp-dev
    1. libmpfr-dev
    1. zlib1g-dev
  1. Download buildroot from the NuttX website, NuttX itself and the applications of NuttX. [Download](http://sourceforge.net/projects/nuttx/files/)
  1. Unpack the three tar-files to a directory without spaces, like /home/nxp
  1. Remove the version names from the apps, buildroot and nuttx directories.
  1. Now you will have a directory view like:
    * /home/nxp
      * apps
        * examples
        * graphics
        * include
        * ...
      * buildroot
        * configs
        * package
        * toolchain
      * nuttx
        * arch
        * binfmt
        * configs
        * ...
  1. `cd ./nxp/nuttx/tools`
  1. Set up which board program you want to develop.  Here it's the "nsh" application for the lpcxpresso board
    * `./configure.sh lpcxpresso-lpc1768/nsh`
  1. Open "/nxp/nuttx/.config" using nano,or some other text editor
  1. Check that Buildroot for Linux is enabled in the toolchain and linker options section:
```
#
# Identify toolchain and linker options
#
CONFIG_LPC17_CODESOURCERYW=n
CONFIG_LPC17_CODESOURCERYL=n
CONFIG_LPC17_DEVKITARM=n
CONFIG_LPC17_BUILDROOT=y
CONFIG_LPC17_CODEREDW=n
CONFIG_LPC17_CODEREDL=n
```
  1. Save the file and close it.
  1. Open "/nxp/nuttx/setenv.sh" using nano or other text editor.  Now we also change the toolchain section here:
```
# This is where the buildroot might reside on a Linux or Cygwin system
export TOOLCHAIN_BIN="${WD}/../buildroot/build_arm_nofpu/staging_dir/bin"

# This is the default install location for Code Red on Linux
#export TOOLCHAIN_BIN="/usr/local/LPCXpresso/tools/bin"

# This the Cygwin path to the LPCXpresso 3.6 install location under Windows
#export TOOLCHAIN_BIN="/cygdrive/c/nxp/lpcxpresso_3.6/Tools/bin"
```
  1. Save the file and close it.

# Building your application #
  1. If you tried Code Red for Linux earlier on, you'll first have to delete the `make.dep` files.

  1. `cd /nxp/nuttx`
  1. `. ./setenv.sh`
  1. `make`

# Debugging your application #
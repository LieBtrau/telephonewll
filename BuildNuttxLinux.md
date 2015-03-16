Make it easy for yourself.  Install Linux (you'll be amazed how easy that is) and compile your code.

# Compiling using Code Red for Linux #
  1. Download Code Red for Linux
  1. Install Code Red for Linux
  1. Download nuttx and the applications of nuttx
  1. Unpack both tar-files to a directory without spaces, like /home/nxp
  1. Remove the version names from the apps and nuttx directories.
  1. Now you will have a directory view like:
    * /home/nxp
      * apps
        * examples
        * graphics
        * include
        * ...
      * nuttx
        * arch
        * binfmt
        * configs
        * ...
  1. `cd ./nxp/nuttx/tools`
  1. Set up which board program you want to develop.  Here it's the "nsh" application for the lpcxpresso board
    * `./configure.sh lpcxpresso-lpc1768/nsh`
  1. Open "/nxp/nuttx/.config" using nano,or some other text editor
  1. Check that Code Red for Linux is enabled in the toolchain and linker options section:
```
#
# Identify toolchain and linker options
#
CONFIG_LPC17_CODESOURCERYW=n
CONFIG_LPC17_CODESOURCERYL=n
CONFIG_LPC17_DEVKITARM=n
CONFIG_LPC17_BUILDROOT=n
CONFIG_LPC17_CODEREDW=n
CONFIG_LPC17_CODEREDL=y
```
  1. Save the file and close it.
  1. Get the directory path of the tools/bin folder of your Code Red installation.
  1. Open "/nxp/nuttx/setenv.sh" using nano or other text editor.  Now we also change the toolchain section here:
```
# This is where the buildroot might reside on a Linux or Cygwin system
#export TOOLCHAIN_BIN="${WD}/../buildroot/build_arm_nofpu/staging_dir/bin"

# This is the default install location for Code Red on Linux
export TOOLCHAIN_BIN="/usr/local/LPCXpresso/tools/bin"

# This the Cygwin path to the LPCXpresso 3.6 install location under Windows
#export TOOLCHAIN_BIN="/cygdrive/c/nxp/lpcxpresso_3.6/Tools/bin"
```
  1. Replace the `/usr/local/LPCXpresso/tools/bin` by the correct path to your /tools/bin directory of your Code Red installation (e.g. "/usr/local/lpcxpresso\_4.1.5\_187/lpcxpresso/tools/bin")
  1. Save the file and close it.

# Building your application #
  1. `cd /nxp/nuttx`
  1. `. ./setenv.sh`
  1. `make`

# Debugging your application #
http://knowledgebase.nxp.com/showthread.php?t=2361&langid=2
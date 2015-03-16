# Development #
The software will be based on the [openusbfxs](http://code.google.com/p/openusbfxs/).

The system consists of three blocks:
  * SI3210 chip POTS-interface
  * LPCXpresso
  * PC

These three blocks are connected using interfaces:

# Interfaces #
## Interfase SI3210 to LPCXpresso ##

## Interface LPCXpresso to PC ##
Command list:
  * READ\_VERSION
    * reads the software version of the embedded code of the LPCXpresso.
  * PROSLIC\_WDIRECT
    * checks if the addressed direct register is valid
    * write 8bit data to a control (=direct) register
    * this function automatically calls PROSLIC\_RDIRECT, so that it's checked that the byte has actually been written.
  * PROSLIC\_RDIRECT
    * checks if the addressed direct register is valid
    * read 8bit data from a control (=direct) register
  * PROSLIC\_WRINDIR
    * write data to an indirect register
    * this function automatically calls PROSLIC\_RDINDIR, so that it's checked that the byte has actually been written.
  * PROSLIC\_RDINDIR
    * read data from an indirect register
    * the register address will be returned as well.
  * PROSLIC\_INIT
    * reset the PROSLIC and initialize it afterwards.
  * PROSLIC\_WRPCM
    * write PCM-data to the PROSLIC
    * this data will be (most often digitized voice) data coming from the PC.
  * PROSLIC\_RDPCM
    * read PCM-data from the PROSLIC
    * this data comes from the phone connected to the PROSLIC

Remark that the [openusbfxs](http://code.google.com/p/openusbfxs/) doesn't have a PROSLIC\_WRPCM or PROSLIC\_RDPCM command.  That's because they put the USB-connection into isochronous mode.  At regular time intervals, data is "automatically" transferred between PC and openusbfxs.  There's no need for the PC to poll for data regularly.
In their project, the isochronous mode is not only used to transfer PCM-data, but also to set the registers of the PROSLIC.
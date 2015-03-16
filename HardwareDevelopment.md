# Hardware Design #
The LPCduino is actually a baseboard for an LPCXpresso-board, so that LPCXpresso boards can be connected to Arduino shields.

The Arduino has limited computational resources.  This could become a problem in this project, especially if some real-time speech compression needs to be done on board.
So some kind of "speed upgrade" of the Arduino was needed.  At the time of writing, boards like the [chipKIT Uno32 and the chipKIT Max32](http://www.digilentinc.com/Products/Catalog.cfm?NavPath=2,892&Cat=18) didn't exist yet.  Developing a pin-compatible LPCduino came as a logical next step.

## LPCduino shield ##
![![](http://telephonewll.googlecode.com/files/LPCduino_0_small.png)](http://telephonewll.googlecode.com/files/LPCduino_0.png)
  * [PDF-data](http://telephonewll.googlecode.com/files/LPCduino_0.PDF)

# Software Development #
## Tools ##
  * [Software](DevelopmentTools.md)
  * [Hardware](DevelopmentToolsHW.md)
//Code adapted from: http://www.markfickett.com/stuff/artPage.php?id=377
#pragma once
#include <Arduino.h>

/**
 * Read numbers from a rotary dial (or otherwise using pulse dialing; see
 * http://en.wikipedia.org/wiki/Rotary_dial ). 
 *
 * See the README for further documentation.
 */
class RotaryDialer {
private:
  byte _yNumber;
  boolean _bFinishedNumberRead;

  enum PulseState {
    WAITING_FOR_LOOP_CLOSURE,
    WAITING_FOR_LOOP_OPENING,
  };
  enum PulseState _tPulseState;
  unsigned long _uiTimeOfLastPulse;

  /**
   * 		 * Change state, but only if enough time has elapsed since
   * 		 * the last state change (to protect from noise).
   		 */
  boolean getPulsePeriod(unsigned long&);

  void updatePulseCounter();
public:
  /**
   * 		 * Create a new RotaryDialer l
   		 */
  RotaryDialer();

  /**
   * 		 * Initialize the pins; digital read pins, held HIGH.
   		 */
  void setup();

  /**
   * 		 * Check the pins and update state (in or out of a pulse,
   * 		 * dialing complete, etc). This must be called at least as
   * 		 * pulses; assuming 10 pulses per second, every 50ms.
   		 */
  void updatePulseState(byte yLoopStatus);

  /**
   * 		 * @return whether a new number has been dialed since the last
   * 		 *	getNextNumber call
   		 */
  boolean hasNextNumber();

  /**
   * 		 * Get the most recently dialed number. After this is called,
   * 		 * hasNextNumber will return false until a new number is dialed.
   * 		 * @return the most recently dialed number, and clear
   * 		 *	hasNextNumber
   		 */
  boolean getNextNumber(byte&);
};




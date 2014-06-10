/**
 * The power system is defined by the following state machine.
 * @version June 10, 2014.
 * @author Cyrille Toulet, <cyrille.toulet@gmail.com>.
 */

#ifndef PIRACK_STATE_MACHINE_H

  // Avoid recursions
  #define PIRACK_STATE_MACHINE_H
  
  // Include the Arduino libraries
  #include "Arduino.h"

  // The default values
  #define PIRACK_STATE_MACHINE_IDLE_TIME 2500
  #define PIRACK_STATE_MACHINE_REBOOT_TIME 3500
  #define PIRACK_STATE_MACHINE_BIP_DURATION 100
  #define PIRACK_STATE_MACHINE_BLINK_DURATION 250
  #define PIRACK_STATE_MACHINE_REBOOT_DURATION 1000

  /**
   * The diferents states
   * For more information, see Readme.md
   */
  enum _state_e {
    PIRACK_STATE_OFF,
    PIRACK_STATE_ON,
    PIRACK_STATE_IDLE,
    PIRACK_STATE_REBOOT,
    PIRACK_STATE_POWER_DOWN
  };

  /**
   * The state machine class
   */
  class PiRackStateMachine {
  
    public:
      /**
       * Constructor
       * @param switchPin The switch input pin (with a pull-up resistor)
       * @param switchPin The led output pin
       * @param switchPin The buzzer output pin
       * @param switchPin The relay output pin
       */
      PiRackStateMachine(int switchPin, int ledPin, int buzzerPin, int relayPin);
      
      /**
       * Setup the state machine
       */
      void setup();
      
      /**
       * Run a step of automaton
       * The entry step is "OFF"
       */
      void step();
  
  
    private:
      // HMI configuration
      int _switchPin;
      int _ledPin;
      int _buzzerPin;
      int _relayPin;
      int _idleTime;
      int _rebootTime;
      int _bipDuration;
      int _blinkDuration;
      int _rebootDuration;
      
      // State machine attributes
      _state_e _state;
      int _switchMemory;
      int _timer;
      int _blink;
      int _led;
  };

#endif

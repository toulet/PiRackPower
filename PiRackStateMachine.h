/**
 *
 *
 * @author Cyrille Toulet, <cyrille.toulet@gmail.com>
 */

#ifndef PiRackStateMachine_h

  #define PiRackStateMachine_h
  #include "Arduino.h"

  enum state {
    OFF,
    ON,
    ATTEMPT,
    REBOOT
  };

  class PiRackStateMachine {
  
    public:
      PiRackStateMachine(int switchPin, int ledPin, int buzzerPin, int relayPin, int safeTime, int rebootTime);
      void setup();
      void step();
  
    private:
      // HMI configuration
      int _switchPin;
      int _ledPin;
      int _buzzerPin;
      int _relayPin;
      int _safeTime;
      int _rebootTime;
      
      // State machine attributes
      state _currentState;
      int _lastSwitchValue;
      int _switchPressedFor;
      int _bipDuration;
  };

#endif

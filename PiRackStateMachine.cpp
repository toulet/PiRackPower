/**
 *
 *
 * @author Cyrille Toulet, <cyrille.toulet@gmail.com>
 */

#include "Arduino.h"
#include "PiRackStateMachine.h"


/**
 * @see PiRackStateMachine.h
 */
PiRackStateMachine::PiRackStateMachine(int switchPin, int ledPin, int buzzerPin, int relayPin, int safeTime, int rebootTime) {
  // The HMI settings
  _switchPin = switchPin;
  _ledPin = ledPin;
  _buzzerPin = buzzerPin;
  _relayPin = relayPin;
  _safeTime = safeTime;
  _rebootTime = rebootTime;
  
  // The state machine attributes
  _currentState = OFF;
  _lastSwitchValue = HIGH;
  _switchPressedFor = 0;
  _bipDuration = 100;
}


/**
 * @see PiRackStateMachine.h
 */
void PiRackStateMachine::setup() {
  pinMode(_switchPin, INPUT);
  pinMode(_ledPin, OUTPUT);
  pinMode(_relayPin, OUTPUT);
  pinMode(_buzzerPin, OUTPUT);
  Serial.begin(9600);
}


/**
 * @see PiRackStateMachine.h
 */
void PiRackStateMachine::step() {
  // Read switch value
  int switchValue = digitalRead(_switchPin);

  // Set the state machine
  switch (_currentState) {
    // State ATTEMPT
    case ATTEMPT:
      // If the switch was pressed
      if (_lastSwitchValue == LOW) {
        // If the switch was released
        if (switchValue == HIGH) {
          // If we request reboot
          if (_switchPressedFor >= _safeTime) {
            // Go to state REBOOT
            _currentState = REBOOT;
            Serial.println("ATTEMPT -> REBOOT");
          }
          // If the safe time isn't exceeded
          else {
            // Go to state ON
            _currentState = ON;
            Serial.println("ATTEMPT -> ON");
          }
          
          // Reset pressed time
          _switchPressedFor = 0;
        }
        // Else, if switch is still pressed, count time
        else {
          delay(45);
          _switchPressedFor += 50;
        }
        
        // If it's time to power down
        if (_switchPressedFor >= _safeTime + _rebootTime) {
          // Switch off the led
          digitalWrite(_ledPin, LOW);
      
          // Power down
          digitalWrite(_relayPin, LOW);
        
          // Bip bip
          digitalWrite(_buzzerPin, HIGH);
          delay(_bipDuration);
          digitalWrite(_buzzerPin, LOW);
          delay(_bipDuration);
          digitalWrite(_buzzerPin, HIGH);
          delay(_bipDuration);
          digitalWrite(_buzzerPin, LOW);
          
          // Reset pressed time
          _switchPressedFor = 0;
          
          // Go to state OFF
          _currentState = OFF;
          Serial.println("ATTEMPT -> OFF");
          }
      }
      
    // End of state ATTEMPT
    break;
    

    // State ON
    case ON:
      // Switch on the led
      digitalWrite(_ledPin, HIGH);

      // Power up
      digitalWrite(_relayPin, HIGH);
      
      // If the switch is pressed
      if (_lastSwitchValue == HIGH && switchValue == LOW) {
        // Go to state ATTEMPT
        _currentState = ATTEMPT;
        Serial.println("ON -> ATTEMPT");
      }
      
    // End of state ON
    break;
    
    
    // State OFF
    case OFF:
      // Switch off the led
      digitalWrite(_ledPin, LOW);

      // Power down
      digitalWrite(_relayPin, LOW);

      // If the switch was released
      if (_lastSwitchValue == LOW && switchValue == HIGH) {
        // Bip
        digitalWrite(_buzzerPin, HIGH);
        delay(_bipDuration);
        digitalWrite(_buzzerPin, LOW);
      
        // Go to state ON
        _currentState = ON;
        Serial.println("OFF -> ON");
      }
        
    // End of state OFF
    break;
    
    
    // State REBOOT
    case REBOOT:
      // Switch off the led
      digitalWrite(_ledPin, LOW);

      // Power down
      digitalWrite(_relayPin, LOW);

      // Bip
      digitalWrite(_buzzerPin, HIGH);
      delay(_bipDuration);
      digitalWrite(_buzzerPin, LOW);
      
      // Go to state ON
      _currentState = ON;
      Serial.println("REBOOT -> ON");

    // End of state REBOOT
    break;


    // Else, somthing goes wrong ...
    default:
      Serial.println("ERROR");
      // Reinitialize the state machine
      _currentState = OFF;
      _switchPressedFor = 0;
  }
  
  // Update values
  _lastSwitchValue = switchValue;
}


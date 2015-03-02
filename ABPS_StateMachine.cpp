/**
 * The power system state machine
 * @version March 3, 2015
 * @author Cyrille TOULET, <cyrille.toulet@linux.com>
 */

#include "Arduino.h"
#include "ABPS_StateMachine.h"


/**
 * @see ABPS_StateMachine.h
 */
ABPS_StateMachine::ABPS_StateMachine(int switchPin, int ledPin, int buzzerPin, 
    int relayPin) {
    // The HMI settings
    this->_switchPin = switchPin;
    this->_ledPin = ledPin;
    this->_buzzerPin = buzzerPin;
    this->_relayPin = relayPin;
    this->_idleTime = ABPS_STATE_MACHINE_IDLE_TIME;
    this->_rebootTime = ABPS_STATE_MACHINE_REBOOT_TIME;
    this->_bipDuration = ABPS_STATE_MACHINE_BIP_DURATION;
    this->_blinkDuration = ABPS_STATE_MACHINE_BLINK_DURATION;
    this->_rebootDuration = ABPS_STATE_MACHINE_REBOOT_DURATION;
    
    // The state machine attributes
    this->_state = ABPS_STATE_OFF;
    this->_switchMemory = HIGH;
    this->_led = HIGH;
    this->_timer = 0;
    this->_blink = 0;
}


/**
 * @see ABPS_StateMachine.h
 */
void ABPS_StateMachine::setup() {
    // Set pin modes
    pinMode(this->_switchPin, INPUT);
    pinMode(this->_ledPin, OUTPUT);
    pinMode(this->_relayPin, OUTPUT);
    pinMode(this->_buzzerPin, OUTPUT);
    
    // Init pins
    digitalWrite(this->_ledPin, LOW);
    digitalWrite(this->_relayPin, LOW);
    digitalWrite(this->_buzzerPin, LOW);
    
    // Init attributes
    this->_state = ABPS_STATE_OFF;
    this->_switchMemory = HIGH;
    this->_led = HIGH;
    this->_timer = 0;
    this->_blink = 0;
}


/**
 * @see ABPS_StateMachine.h
 */
void ABPS_StateMachine::step() {
    // Read switch value
    int switchValue = digitalRead(this->_switchPin);

    // Set the state machine
    switch (this->_state) {
        // State OFF
        case ABPS_STATE_OFF:
            // If the switch was pressed and released
            if (this->_switchMemory == LOW && switchValue == HIGH) {
                // Power up
                digitalWrite(this->_relayPin, HIGH);
                // Switch on the led
                digitalWrite(this->_ledPin, HIGH);
                // Bip one time
                digitalWrite(this->_buzzerPin, HIGH);
                delay(this->_bipDuration);
                digitalWrite(this->_buzzerPin, LOW);
                
                // Wait release
                while ((switchValue = digitalRead(this->_switchPin)) != LOW);
                
                // Go to state ON
                this->_state = ABPS_STATE_ON;
            }
            
            // End of State OFF
            break;
        
        // State ON
        case ABPS_STATE_ON:
            // If the switch is pressed
            if (this->_switchMemory == HIGH && switchValue == LOW) {
                // Go to state IDLE
                this->_state = ABPS_STATE_IDLE;
            }
            
            // End of State ON
            break;
        
        // State IDLE
        case ABPS_STATE_IDLE:
            // If the switch is still pressed
            if (this->_switchMemory == LOW && switchValue == LOW) {
                // Update timer
                delay(45);
                this->_timer += 50;
                
                // If the timer exceed ABPS_STATE_IDLE time
                if (this->_timer >= this->_idleTime) {
                    // Go to state REBOOT
                    this->_state = ABPS_STATE_REBOOT;
                }
            }
            // Else, the switch was released
            else {
                // Reset timer
                this->_timer = 0;
                
                // Go to state ON
                this->_state = ABPS_STATE_ON;
            }
            
            // End of state IDLE
            break;
        
        // State REBOOT
        case ABPS_STATE_REBOOT:
            // If the switch is still pressed
            if (this->_switchMemory == LOW && switchValue == LOW) {
                // Update timer
                delay(45);
                this->_timer += 50;
                this->_blink += 50;
                
                // Toogle led state
                if (this->_blink >= this->_blinkDuration) {
                    // Reset timer
                    this->_blink = 0;
                    
                    // Toogle led
                    this->_led = this->_led == HIGH? LOW:HIGH;
                    digitalWrite(this->_ledPin, this->_led);
                }
                
                // If the timer exceed ABPS_STATE_IDLE + ABPS_STATE_REBOOT time
                if (this->_timer >= this->_idleTime + this->_rebootTime) {
                    // Power down
                    digitalWrite(this->_relayPin, LOW);
                    // Switch off the led
                    digitalWrite(this->_ledPin, LOW);
                    // Bip two times
                    digitalWrite(this->_buzzerPin, HIGH);
                    delay(this->_bipDuration);
                    digitalWrite(this->_buzzerPin, LOW);
                    delay(this->_bipDuration);
                    digitalWrite(this->_buzzerPin, HIGH);
                    delay(this->_bipDuration);
                    digitalWrite(this->_buzzerPin, LOW);
                    
                    // Reset led blinking
                    this->_blink = 0;
                    this->_led = HIGH;
                    
                    // Go to state POWER_DOWN
                    this->_state = ABPS_STATE_POWER_DOWN;
                }
            }
            // Else, the switch was released
            else {
                // Power down
                digitalWrite(this->_relayPin, LOW);
                // Switch off the led
                digitalWrite(this->_ledPin, LOW);
                // Bip one time
                digitalWrite(this->_buzzerPin, HIGH);
                delay(this->_bipDuration);
                digitalWrite(this->_buzzerPin, LOW);
                // Wait a moment
                delay(this->_rebootDuration);
                // Power up
                digitalWrite(this->_relayPin, HIGH);
                // Switch on the led
                digitalWrite(this->_ledPin, HIGH);
                
                // Reset timer
                this->_timer = 0;
             
                // Go to state ON
                this->_state = ABPS_STATE_ON;
            }
            
            // End of state REBOOT
            break;

        // State DOWN
        case ABPS_STATE_POWER_DOWN:
            // If the switch was released
            if (this->_switchMemory == LOW) {
                // Reset timer
                this->_timer = 0;
                
                // Reset led blinking
                this->_blink = 0;
                this->_led = LOW;
                
                // Wait release
                while ((switchValue = digitalRead(this->_switchPin)) != HIGH);
                
                // Go to state OFF
                this->_state = ABPS_STATE_OFF;
            }
            
            // End of state DOWN
            break;
            
        // Else, somthing goes wrong ...
        default:
            // Reinitialize the state machine
            this->setup();
    }
    
    // Update values
    this->_switchMemory = switchValue;
}


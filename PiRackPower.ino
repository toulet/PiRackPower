/**
 *
 *
 * @author Cyrille Toulet, <cyrille.toulet@gmail.com>
 */

#include "PiRackStateMachine.h"


// Configuration of press and release times
int SAFE_TIME = 2500;
int REBOOT_TIME = 5000;

// Configuration of buzzer
int BUZZER_PIN = 2;

// Configuration of first hardware parts
int POWER_0_SWITCH_PIN = 3;
int POWER_0_LED_PIN = 4;
int POWER_0_RELAY_PIN = 5;

// The state machine for first RPi
PiRackStateMachine sm1(POWER_0_SWITCH_PIN, POWER_0_LED_PIN, BUZZER_PIN, POWER_0_RELAY_PIN, SAFE_TIME, REBOOT_TIME);


/**
 * Setup the application
 */
void setup() {
  // Set up the first state machine
  sm1.setup();
}


/**
 * The main loop
 */
void loop() {
  // Run a step of first state machine
  sm1.step();
}


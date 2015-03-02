# ABPS - Arduino based power supply

ABPS is an Arduino based power-supply which can be used for manage any 
electronic system power, like micro-computers (Raspberry Pi, BeagleBone, etc).


## Contributors

Cyrille TOULET, <cyrille.toulet@linux.com>


## The state machine
The power system is defined by the following state machine.

The default entry state is **OFF**, but we can use a persistent memory to 
restore the system to its last state after a power interrupt.

```
+----------+       Press & Release       +------------+
|   OFF    | --------------------------> |     ON     | <-------------+
+----------+                 ,---------> +------------+               |
     ^                      |                  |                      |
     |                      |                  | Press while(T)       |
     |                      |                  v                      |
     |                      |            +------------+               |
     |                      +----------- |    IDLE    |               |
     |                         Release   +------------+               |
     |                                         |                      |
     |                                         | T >= IDLE_TIME       |
     |                                         v                      |
     |                                   +------------+               |
     |                                   |   REBOOT   | --------------+
     |                                   +------------+     Release
     |                                         |
     |                                         | T >= IDLE_TIME + REBOOT_TIME
     |                                         v 
     |               Release             +------------+
     +---------------------------------- | POWER_DOWN |
                                         +------------+
```

### Actions
- **OFF** [*Press* & *Release*] -> **ON**:
  - Power down
  - Switch off the LED
  - Bip one time
- **REBOOT** [*Release*] -> **ON**:
  - Power down
  - Switch off the LED
  - Bip one time
  - Wait a moment
  - Power up
  - Switch on the LED
- **REBOOT**:
  - Blink the LED
- **REBOOT** [*T >= IDLE_TIME + REBOOT_TIME*] -> **POWER_DOWN**:
  - Power down
  - Switch off the LED
  - Bip two times



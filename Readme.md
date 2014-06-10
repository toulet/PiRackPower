PiRackPower
===========

# The state machine

```
+----------+       Press & Release       +-----------+
|   OFF    | --------------------------> |    ON     | <-------------+
+----------+                 ,---------> +-----------+               |
     ^                      |                 |                      |
     |                      |                 | Press while(T)       |
     |                      |                 v                      |
     |                      |            +-----------+               |
     |                      +----------- |   IDLE    |               |
     |                         Release   +-----------+               |
     |                                        |                      |
     |                                        | T >= IDLE_TIME       |
     |                                        v                      |
     |                                   +-----------+               |
     |                                   |  REBOOT   | --------------+
     |                                   +-----------+     Release
     |                                        |
     |                                        | T >= IDLE_TIME + REBOOT_TIME
     |                                        v 
     |               Release             +-----------+
     +---------------------------------- | POWERDOWN |
                                         +-----------+
```

## Actions
- OFF [Press & Release] -> ON:
  - Power : on
  - Led   : on
  - Bip   : 1
- REBOOT [Release] -> ON:
  - Power : off
  - Led   : on
  - Bip   : 1
  - Power : on
- POWERDOWN [Release] -> OFF:
  - Power : off
  - Led   : off
  - Bip   : 2



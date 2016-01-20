#Laser Safety System
A low cost laser cutter/engraver safety monitoring system for K40 and similair laser systems.

##Features

####Configuration File
Configuration of the system is accomplished by editing a simple text file.
***
####Door interlock events
Door interlock input for the main work area door.
***
####Temperature monitoring events
Input for monitoring coolant temperature.
***
####Coolant flow monitoring events
One input to monitor coolant flow using low cost flow sensor.
***
####Audible Alarm
Three types of alarm tones are available, a slow beep, a fast beep, and a continuous tone. Events can be assigned one of the three tones.
***
####Visual Indicators
There are 2 LED outputs that at a glance indicate the overall health of the laser system. Red is for a critical shutdown error and green indicates all is well with the system.
***
####Automatic Laser Shutdown
Critical event disables the Laser
***
##Arduino Pin Assignments
####These pin assignments are based on the Arduino Uno board
  1. D5 - Main door interlock
  2. D6 - Control door interlock
  3. D7 - Laser tube door interlock
  4. D8 - Coolant flow meter input
  5. D9 - Red LED
  6. D10 - Yellow LED
  7. D11 - Green LED
  8. D12 - Alarm buzzer
  9. A3 - Coolant temperature input
  10. A4 - Laser tube temperature input

***
####Required Hardware
  1. Arduino Uno board
  2. Microswitch for each door interlock
  3. TMP36 temperature sensor for each temperature monitoring event
  4. 5V relay

***

#Laser Safety System
A low cost laser cutter/engraver safety monitoring system for K40 and similair laser systems.

##Features

####Door interlock event
Door interlock input for the main work area door.
***
####Temperature monitoring event
Input for monitoring coolant temperature.
***
####Coolant flow monitoring event
Input to monitor coolant flow using low cost flow sensor.
***
####Audible Alarm
Critical event sets off buzzer with continuous tone.
***

####Automatic Laser Shutdown
Critical event disables the Laser
***
##Arduino Pin Assignments
####These pin assignments are based on the Arduino Uno board
  1. D2 - Flow senso input
  2. D3 - Door microswitch input
  3. D4 - Buzzer alarm output
  4. D5 - Relay trigger pin to disable laser
  5. D9 - Red LED
  6. A0 - Coolant temperature sensor input

***
####Required Hardware
  1. Arduino Nano with expansion board
  2. Microswitch for door interlock
  3. TMP36 temperature sensor for coolant temperature monitoring
  4. 5V relay to disable laser firing

***

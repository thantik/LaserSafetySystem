#Laser Safety System
A laser cutter/engraver safety monitoring system for K40 and similair laser systems running on an Arduino board.

##Features

####Configuration File
Configuration of the system is accomplished through editing a simple text file.
***
####Door interlocks
Three door interlock inputs for the main work area door, controls and power supplly door, and the laser tube compartment door. It is configurable to use only the interlocks you want. (Main work area interlock is mandatory.)
***
####Temperature Monitoring
Two inputs for monitoring coolant temperature and laser tube temperature. These inputs are configurable. (Coolant temperature moniotoring is mandatory, laser tube temperature is optional.)
***
####Coolant Flow Monitoring
One input to monitor coolant flow using low cost flow sensor.
***
####Audible Alarm
Three types of alarm tones are available, a slow beep, a fast beep, and a continuous tone. Events can be assigned one of the three tones.
***
####Visual Indicators
There are 3 LED outputs that at a glance indicate the overall health of the laser system. Red is for a critical shutdown error, yellow is for a problem that does not require immediate shutdown, and green indicates all is well with the system.

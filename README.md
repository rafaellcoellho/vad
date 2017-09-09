# Ventricular Assist Device prototype for tests

This project was used in a VAD (Ventricular Assist Device) prototype implanted in a pig for a few hours to test the tecnology. His 
main functionality is assisting cardiac circulation, which is used either to partially or to completely replace the function of a 
failing heart. 

The architecture of this firmware is interrupt based. In the main loop the buttons with less priority is checked 
continously, and the interrupt of the timer control the pump's speed.

User interface with LED and buttons are made with the keypad.h, using the facade pattern. 

The control of brushless DC motors are made with the DEC_module.h, using the facade pattern also.

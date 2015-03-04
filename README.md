rccarlights
===========

This Arduino program implements a light controller for a RC car. The light controller requires only a 2 Channel RC controller but
provides light switch and more.

## Functionality
The following action can be either controlled by the "virtual" switches (see below) or are triggered automatically by the controller itself:

* *Manually controlled*
    * **parking lights** - The parking lights and the tail lights can be controlled with a "virtual" switch. E.g. by pressing the throttle switch about 1 second with a minimal deflection, the lights can be toggled.

* *Automated*
    * **turn lights right and left** - The turn lights will be switched on if the car stands still and the steering wheel was turned right or left and will be stopped immediately after the steering wheel is in neutral position. When the car is in movement the blinker will not be triggered by the steering wheel.
    * **brake lights** - The brake lights are switched on if the hand throttle is moved fast towards the neutral position. The program used a threshold to determine the speed of the change of the hand throttle.
    * **back-up lights** - The back-up lights will be switched on when the throttle is in reverse position.
    * **headlights** - The headlights will be switched on, when the throttle is presses in any direction after the parking light switched on manually. The headlights will turns off after the throttle switch is in neutral position for several seconds.

## Virtual Switches
The program provides different "virtual" switches, which can be used to switch on lights or other extra functionality. The switches will be controlled via the throttle or the steering channels. At the moment the hand throttle has to be pressed with a deflection of 5-10% for about 1 second to turn on/off the parking and tail lights. The deflection could vary and may has to be adapted to the remote controller used. Be aware that depending on the speed controller your car starts moving when switch on the lights. Instead the steering switch could be used, but requires some changes in the RcCarLights class.

## Known Issues
At the moment neither Makefiles nor Eclipse project files are part of the project.


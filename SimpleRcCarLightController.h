/*--------------------------------------------------------------------
 * This file is part of the RcCarLights arduino application.
 *
 * RcCarLights is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RcCarLights is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RcCarLights.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright: Jochen Schales 2014
 *
 * --------------------------------------------------------------------*/

#ifndef SIMPLERCCARLIGHTCONTROLLER_H_
#define SIMPLERCCARLIGHTCONTROLLER_H_

#include "AbstractRcCarLightController.h"
#include "LightSwitchBehaviour.h"

/**
 * Simple implementation of a RcCarLight output class
 *
 * This class uses 6 pins to control the lights for parking, headlights, brake, backup light, right and left blinker.
 *
 * The used pins have to passed in the right order to the constructor and the loop method will set these pins to HIGH
 * according the light status passed.
 */
class SimpleRcCarLightController : public AbstractRcCarLightController
{
public:
    /**
     * Constructor
     * @param pinParkingLight specifies pin used for parking light
     * @param pinHeadlight specifies pin used for headlight
     * @param pinRightBlinker specifies pin used for right blinker
     * @param pinLeftBlinker specifies pin used for left blinker
     * @param pinBackUpLight specifies pin used for back up  light
     * @param pinBrakeLight specifies pin used for brake light
     */
    SimpleRcCarLightController(int pPinParkingLight, int pPinHeadlight, int pPinRightBlinker, int pPinLeftBlinker,
                           int pPinBackUpLight, int pPinBrakeLight);

    /**
     * configures the required pins for OUTPUT.
     *
     * The method has to be called during setup
     */
    void setupPins(void);

    /**
     * allows to add a behavior for a specific light type. The SimpleRcCarLightControllor only support a behavior for the headlights.
     *
     * @param pLightType lights type where a behavior should be assigned
     * @param pLightSwitchBehaviour behavior, which influences the light switching
     */
    void addBehaviour(LightType_t pLightType, LightSwitchBehaviour *pLightSwitchBehaviour);

    /**
     *  sets the configured pins according to the light status
     */
    void loop(LightStatus_t pLightStatus);

private:
    // pin for parking lights
    int mPinParkingLight;

    // pin for headlights
    int mPinHeadlight;

    // pin for right blinker
    int mPinRightBlinker;

    // pin for left blinker
    int mPinLeftBlinker;

    // pin for backup lights
    int mPinBackUpLight;

    // pin or brake lights
    int mPinBrakeLight;

    LightSwitchBehaviour *mheadlightBehaviour;
};

#endif /* SIMPLERCCARLIGHTCONTROLLER_H_ */

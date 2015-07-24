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

#ifndef ABSTRACTRCCARLIGHTCONTROLLER_H_
#define ABSTRACTRCCARLIGHTCONTROLLER_H_

class LightSwitchBehaviour;

/**
 * Abstarct base class for RC car light output
 *
 * Is offers a public struct to store the light status of the supported light categories and provides a setup and loop
 * method.
 * Both methods (setupPins and loop) has to be overloaded by a concrete implementation class.
 */
class AbstractRcCarLightController
{
public:
    typedef enum
    {
        PARKING_LIGHT, HEADLIGHT, TAIL_LIGHT, RIGHT_BLINKER, LEFT_BLINKER, BACKUP_LIGHT, BRAKE_LIGHT
    } LightType_t;

    /**
     * struct to store several flags to represent the status of each light "category"
     */
    typedef struct
    {

        unsigned int parkingLight : 1; // 1 if parking lights are on, otherwise 0
        unsigned int headlight    : 1; // 1 if headlights are on, otherwise 0
        unsigned int rightBlinker : 1; // 1 if right blink lights are on, otherwise 0
        unsigned int leftBlinker  : 1; // 1 if left blink lights are on, otherwise 0
        unsigned int backUpLight  : 1; // 1 if back up light is on, otherwise 0
        unsigned int brakeLight   : 1; // 1 if brake light is on, 0 otherwise
    } CarLightsStatus_t;

    /**
     * constructor
     */
    AbstractRcCarLightController( void );

    /**
     * destructor
     */
    virtual ~AbstractRcCarLightController(void);

    /**
     * setup the pins.
     *
     * This method will be called from the general setup method of class RcCarLights.
     * Overload this method to configure the output pins as required.
     */
    virtual void setupPins(void) = 0;

    /**
     * allows to add a behaviour for a specifc light type.
     *
     * @param pLightType lights type where a behaviour should be assigned
     * @param pLightSwitchBehaviour behaviour, which influences the light switching
     */
    virtual void addBehaviour(LightType_t pLightType, LightSwitchBehaviour *pLightSwitchBehaviour) = 0;

    /**
     * provides the given light status to the output pins
     *
     * This method will be called by the general loop method of class RcCarLights.
     * Overload this method to implement the specific need to pass the light status
     * information to the required output pins of the arduino board.
     *
     * @param lightStatus current light status
     */
    virtual void loop(CarLightsStatus_t pLightStatus) = 0;
};


#endif /* ABSTRACTRCCARLIGHTCONTROLLER_H_ */

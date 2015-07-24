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

#include "Arduino.h"

#include "SimpleRcCarLightController.h"
#include "XenonLightSwitchBehaviour.h"

static float HEAD_LIGHT_ANALOG_WRITE_FACTOR = 2.55;
/**
 * constructor
 * @param pinParkingLight specifies pin used for parking light
 * @param pinHeadlight specifies pin used for headlight
 * @param pinRightBlinker specifies pin used for right blinker
 * @param pinLeftBlinker specifies pin used for left blinker
 * @param pinBackUpLight specifies pin used for back up  light
 * @param pinBrakeLight specifies pin used for brake light
 */
SimpleRcCarLightController::SimpleRcCarLightController(int pPinParkingLight,
        int pPinHeadlight, int pPinRightBlinker, int pPinLeftBlinker,
        int pPinBackUpLight, int pPinBrakeLight)
{
    mPinParkingLight = pPinParkingLight;
    mPinHeadlight = pPinHeadlight;
    mPinRightBlinker = pPinRightBlinker;
    mPinLeftBlinker = pPinLeftBlinker;
    mPinBackUpLight = pPinBackUpLight;
    mPinBrakeLight = pPinBrakeLight;

    mHeadlightBehaviour = NULL;
}

/**
 * configures the required pins for OUTPUT.
 *
 * The method has to be called during setup
 */
void SimpleRcCarLightController::setupPins(void)
{
    pinMode(mPinParkingLight, OUTPUT);
    pinMode(mPinHeadlight, OUTPUT);
    pinMode(mPinRightBlinker, OUTPUT);
    pinMode(mPinLeftBlinker, OUTPUT);
    pinMode(mPinBackUpLight, OUTPUT);
    pinMode(mPinBrakeLight, OUTPUT);
}

void SimpleRcCarLightController::addBehaviour(LightType_t pLightType,
        LightSwitchBehaviour *pLightSwitchBehaviour)
{
    // only for headlights a special behaviour was supported by this controller
    if (HEADLIGHT == pLightType)
    {
        mHeadlightBehaviour = pLightSwitchBehaviour;
    }
}

/**
 * Set set
 */
void SimpleRcCarLightController::setHeadlights(bool pHeadlightStatus)
{
    if (mHeadlightBehaviour)
    {
        mHeadlightBehaviour->setLightStatus(
                pHeadlightStatus ?
                        LightSwitchBehaviour::ON : LightSwitchBehaviour::OFF);
        analogWrite(mPinHeadlight,
                HEAD_LIGHT_ANALOG_WRITE_FACTOR
                        * mHeadlightBehaviour->getBrightness());
    }
    else
    {
        digitalWrite(mPinHeadlight, pHeadlightStatus ? HIGH : LOW);
    }
}

/**
 *  sets the configured pins according to the light status
 *
 *   @param pLightStatus current light status of all the lights
 */
void SimpleRcCarLightController::loop(CarLightsStatus_t pLightStatus)
{
    digitalWrite(mPinParkingLight, pLightStatus.parkingLight ? HIGH : LOW);

    setHeadlights(pLightStatus.headlight);

    digitalWrite(mPinBrakeLight, pLightStatus.brakeLight ? HIGH : LOW);
    digitalWrite(mPinBackUpLight, pLightStatus.backUpLight ? HIGH : LOW);

    digitalWrite(mPinRightBlinker, pLightStatus.rightBlinker ? HIGH : LOW);
    digitalWrite(mPinLeftBlinker, pLightStatus.leftBlinker ? HIGH : LOW);
}

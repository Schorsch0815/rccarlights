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

#include "SimpleRcCarLightController.h"
#include "XenonLightSwitchBehaviour.h"


/**
 * constructor
 * @param pinParkingLight specifies pin used for parking light
 * @param pinHeadlight specifies pin used for headlight
 * @param pinRightBlinker specifies pin used for right blinker
 * @param pinLeftBlinker specifies pin used for left blinker
 * @param pinBackUpLight specifies pin used for back up  light
 * @param pinBrakeLight specifies pin used for brake light
 */
SimpleRcCarLightController::SimpleRcCarLightController(int pPinParkingLight, int pPinHeadlight, int pPinRightBlinker,
                                               int pPinLeftBlinker, int pPinBackUpLight, int pPinBrakeLight)
{
    m_PinParkingLight = pPinParkingLight;
    m_PinHeadlight = pPinHeadlight;
    m_PinRightBlinker = pPinRightBlinker;
    m_PinLeftBlinker = pPinLeftBlinker;
    m_PinBackUpLight = pPinBackUpLight;
    m_PinBrakeLight = pPinBrakeLight;

    m_headlightBehaviour = NULL;
}

/**
 * configures the required pins for OUTPUT.
 *
 * The method has to be called during setup
 */
void SimpleRcCarLightController::setupPins(void)
{
    pinMode(m_PinParkingLight, OUTPUT);
    pinMode(m_PinHeadlight, OUTPUT);
    pinMode(m_PinRightBlinker, OUTPUT);
    pinMode(m_PinLeftBlinker, OUTPUT);
    pinMode(m_PinBackUpLight, OUTPUT);
    pinMode(m_PinBrakeLight, OUTPUT);
}

void SimpleRcCarLightController::addBehaviour(LightType_t pLightType, LightSwitchBehaviour *pLightSwitchBehaviour)
{
    // only for headlights a special behaviour was supported by this controller
    if (HEADLIGHT == pLightType)
    {
        m_headlightBehaviour = pLightSwitchBehaviour;
    }
}

/**
 *  sets the configured pins according to the light status
 *
 *   @param pLightStatus current light status of all the lights
 */
void SimpleRcCarLightController::loop(LightStatus_t pLightStatus)
{
    digitalWrite(m_PinParkingLight, pLightStatus.parkingLight ? HIGH : LOW);

    if (m_headlightBehaviour)
    {
        m_headlightBehaviour->setLightStatus(pLightStatus.headlight ? LightSwitchBehaviour::ON : LightSwitchBehaviour::OFF);
        analogWrite(m_PinHeadlight, m_headlightBehaviour->getBrightness() * 2.55);
    }
    else
    {
        digitalWrite(m_PinHeadlight, pLightStatus.headlight ? HIGH : LOW);
    }

    digitalWrite(m_PinBrakeLight, pLightStatus.brakeLight ? HIGH : LOW);
    digitalWrite(m_PinBackUpLight, pLightStatus.backUpLight ? HIGH : LOW);

    digitalWrite(m_PinRightBlinker, pLightStatus.rightBlinker ? HIGH : LOW);
    digitalWrite(m_PinLeftBlinker, pLightStatus.leftBlinker ? HIGH : LOW);
}

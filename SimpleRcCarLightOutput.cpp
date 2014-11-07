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

#include "SimpleRcCarLightOutput.h"

/**
 * constructor
 * @param pinParkingLight specifies pin used for parking light
 * @param pinHeadlight specifies pin used for headlight
 * @param pinRightBlinker specifies pin used for right blinker
 * @param pinLeftBlinker specifies pin used for left blinker
 * @param pinBackUpLight specifies pin used for back up  light
 * @param pinBrakeLight specifies pin used for brake light
 */
SimpleRcCarLightOutput::SimpleRcCarLightOutput(int pinParkingLight, int pinHeadlight, int pinRightBlinker,
                                               int pinLeftBlinker, int pinBackUpLight, int pinBrakeLight)
{
    m_PinParkingLight = pinParkingLight;
    m_PinHeadlight = pinHeadlight;
    m_PinRightBlinker = pinRightBlinker;
    m_PinLeftBlinker = pinLeftBlinker;
    m_PinBackUpLight = pinBackUpLight;
    m_PinBrakeLight = pinBrakeLight;
}

/**
 * configures the required pins for OUTPUT.
 *
 * The method has to be called during setup
 */
void SimpleRcCarLightOutput::setupPins(void)
{
    pinMode(m_PinParkingLight, OUTPUT);
    pinMode(m_PinHeadlight, OUTPUT);
    pinMode(m_PinRightBlinker, OUTPUT);
    pinMode(m_PinLeftBlinker, OUTPUT);
    pinMode(m_PinBackUpLight, OUTPUT);
    pinMode(m_PinBrakeLight, OUTPUT);
}

/**
 *  sets the configured pins according to the light status
 */
void SimpleRcCarLightOutput::loop(LightStatus_t lightStatus)
{
    digitalWrite(m_PinParkingLight, lightStatus.parkingLight ? HIGH : LOW);
    digitalWrite(m_PinHeadlight, lightStatus.headlight ? HIGH : LOW);
    digitalWrite(m_PinBrakeLight, lightStatus.brakeLight ? HIGH : LOW);
    digitalWrite(m_PinBackUpLight, lightStatus.backUpLight ? HIGH : LOW);

    digitalWrite(m_PinRightBlinker, lightStatus.rightBlinker ? HIGH : LOW);
    digitalWrite(m_PinLeftBlinker, lightStatus.leftBlinker ? HIGH : LOW);
}

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

#ifndef RcCarLights_h
#define RcCarLights_h

#include "RemoteControlCarAdapter.h"
#include "SimpleRcCarLightController.h"

class RcCarLights
{
public:

    RcCarLights();

    void setup(void);

    void loop(void);
private:
    void updateLightStatus();

    void setLights();

    void handleLightSwitch();
    void handleHeadlight();
    void handleBackUpLights();
    void handleBrakeLights();
    void handleBlinkerSwitch();
    void doBlinking();

private:

    // duration in msec to switch on/off lights
    const long SWITCH_DURATION_LIGHTS = 1000;

    // delay in msec to switch from Headlights to Parking light if car is stopped
    const long DIM_HEADLIGHTS_TO_PARKING_DELAY = 1500;

    // duration of blinker (on or off) in msec
    const long BLINKING_DURATION = 600;

    // delay in msec before blinking starts when stands still and steering is LEFT or RIGHT
    const long BLINKING_ON_DELAY = 300;

    // switch of delay for breaks to decrease flickering
    const long BREAK_LIGHTS_OFF_DELAY = 200;

    // acceleration threshold for brake lights
    const long BREAK_ACCELERATION_LEVEL = -20;

    // flag if light is switched is currently pressed (needed to suppress toggling the lights)
    bool m_isLightSwitchPressed;

    // timestamp when brake lights are switched on
    long m_BrakeLightsOnTimestamp;

    // last timestamp then blinker was switched on or off
    long m_LastBlinkTimestamp;

    // is true if blinker is switched on, false otherwise
    bool m_isBlinkingOn;

    RemoteControlCarAdapter m_RemoteControlCarAdapter;

    SimpleRcCarLightController m_LightController;

    AbstractRcCarLightController::LightStatus_t m_LightStatus;
};

#endif


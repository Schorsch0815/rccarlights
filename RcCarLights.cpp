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
//#include <Adafruit_NeoPixel.h>
#include "RcCarLights.h"

// pin 7 for pwm input
const int gPinThrottle = 7;

// pin 8 for pwm input
const int gPinSteering = 8;

const int gPinParkingLight = 2;
const int gPinHeadingLight = 3;
const int gPinBreakLight = 4;
const int gPinBackUpLight = 5;

const int gPinRightBlinker = 9;
const int gPinLeftBlinker = 10;

/**
 * Constructor
 */
RcCarLights::RcCarLights() :
        m_RemoteControlCarAdapter(gPinThrottle, gPinSteering), m_LightOutput(gPinParkingLight, gPinHeadingLight,
                                                                             gPinRightBlinker, gPinLeftBlinker,
                                                                             gPinBackUpLight, gPinBreakLight)
{
    // is true if parking light is on, false otherwise
    m_LightStatus.parkingLight = 0;

    m_isLightSwitchPressed = false;

    // is true if dimmed headlights are on, false otherwise
    m_LightStatus.headlight = 0;

    // is true if break light is on, false otherwise
    m_LightStatus.brakeLight = 0;

    // initialize timestamp when brakes switched on with 0
    m_BrakeLightsOnTimestamp = 0;

    // is true if back up light is on, false otherwise
    m_LightStatus.backUpLight = 0;

    // blinkers are off
    m_LightStatus.rightBlinker = 0;
    m_LightStatus.leftBlinker = 0;

    // last timestamp of blinker switch;
    m_LastBlinkTimestamp = 0;

    // no blinking at startup
    m_isBlinkingOn = false;
}

/**
 * configure the input and output pins of
 */
void RcCarLights::setup(void)
{
    Serial.begin(9600);
    m_RemoteControlCarAdapter.setupPins();

    m_LightOutput.setupPins();
}

/**
 * handles the light control:
 * 1. rerfresh the information read from RC
 * 2. calculates the new light status
 * 3. set the lights according to the light status
 */
void RcCarLights::loop(void)
{
    m_RemoteControlCarAdapter.refresh();

    updateLightStatus();

    Serial.print("\nLights : ");
    Serial.print(m_LightStatus.parkingLight);

    Serial.print("   Headlights : ");
    Serial.print(m_LightStatus.headlight);

    Serial.print("   BackUpLights : ");
    Serial.print(m_LightStatus.backUpLight);

    Serial.print("   Brakelights : ");
    Serial.print(m_LightStatus.brakeLight);

    Serial.print("   Blinking : ");
    Serial.print(m_isBlinkingOn);

    if (m_LightStatus.leftBlinker)
    {
        Serial.print(" (L)");
    }
    else if (m_LightStatus.rightBlinker)
    {
        Serial.print(" (R)");
    }
    else
    {
        Serial.print(" (  )");
    }

    Serial.print("  Throttle : ");
    Serial.print(m_RemoteControlCarAdapter.getThrottle());

    Serial.print("  Throttle Switch : ");
    Serial.print(m_RemoteControlCarAdapter.getThrottleSwitch());

    Serial.print("     Acceleration : ");
    Serial.print(m_RemoteControlCarAdapter.getAcceleration());

    Serial.print("  Steering : ");
    Serial.print(m_RemoteControlCarAdapter.getSteering());

    setLights();

#ifdef DEBUG
    Serial.print("\nThrottle : ");
    Serial.print(m_RemoteControlCarAdapter.getThrottle());

    Serial.print(" Throttle Switch : ");
    Serial.print(m_RemoteControlCarAdapter.getThrottleSwitch());
    Serial.print(" Duration : ");
    Serial.print(m_RemoteControlCarAdapter.getDurationOfThrottleSwitch());

    Serial.print(" [ms]; Steering : ");
    Serial.print(m_RemoteControlCarAdapter.getSteering());

    Serial.print(" Steering Switch : ");
    Serial.print(m_RemoteControlCarAdapter.getSteeringSwitch());
    Serial.print(" Duration : ");
    Serial.print(m_RemoteControlCarAdapter.getDurationOfSteeringSwitch());
    Serial.print(" [ms]; Steering : ");

    Serial.print("; Acceleration : ");
    Serial.print(m_RemoteControlCarAdapter.getAcceleration());
#endif
}

/**
 * updates the light status of the different lights
 */
void RcCarLights::updateLightStatus()
{
    // switch light (in general) on and off
    handleLightSwitch();

    // switch headlights on or off
    handleHeadlight();

    // switch on/off back-up lights
    handleBackUpLights();

    // swicth on off brake lights
    handleBrakeLights();

    // no blinker if steering is neutral
    handleBlinkerSwitch();

    // switch blinker on and off
    doBlinking();
}

/**
 * switches lights output pin(s) according to the current status
 */
void RcCarLights::setLights()
{
    m_LightOutput.loop(m_LightStatus);
}

/**
 * handles the general light switch
 *
 * The lights can be switched on and off by the so called throttle switch. When the throttle switch was set to FORWARD
 * for at least SWITCH_DURATION_LIGHTS, the lights will be switched on or off.
 */
void RcCarLights::handleLightSwitch()
{
    // switch light (in general) on and off
    if (RemoteControlCarAdapter::FORWARD == m_RemoteControlCarAdapter.getThrottleSwitch()
            && SWITCH_DURATION_LIGHTS < m_RemoteControlCarAdapter.getDurationOfThrottleSwitch())
    {
        if (!m_isLightSwitchPressed)
        {
            // toggle light status
            m_LightStatus.parkingLight ^= 1;
            m_isLightSwitchPressed = true;
        }
    }
    else
    {
        m_isLightSwitchPressed = false;
    }
}

/**
 * handles the headlights
 *
 * The headlights are turned on, when the car starts moving (forward or backward)
 */
void RcCarLights::handleHeadlight()
{
    // switch headlights on or off
    if (m_LightStatus.parkingLight)
    {
        // headlights will be switched on when car starts moving and the throttle switch is not FORWARD
        if (RemoteControlCarAdapter::FORWARD != m_RemoteControlCarAdapter.getThrottleSwitch())
        {
            // lights are switched on let's test for any movement
            if (RemoteControlCarAdapter::STOP != m_RemoteControlCarAdapter.getThrottle())
            {
                // switch the lights on, we are on the road
                m_LightStatus.headlight = 1;
            }
            else
            {
                // switch back to parking lights after delay
                if (DIM_HEADLIGHTS_TO_PARKING_DELAY < m_RemoteControlCarAdapter.getDurationOfThrottleSwitch())
                {
                    // look's we are parking: DIM THE LIGHTS...
                    m_LightStatus.headlight = 0;
                }
            }
        }
    }
    else
    {
        m_LightStatus.headlight = 0;
    }
}

/**
 * handles brake lights
 *
 * Brake lights will be switched if acceleration is below a specific threshold
 */
void RcCarLights::handleBrakeLights()
{
//    Serial.print( "    ");
//    Serial.print( BREAK_ACCELERATION_LEVEL );
//    Serial.print( "    ");
//    Serial.print( m_RemoteControlCarAdapter.getAcceleration() );
//    Serial.print( "  Cond  ");
//    Serial.print( BREAK_ACCELERATION_LEVEL > m_RemoteControlCarAdapter.getAcceleration() );

    // switch on off brake lights
    if (BREAK_ACCELERATION_LEVEL > m_RemoteControlCarAdapter.getAcceleration())
    {
        // brake lights on and store timestamp
        m_LightStatus.brakeLight = 1;
        m_BrakeLightsOnTimestamp = millis();
    }
    else
    {
        // if brake lights are on switch them off with a delay
        if (m_LightStatus.brakeLight && (BREAK_LIGHTS_OFF_DELAY < millis() - m_BrakeLightsOnTimestamp))
        {
            m_LightStatus.brakeLight = 0;
        }
    }
}

/**
 * handles backup lights
 *
 * Backup lights will be turned on when car moves backwards
 */
void RcCarLights::handleBackUpLights()
{
    // switch on/off back-up lights
    m_LightStatus.backUpLight = (RemoteControlCarAdapter::BACKWARD == m_RemoteControlCarAdapter.getThrottle());
}

/**
 * handles blinker switch
 *
 * The blinker will be turned on whenever the car did not move but the steering is left or right. When steering goes
 * back to neutral, the blinke is switched off. During movement no blinker will be switched on again.
 */
void RcCarLights::handleBlinkerSwitch()
{
    // no blinker if steering is neutral
    if (RemoteControlCarAdapter::NEUTRAL == m_RemoteControlCarAdapter.getSteering())
    {
        m_isBlinkingOn = false;
        m_LightStatus.leftBlinker = false;
        m_LightStatus.rightBlinker = false;
    }
    else
    {
        // handle blinker logic, blinker will be switched on if car stand still (throttle is STOP) for a while.
        if ((RemoteControlCarAdapter::STOP == m_RemoteControlCarAdapter.getThrottle())
                && (BLINKING_ON_DELAY < m_RemoteControlCarAdapter.getDurationOfThrottleSwitch()))
        {
            m_isBlinkingOn = true;
        }
    }
}

/**
 * flashes the blinker
 *
 * The blinker left or right will be switched on or off depending on the blink duration
 */
void RcCarLights::doBlinking()
{
    // switch blinker on and off
    if (m_isBlinkingOn)
    {
        if (RemoteControlCarAdapter::LEFT == m_RemoteControlCarAdapter.getSteering())
        {
            m_LightStatus.rightBlinker = false;
            if ((BLINKING_DURATION < millis() - m_LastBlinkTimestamp))
            {
                m_LightStatus.leftBlinker ^= 1;
                m_LastBlinkTimestamp = millis();
            }
        }
        else if (RemoteControlCarAdapter::RIGHT == m_RemoteControlCarAdapter.getSteering())
        {
            m_LightStatus.leftBlinker = false;
            if (BLINKING_DURATION < millis() - m_LastBlinkTimestamp)
            {
                m_LightStatus.rightBlinker ^= 1;
                m_LastBlinkTimestamp = millis();
            }
        }
    }
}


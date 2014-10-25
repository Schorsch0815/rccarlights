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
const int gPinLightBlinker = 10;

RcCarLights::RcCarLights() :
        m_RemoteControlCarAdapter(gPinThrottle, gPinSteering)
{
    // is true if parking light is on, false otherwise
    m_areParkingLightsOn = false;

    m_isLightSwitchPressed = false;

    // is true if dimmed headlights are on, false otherwise
    m_areHeadlightsOn = false;

    // is true if break light is on, false otherwise
    m_isBrakeLightOn = false;

    // initialize timestamp when brakes switched on with 0
    m_BrakeLightsOnTimestamp = 0;

    // is true if back up light is on, false otherwise
    m_isBackUpLightOn = false;

    // blinkers are off
    m_areRightBlinkLightsOn = false;
    m_areLeftBlinkLightsOn = false;

    // last timestamp of blinker switch;
    m_LastBlinkTimestamp = 0;

    // no blinking at startup
    m_isBlinkingOn = false;
}

void RcCarLights::setup(void)
{
    Serial.begin(9600);
    m_RemoteControlCarAdapter.setupPins();

    pinMode(gPinParkingLight,OUTPUT);
    pinMode(gPinHeadingLight,OUTPUT);
    pinMode(gPinBreakLight,OUTPUT);
    pinMode(gPinBackUpLight,OUTPUT);

    pinMode(gPinRightBlinker,OUTPUT);
    pinMode(gPinLightBlinker,OUTPUT);
}

void RcCarLights::loop(void)
{
    m_RemoteControlCarAdapter.refresh();

    updateLightStatus();

    Serial.print("\nLights : ");
    Serial.print(m_areParkingLightsOn);

    Serial.print("   Headlights : ");
    Serial.print(m_areHeadlightsOn);

    Serial.print("   BackUpLights : ");
    Serial.print(m_isBackUpLightOn);

    Serial.print("   Brakelights : ");
    Serial.print(m_isBrakeLightOn);

    Serial.print("   Blinking : ");
    Serial.print(m_isBlinkingOn);

    if (m_areLeftBlinkLightsOn)
    {
        Serial.print(" (L)");
    }
    else if (m_areRightBlinkLightsOn)
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

    setNeoPixelStatus();

    refreshNeoPixels();

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

void RcCarLights::setNeoPixelStatus()
{

}

void RcCarLights::refreshNeoPixels()
{
    digitalWrite( gPinParkingLight, m_areParkingLightsOn ? HIGH : LOW);
    digitalWrite( gPinHeadingLight,  m_areHeadlightsOn ? HIGH : LOW);
    digitalWrite( gPinBreakLight,  m_isBrakeLightOn ? HIGH : LOW);
    digitalWrite( gPinBackUpLight, m_isBackUpLightOn ? HIGH : LOW);

    digitalWrite( gPinRightBlinker, m_areRightBlinkLightsOn ? HIGH : LOW);
    digitalWrite( gPinLightBlinker, m_areLeftBlinkLightsOn ? HIGH : LOW);

}

void RcCarLights::handleLightSwitch()
{
    // switch light (in general) on and off
    if (RemoteControlCarAdapter::FORWARD == m_RemoteControlCarAdapter.getThrottleSwitch()
            && SWITCH_DURATION_HEADINGLIGHTS < m_RemoteControlCarAdapter.getDurationOfThrottleSwitch())
    {
        if (!m_isLightSwitchPressed)
        {
            // toggle light status
            m_areParkingLightsOn ^= 1;
            m_isLightSwitchPressed = true;
        }
    }
    else
    {
        m_isLightSwitchPressed = false;
    }
}

void RcCarLights::handleHeadlight()
{
    // switch headlights on or off
    if (m_areParkingLightsOn)
    {
        if (!m_isLightSwitchPressed)
        {
            // lights are switched on let's test for any movement
            if (RemoteControlCarAdapter::STOP != m_RemoteControlCarAdapter.getThrottle())
            {
                // switch the lights on, we are on the road
                m_areHeadlightsOn = true;
            }
            else
            {
                if (DIM_HEADLIGHTS_TO_PARKING_DELAY < m_RemoteControlCarAdapter.getDurationOfThrottleSwitch())
                {
                    // look's we are parking: DIM THE LIGHTS...
                    m_areHeadlightsOn = false;
                }
            }
        }
    }
    else
    {
        m_areHeadlightsOn = false;
    }
}

void RcCarLights::handleBrakeLights()
{
//    Serial.print( "    ");
//    Serial.print( BREAK_ACCELERATION_LEVEL );
//    Serial.print( "    ");
//    Serial.print( m_RemoteControlCarAdapter.getAcceleration() );
//    Serial.print( "  Cond  ");
//    Serial.print( BREAK_ACCELERATION_LEVEL > m_RemoteControlCarAdapter.getAcceleration() );

    // swicth on off brake lights
    if (BREAK_ACCELERATION_LEVEL > m_RemoteControlCarAdapter.getAcceleration())
    {
        // brake lights on and store timestamp
        m_isBrakeLightOn = true;
        m_BrakeLightsOnTimestamp = millis();
    }
    else
    {
        // if brake lights are on switch them off with a delay
        if (m_isBrakeLightOn && (BREAK_LIGHTS_OFF_DELAY < millis() - m_BrakeLightsOnTimestamp))
        {
            m_isBrakeLightOn = false;
        }
    }
}

void RcCarLights::handleBackUpLights()
{
    // switch on/off back-up lights
    m_isBackUpLightOn = (bool) (RemoteControlCarAdapter::BACKWARD == m_RemoteControlCarAdapter.getThrottle());
}

void RcCarLights::handleBlinkerSwitch()
{
    // no blinker if steering is neutral
    if (RemoteControlCarAdapter::NEUTRAL == m_RemoteControlCarAdapter.getSteering())
    {
        m_isBlinkingOn = false;
        m_areLeftBlinkLightsOn = false;
        m_areRightBlinkLightsOn = false;
    }
    else
    {
        // handle blinker logic
        if ((RemoteControlCarAdapter::STOP == m_RemoteControlCarAdapter.getThrottle())
                && (BLINKING_ON_DELAY < m_RemoteControlCarAdapter.getDurationOfThrottleSwitch()))
        {
            m_isBlinkingOn = true;
        }
    }
}

void RcCarLights::doBlinking()
{
    // switch blinker on and off
    if (m_isBlinkingOn)
    {
        if (RemoteControlCarAdapter::LEFT == m_RemoteControlCarAdapter.getSteering())
        {
            m_areRightBlinkLightsOn = false;
            if ((BLINKING_DURATION < millis() - m_LastBlinkTimestamp))
            {
                m_areLeftBlinkLightsOn ^= 1;
                m_LastBlinkTimestamp = millis();
            }
        }
        else if (RemoteControlCarAdapter::RIGHT == m_RemoteControlCarAdapter.getSteering())
        {
            m_areLeftBlinkLightsOn = false;
            if (BLINKING_DURATION < millis() - m_LastBlinkTimestamp)
            {
                m_areRightBlinkLightsOn ^= 1;
                m_LastBlinkTimestamp = millis();
            }
        }
    }
}


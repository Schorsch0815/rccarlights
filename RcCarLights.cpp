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
#include "XenonLightSwitchBehaviour.h"

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

#define THROTTLE_REVERSE    true
XenonLightSwitchBehaviour gHeadlightBehaviour;

/**
 * Constructor
 */
RcCarLights::RcCarLights() :
        mRemoteControlCarAdapter(gPinThrottle, THROTTLE_REVERSE, gPinSteering), mLightController(gPinParkingLight, gPinHeadingLight,
                                                                             gPinRightBlinker, gPinLeftBlinker,
                                                                             gPinBackUpLight, gPinBreakLight)
{
    // is true if parking light is on, false otherwise
    mLightStatus.parkingLight = 0;

    misLightSwitchPressed = false;

    // is true if dimmed headlights are on, false otherwise
    mLightStatus.headlight = 0;

    // is true if break light is on, false otherwise
    mLightStatus.brakeLight = 0;

    // initialize timestamp when brakes switched on with 0
    mBrakeLightsOnTimestamp = 0;

    // is true if back up light is on, false otherwise
    mLightStatus.backUpLight = 0;

    // blinkers are off
    mLightStatus.rightBlinker = 0;
    mLightStatus.leftBlinker = 0;

    // last timestamp of blinker switch;
    mLastBlinkTimestamp = 0;

    // no blinking at startup
    misBlinkingOn = false;
}

/**
 * configure the input and output pins of
 */
void RcCarLights::setup(void)
{
    Serial.begin(9600);
    mRemoteControlCarAdapter.setupPins();

    mLightController.setupPins();
    mLightController.addBehaviour(AbstractRcCarLightController::HEADLIGHT, &gHeadlightBehaviour);
    Serial.print("\nSetup.");

}

/**
 * handles the light control:
 * 1. rerfresh the information read from RC
 * 2. calculates the new light status
 * 3. set the lights according to the light status
 */
void RcCarLights::loop(void)
{
    mRemoteControlCarAdapter.refresh();

    updateLightStatus();
#ifdef DEBUG

    Serial.print("\nLights : ");
    Serial.print(mLightStatus.parkingLight);

    Serial.print("   Headlights : ");
    Serial.print(mLightStatus.headlight);

    Serial.print("   BackUpLights : ");
    Serial.print(mLightStatus.backUpLight);

    Serial.print("   Brakelights : ");
    Serial.print(mLightStatus.brakeLight);

    Serial.print("   Blinking : ");
    Serial.print(misBlinkingOn);

    if (mLightStatus.leftBlinker)
    {
        Serial.print(" (L)");
    }
    else if (mLightStatus.rightBlinker)
    {
        Serial.print(" (R)");
    }
    else
    {
        Serial.print(" (  )");
    }

    Serial.print("  Throttle : ");
    Serial.print(mRemoteControlCarAdapter.getThrottle());

    Serial.print("  Throttle Switch : ");
    Serial.print(mRemoteControlCarAdapter.getThrottleSwitch());

    Serial.print("     Acceleration : ");
    Serial.print(mRemoteControlCarAdapter.getAcceleration());

    Serial.print("  Steering : ");
    Serial.print(mRemoteControlCarAdapter.getSteering());
#endif

    setLights();

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
    mLightController.loop(mLightStatus);
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
    if (RemoteControlCarAdapter::FORWARD == mRemoteControlCarAdapter.getThrottleSwitch()
            && SWITCH_DURATION_LIGHTS < mRemoteControlCarAdapter.getDurationOfThrottleSwitch())
    {
        if (!misLightSwitchPressed)
        {
            // toggle light status
            mLightStatus.parkingLight ^= 1;
            misLightSwitchPressed = true;
        }
    }
    else
    {
        misLightSwitchPressed = false;
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
    if (mLightStatus.parkingLight)
    {
        // headlights will be switched on when car starts moving and the throttle switch is not FORWARD
        if (RemoteControlCarAdapter::FORWARD != mRemoteControlCarAdapter.getThrottleSwitch())
        {
            // lights are switched on let's test for any movement
            if (RemoteControlCarAdapter::STOP != mRemoteControlCarAdapter.getThrottle())
            {
                // switch the lights on, we are on the road
                mLightStatus.headlight = 1;
            }
            else
            {
                // switch back to parking lights after delay
                if (DIM_HEADLIGHTS_TO_PARKING_DELAY < mRemoteControlCarAdapter.getDurationOfThrottleSwitch())
                {
                    // look's we are parking: DIM THE LIGHTS...
                    mLightStatus.headlight = 0;
                }
            }
        }
    }
    else
    {
        mLightStatus.headlight = 0;
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
//    Serial.print( mRemoteControlCarAdapter.getAcceleration() );
//    Serial.print( "  Cond  ");
//    Serial.print( BREAK_ACCELERATION_LEVEL > mRemoteControlCarAdapter.getAcceleration() );

    // switch on off brake lights
    if (BREAK_ACCELERATION_LEVEL > mRemoteControlCarAdapter.getAcceleration())
    {
        // brake lights on and store timestamp
        mLightStatus.brakeLight = 1;
        mBrakeLightsOnTimestamp = millis();
    }
    else
    {
        // if brake lights are on switch them off with a delay
        if (mLightStatus.brakeLight && (BREAK_LIGHTS_OFF_DELAY < millis() - mBrakeLightsOnTimestamp))
        {
            mLightStatus.brakeLight = 0;
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
    mLightStatus.backUpLight = (RemoteControlCarAdapter::BACKWARD == mRemoteControlCarAdapter.getThrottle());
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
    if (RemoteControlCarAdapter::NEUTRAL == mRemoteControlCarAdapter.getSteering())
    {
        misBlinkingOn = false;
        mLightStatus.leftBlinker = false;
        mLightStatus.rightBlinker = false;
    }
    else
    {
        // handle blinker logic, blinker will be switched on if car stand still (throttle is STOP) for a while.
        if ((RemoteControlCarAdapter::STOP == mRemoteControlCarAdapter.getThrottle())
                && (BLINKING_ON_DELAY < mRemoteControlCarAdapter.getDurationOfThrottleSwitch()))
        {
            misBlinkingOn = true;
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
    if (misBlinkingOn)
    {
        if (RemoteControlCarAdapter::LEFT == mRemoteControlCarAdapter.getSteering())
        {
            mLightStatus.rightBlinker = false;
            if ((BLINKING_DURATION < millis() - mLastBlinkTimestamp))
            {
                mLightStatus.leftBlinker ^= 1;
                mLastBlinkTimestamp = millis();
            }
        }
        else if (RemoteControlCarAdapter::RIGHT == mRemoteControlCarAdapter.getSteering())
        {
            mLightStatus.leftBlinker = false;
            if (BLINKING_DURATION < millis() - mLastBlinkTimestamp)
            {
                mLightStatus.rightBlinker ^= 1;
                mLastBlinkTimestamp = millis();
            }
        }
    }
}

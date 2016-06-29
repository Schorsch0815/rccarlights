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

#include "XenonLightBehavior.h"

// using namespace rccarlights;

// pin 7 for pwm input
const int PIN_THROTTLE = 7;

// pin 8 for pwm input
const int PIN_STEERING = 8;

// pin 9 for pwm input
const int PIN_3RD_CHANNEL = 9;

const int PIN_PARKING_LIGHT = 2;
const int PIN_HEADLIGHT = 3;
const int PIN_NEO_PIXEL = 4;

const int PIN_EMERGENCY_LIGHT_SWITCH = 10;
const int PIN_SIRENE_SWITCH = 11;
const int PIN_TRAFFIC_BAR_SWITCH = 12;

const unsigned short MAX_LIGHT_GROUPS = 3;

#define DEBUG 1

#define THROTTLE_REVERSE true

using namespace rccarlights;

/**
 * Constructor
 */
RcCarLights::RcCarLights()
    : mRemoteControlCarAdapter( PIN_THROTTLE, THROTTLE_REVERSE, PIN_STEERING, PIN_3RD_CHANNEL )
    , mLightSwitchCondition( *this )
    , mLightSwitch( mLightSwitchCondition, SWITCH_LIGHT_IMPULSE_DURATION, SWITCH_LIGHT_COOL_DOWN )
    ,

    mHeadLightSwitchCondition( *this )
    , mHeadLightSwitch( mHeadLightSwitchCondition )
    ,

    mBreakSwitchCondition( *this )
    , mBreakSwitch( mBreakSwitchCondition )
    ,

    mBlinkerLeftSwitchCondition( *this )
    , mBlinkerLeftSwitch( mBlinkerLeftSwitchCondition )
    ,

    mBlinkerRightSwitchCondition( *this )
    , mBlinkerRightSwitch( mBlinkerRightSwitchCondition )
    ,

    mBackupLightSwitchCondition( *this )
    , mBackupLightSwitch( mBackupLightSwitchCondition )
    ,

    mSireneSwitchCondition( *this )
    , mSireneSwitch( mSireneSwitchCondition, SWITCH_SIREN_IMPULSE_DURATION, SWITCH_SIREN_COOL_DOWN )
    ,

    mEmergencySwitchCondition( *this )
    , mEmergencyLightBarSwitch( mEmergencySwitchCondition )
    ,

    mTrafficLightBarSwitchCondition( *this )
    , mTrafficLightBarSwitch( mTrafficLightBarSwitchCondition )
    ,

    mParkingLightGroup( PIN_PARKING_LIGHT, mLightSwitch )
    ,

    mHeadlightLightGroup( PIN_HEADLIGHT, mHeadLightSwitch )
    ,

    mCamaroLightGroup( PIN_NEO_PIXEL,
                       mLightSwitch,
                       mEmergencyLightBarSwitch, // use emergency lightbar switch to handle fog lights
                       mBreakSwitch,
                       mBlinkerLeftSwitch,
                       mBlinkerRightSwitch,
                       mBackupLightSwitch )
{
}

/**
 * configure the input and output pins of
 */
void RcCarLights::setup( void )
{
    Serial.begin( 9600 );
    mRemoteControlCarAdapter.setupPins();
#ifdef DEBUG
    Serial.print( "\nSetup." );
#endif

    mLightSwitch.setup();
    mHeadLightSwitch.setup();
    mBreakSwitch.setup();
    mBlinkerLeftSwitch.setup();
    mBlinkerRightSwitch.setup();
    mBackupLightSwitch.setup();

    mEmergencyLightBarSwitch.setup();
    mSireneSwitch.setup();
    mTrafficLightBarSwitch.setup();
}

/**
 * Refreshs all impacted switches and dependent LightGroups.
 */
void RcCarLights::loop( void )
{
    mRemoteControlCarAdapter.refresh();

    // Switch refresh
    mLightSwitch.refresh();
    mHeadLightSwitch.refresh();
    mBreakSwitch.refresh();
    mBlinkerLeftSwitch.refresh();
    mBlinkerRightSwitch.refresh();
    mBackupLightSwitch.refresh();

    mEmergencyLightBarSwitch.refresh();
    mSireneSwitch.refresh();
    mTrafficLightBarSwitch.refresh();

#ifdef DEBUG

    Serial.print( "\nLights : " );
    Serial.print( mLightSwitch.getState() );

    Serial.print( "   Headlights : " );
    Serial.print( mHeadLightSwitch.getState() );

    Serial.print( "   BackUpLights : " );
    Serial.print( mBackupLightSwitch.getState() );

    Serial.print( "   Brakelights : " );
    Serial.print( mBreakSwitch.getState() );

    Serial.print( "   Blinking Left: " );
    Serial.print( mBlinkerLeftSwitch.getState() );

    Serial.print( "   Blinking Right: " );
    Serial.print( mBlinkerRightSwitch.getState() );

    Serial.print( "  Throttle : " );
    Serial.print( mRemoteControlCarAdapter.getThrottle() );

    Serial.print( "  Throttle Switch : " );
    Serial.print( mRemoteControlCarAdapter.getThrottleSwitch() );

    Serial.print( "     Acceleration : " );
    Serial.print( mRemoteControlCarAdapter.getDirectionIndependentAcceleration() );

    Serial.print( "  Steering : " );
    Serial.print( mRemoteControlCarAdapter.getSteering() );

    Serial.print( "  Steering : " );
    Serial.print( mRemoteControlCarAdapter.getSteeringSwitch() );

    Serial.print( "  Light : " );
    Serial.print( mLightSwitch.getState() );

    Serial.print( "  Emergency light : " );
    Serial.print( mEmergencyLightBarSwitch.getState() );

    Serial.print( "  Traffic light : " );
    Serial.print( mTrafficLightBarSwitch.getState() );

    Serial.print( "  Siren : " );
    Serial.print( mSireneSwitch.getState() );
#endif

    digitalWrite( PIN_EMERGENCY_LIGHT_SWITCH, mEmergencyLightBarSwitch.getState() );
    digitalWrite( PIN_SIRENE_SWITCH, mSireneSwitch.getState() );
    digitalWrite( PIN_TRAFFIC_BAR_SWITCH, mTrafficLightBarSwitch.getState() );

    mParkingLightGroup.refresh();
    mHeadlightLightGroup.refresh();
    mCamaroLightGroup.refresh();
}

bool RcCarLights::HeadLightSwitchCondition::evaluate()
{
    // switch headlights on or off
    if ( mRcCarLights.getLightSwitchState() )
    {
        // headlights will be switched on when car starts moving and the throttle switch is not FORWARD
        if ( RemoteControlCarAdapter::FORWARD != mRcCarLights.mRemoteControlCarAdapter.getThrottleSwitch() )
        {
            // lights are switched on let's test for any movement
            if ( RemoteControlCarAdapter::STOP != mRcCarLights.mRemoteControlCarAdapter.getThrottle() )
            {
                // switch the lights on, we are on the road
                return mConditionValue = true;
            }
            else
            {
                // switch back to parking lights after delay
                if ( DIM_HEADLIGHTS_TO_PARKING_DELAY
                     < mRcCarLights.mRemoteControlCarAdapter.getDurationOfThrottleSwitch() )
                {
                    // look's we are parking: DIM THE LIGHTS...
                    mConditionValue = false;
                }
            }
        }
    }
    else
    {
        mConditionValue = false;
    }

    return mConditionValue;
}

bool RcCarLights::LightSwitchCondition::evaluate()
{
    return RemoteControlCarAdapter::FORWARD == mRcCarLights.mRemoteControlCarAdapter.getThrottleSwitch();
}

bool RcCarLights::BlinkerLeftSwitchCondition::evaluate()
{
    if ( RemoteControlCarAdapter::LEFT == mRcCarLights.mRemoteControlCarAdapter.getSteering()
         && ( RemoteControlCarAdapter::STOP == mRcCarLights.mRemoteControlCarAdapter.getThrottle() )
         && ( BLINKING_ON_DELAY < mRcCarLights.mRemoteControlCarAdapter.getDurationOfThrottleSwitch() ) )
    {
        return true;
    }
    return false;
}

bool RcCarLights::BlinkerRightSwitchCondition::evaluate()
{
    if ( RemoteControlCarAdapter::RIGHT == mRcCarLights.mRemoteControlCarAdapter.getSteering()
         && ( RemoteControlCarAdapter::STOP == mRcCarLights.mRemoteControlCarAdapter.getThrottle() )
         && ( BLINKING_ON_DELAY < mRcCarLights.mRemoteControlCarAdapter.getDurationOfThrottleSwitch() ) )
    {
        return true;
    }
    return false;
}

bool RcCarLights::BreakSwitchCondition::evaluate()
{

    if ( BREAK_ACCELERATION_LEVEL > mRcCarLights.mRemoteControlCarAdapter.getDirectionIndependentAcceleration() )
    {
        // brake lights on
        mConditionValue = true;
    }
    else
    {
        if ( mRcCarLights.getBreakLightOffDelay()
             < mRcCarLights.mRemoteControlCarAdapter.getDurationOfThrottleSwitch() )
        {
            mConditionValue = false;
        }
    }
    return mConditionValue;
}

unsigned long RcCarLights::getBreakLightOffDelay()
{
    return ( RemoteControlCarAdapter::STOP == mRemoteControlCarAdapter.getThrottle() )
               ? BREAK_LIGHTS_OFF_STAND_STILL_DELAY
               : BREAK_LIGHTS_OFF_DELAY;
}

bool RcCarLights::BackupLightSwitchCondition::evaluate()
{
    return ( RemoteControlCarAdapter::BACKWARD == mRcCarLights.mRemoteControlCarAdapter.getThrottle() );
}

bool RcCarLights::SireneSwitchCondition::evaluate()
{
    return ( Switch::ON == mRcCarLights.mEmergencyLightBarSwitch.getState() )
           && ( RemoteControlCarAdapter::LEFT == mRcCarLights.mRemoteControlCarAdapter.getSteeringSwitch() );
}

bool RcCarLights::EmergencySwitchCondition::evaluate()
{
    return ( 1500 > mRcCarLights.mRemoteControlCarAdapter.get3rdChannelValue() ) ? true : false;
}

bool RcCarLights::TrafficLightBarSwitchCondition::evaluate()
{
    return ( Switch::ON == mRcCarLights.mEmergencyLightBarSwitch.getState() )
           && ( RemoteControlCarAdapter::RIGHT == mRcCarLights.mRemoteControlCarAdapter.getSteeringSwitch() );
}

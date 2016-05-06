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
#include "rccarswitches/ConditionSwitch.h"
#include "rccarswitches/ImpulseSwitch.h"
#include "PinLightGroup.h"
#include "CamaroNeoPixelLightGroup.h"

class RcCarLights
{
public:

    RcCarLights();

    void setup( void );

    void loop( void );

private:

    void setupParkingLight();

    class RcCarCondition : public Condition
    {
    public:
        RcCarCondition( RcCarLights &pRcCarLights ) :
                mRcCarLights( pRcCarLights )
        {
        }
        virtual ~RcCarCondition()
        {
        }

    protected:
        RcCarLights & mRcCarLights;
    };

    class LightSwitchCondition : public RcCarCondition
    {
    public:
        LightSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights )
        {
        }
        virtual ~LightSwitchCondition()
        {
        }

        virtual bool evaluate();
    };

    class HeadLightSwitchCondition : public RcCarCondition
    {
    public:
        HeadLightSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights ), mConditionValue( false )
        {
        }
        virtual ~HeadLightSwitchCondition()
        {
        }

        virtual bool evaluate();
    private:
        bool mConditionValue;
    };

    class BlinkerRightSwitchCondition : public RcCarCondition
    {
    public:
        BlinkerRightSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights )
        {
        }
        virtual ~BlinkerRightSwitchCondition()
        {
        }

        virtual bool evaluate();
    };

    class BlinkerLeftSwitchCondition : public RcCarCondition
    {
    public:
        BlinkerLeftSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights )
        {
        }
        virtual ~BlinkerLeftSwitchCondition()
        {
        }

        virtual bool evaluate();
    };

    class BreakSwitchCondition : public RcCarCondition
    {
    public:
        BreakSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights ), mConditionValue( false )
        {
        }
        virtual ~BreakSwitchCondition()
        {
        }

        virtual bool evaluate();
    private:
        bool mConditionValue;
    };

    class BackupLightSwitchCondition : public RcCarCondition
    {
    public:
        BackupLightSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights )
        {
        }
        virtual ~BackupLightSwitchCondition()
        {
        }

        virtual bool evaluate();
    };


    class SireneSwitchCondition : public RcCarCondition
    {
    public:
        SireneSwitchCondition( RcCarLights &pRcCarLights ) :
                RcCarCondition( pRcCarLights )
        {
        }
        virtual ~SireneSwitchCondition()
        {
        }

        virtual bool evaluate();
    };

    class EmergencySwitchCondition : public RcCarCondition
    {
    public:
        EmergencySwitchCondition( RcCarLights &pRcCarLights ) :
            RcCarCondition( pRcCarLights )
        {
        }
        virtual ~EmergencySwitchCondition()
        {
        }

        virtual bool evaluate();
    };

    class TrafficLightBarSwitchCondition : public RcCarCondition
    {
    public:
        TrafficLightBarSwitchCondition( RcCarLights &pRcCarLights ) :
            RcCarCondition( pRcCarLights )
        {
        }
        virtual ~TrafficLightBarSwitchCondition()
        {
        }

        virtual bool evaluate();
    };

    void updateLightStatus();

    Switch::SwitchState_t getLightSwitchState() { return mLightSwitch.getState();}

    unsigned long getBreakLightOffDelay();

    // duration in msec to switch on/off lights
    static const long SWITCH_LIGHT_IMPULSE_DURATION = 1000;

    // cool down time in msec for switch on/off lights
    static const long SWITCH_LIGHT_COOL_DOWN = 100;

    // duration in msec to switch on/off lights
    static const long SWITCH_SIREN_IMPULSE_DURATION = 1000;

    // cool down time in msec for switch on/off lights
    static const long SWITCH_SIREN_COOL_DOWN = 100;

    // delay in msec to switch from Headlights to Parking light if car is stopped
    static const unsigned long DIM_HEADLIGHTS_TO_PARKING_DELAY = 1500;

    // duration of blinker (on or off) in msec
    static const long BLINKING_DURATION = 600;

    // delay in msec before blinking starts when stands still and steering is LEFT or RIGHT
    static const unsigned long BLINKING_ON_DELAY = 300;

    // switch of delay for breaks to decrease flickering
    static const unsigned long BREAK_LIGHTS_OFF_DELAY = 200;

    // switch of delay for breaks when stand still
    static const long BREAK_LIGHTS_OFF_STAND_STILL_DELAY = 1800;

    // switch of delay for breaks when stand still
    static const long BREAK_LIGHTS_OFF__STAND_STILL_DELAY = 700;

    // acceleration threshold for brake lights
    static const long BREAK_ACCELERATION_LEVEL = -20;

    static const unsigned long THRESHOLD_3RD_CHANNEL = 512;

    RemoteControlCarAdapter mRemoteControlCarAdapter;

    // switch, condition and group settings for general light/park light handling
    LightSwitchCondition mLightSwitchCondition;
    ImpulseSwitch mLightSwitch;

    // switch, condition and group settings for headligth handling
    HeadLightSwitchCondition mHeadLightSwitchCondition;
    ConditionSwitch mHeadLightSwitch;

    BreakSwitchCondition mBreakSwitchCondition;
    ConditionSwitch mBreakSwitch;

    BlinkerLeftSwitchCondition mBlinkerLeftSwitchCondition;
    ConditionSwitch mBlinkerLeftSwitch;

    BlinkerRightSwitchCondition mBlinkerRightSwitchCondition;
    ConditionSwitch mBlinkerRightSwitch;

    BackupLightSwitchCondition mBackupLightSwitchCondition;
    ConditionSwitch mBackupLightSwitch;

    // switch, condition and group settings for sirene handling
    SireneSwitchCondition mSireneSwitchCondition;
    ImpulseSwitch mSireneSwitch;

    // switch, condition and group settings for emergency handling
    EmergencySwitchCondition mEmergencySwitchCondition;
    ConditionSwitch mEmergencyLightBarSwitch;

    // switch, condition and group settings for traffic control handling
    TrafficLightBarSwitchCondition mTrafficLightBarSwitchCondition;
    ConditionSwitch mTrafficLightBarSwitch;

    rccarlights::PinLightGroup mParkingLightGroup;

    rccarlights::PinLightGroup mHeadlightLightGroup;

    // light group for neo pixel strip
    rccarlights::CamaroNeoPixelLightGroup mCamaroLightGroup;
};

#endif


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
#include "CamaroRcCarLightController.h"
#include "rccarswitches/ConditionSwitch.h"
#include "rccarswitches/ImpulseSwitch.h"

class RcCarLights
{
public:

    RcCarLights();

    void setup(void);

    void loop(void);

private:

    class LightSwitchCondition: public Condition
    {
    public:
        LightSwitchCondition(RcCarLights & pRcCarLights);
        virtual ~LightSwitchCondition();

        virtual bool evaluate();

    private:
        RcCarLights & mRcCarLights;
    };

    class SireneSwitchCondition: public Condition
    {
    public:
        SireneSwitchCondition(RcCarLights & pRcCarLights);
        virtual ~SireneSwitchCondition();

        virtual bool evaluate();

    private:
        RcCarLights & mRcCarLights;
    };

    class EmergencySwitchCondition: public Condition
    {
    public:
        EmergencySwitchCondition(RcCarLights & pRcCarLights);
        virtual ~EmergencySwitchCondition();

        virtual bool evaluate();

    private:
        RcCarLights & mRcCarLights;
    };

    class TrafficlightSwitchCondition: public Condition
    {
    public:
        TrafficlightSwitchCondition(RcCarLights & pRcCarLights);
        virtual ~TrafficlightSwitchCondition();

        virtual bool evaluate();

    private:
        RcCarLights & mRcCarLights;
    };

    void updateLightStatus();

    void setLights();

    void handleLightSwitch();
    void handleHeadlight();
    void handleBackUpLights();
    void handleBrakeLights();
    void handleBlinkerSwitch();
    void doBlinking();

    // duration in msec to switch on/off lights
    static const long SWITCH_LIGHT_DURATION = 1000;

    // cool down time in msec for switch on/off lights
    static const long SWITCH_LIGHT_COOL_DOWN = 100;

    // duration in msec to switch on/off lights
    static const long SWITCH_SIREN_DURATION = 1000;

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

    // flag if light is switched is currently pressed (needed to suppress toggling the lights)
    bool mIsLightSwitchPressed;

    // timestamp when brake lights are switched on
    long mBrakeLightsOnTimestamp;

    // last timestamp then blinker was switched on or off
    long mLastBlinkTimestamp;

    // is true if blinker is switched on, false otherwise
    bool mIsBlinkingOn;

    RemoteControlCarAdapter mRemoteControlCarAdapter;

    CamaroRcCarLightController mLightController;

    AbstractRcCarLightController::CarLightsStatus_t mLightStatus;

    LightSwitchCondition mLightSwitchCondition;
    ImpulseSwitch mLightSwitch;

    SireneSwitchCondition mSireneSwitchCondition;
    ImpulseSwitch mSireneSwitch;

    EmergencySwitchCondition mEmergencySwitchCondition;
    ConditionSwitch mEmergencyLightBarSwitch;

    TrafficlightSwitchCondition mTrafficLightSwitchCondition;
    ConditionSwitch mTrafficLightBarSwitch;
};

#endif


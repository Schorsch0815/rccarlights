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
 * Copyright: Jochen Schales 2016
 *
 * --------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "Arduino.h"
#include "ArduinoMockController.h"
#include "../RcCarLights.h"
#include "../RcCarLightsConstants.h"

#include <stdio.h>

class RcCarLightsTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        ArduinoMockController::getInstance().reset();
        ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );
    }

    virtual void TearDown()
    {
    }
};

TEST_F( RcCarLightsTest, InitializationTest )
{
    RcCarLights rcCarLights;

    rcCarLights.setup();

    rcCarLights.loop();

    EXPECT_EQ( 1, 1 );
}

TEST_F( RcCarLightsTest, LightSwitchOnTest )
{
    ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );

    RcCarLights rcCarLights;

    rcCarLights.setup();

    rcCarLights.loop();

    // now the parking light should be off
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );

    // trigger impuse switch for light
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    rcCarLights.loop();
    delay( 1000 );
    rcCarLights.loop();
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    rcCarLights.loop();

    // impusle length was sufficient now the parking light should be on
    EXPECT_EQ( 255, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );
}

TEST_F( RcCarLightsTest, LightSwitchOffTest )
{
    ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );

    RcCarLights rcCarLights;

    rcCarLights.setup();

    rcCarLights.loop();

    // trigger impuse switch for light
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    rcCarLights.loop();
    delay( 1000 );
    rcCarLights.loop();
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    rcCarLights.loop();

    // impusle length was sufficient now the parking light should be on
    EXPECT_EQ( 255, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );

    // trigger impuse switch for light
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    rcCarLights.loop();
    delay( 1000 );
    rcCarLights.loop();
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    rcCarLights.loop();

    // impusle length was sufficient now the parking light should be on
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );
}

TEST_F( RcCarLightsTest, LightSwitchImpuseTooShortTest )
{
    ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );

    RcCarLights rcCarLights;

    rcCarLights.setup();

    rcCarLights.loop();

    // now the parking light should be off
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );

    // trigger impuse switch for light
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    rcCarLights.loop();
    delay( 999 );
    rcCarLights.loop();

    // impulse was to short therefore light is still off
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );
}

TEST_F( RcCarLightsTest, HeadlightSwitchOnTest )
{
    ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );

    RcCarLights rcCarLights;

    rcCarLights.setup();

    rcCarLights.loop();

    // now the parking light should be off
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( PIN_PARKING_LIGHT ) );

    // trigger impuse switch for light
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    rcCarLights.loop();
    delay( 1000 );
    rcCarLights.loop();

    // increase speed to switch on head lights
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE - 400 );

    rcCarLights.loop();
    delay( 400 );
    rcCarLights.loop();
    EXPECT_EQ( 17, ArduinoMockController::getInstance().getAnalogValue( PIN_HEADLIGHT ) );

    delay( 1600 );
    rcCarLights.loop();

    EXPECT_EQ( 255, ArduinoMockController::getInstance().getAnalogValue( PIN_HEADLIGHT ) );
}

TEST_F( RcCarLightsTest, HeadlightSwitchOffTest )
{
    ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );

    RcCarLights rcCarLights;

    rcCarLights.setup();

    rcCarLights.loop();

    // trigger impuse switch for light
    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    rcCarLights.loop();
    delay( 1000 );
    rcCarLights.loop();

    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE - 400 );

    delay( 2000 );
    rcCarLights.loop();

    EXPECT_EQ( 255, ArduinoMockController::getInstance().getAnalogValue( PIN_HEADLIGHT ) );

    ArduinoMockController::getInstance().setPulseValue( PIN_THROTTLE, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    rcCarLights.loop();
    // wait 1500ms to turn light outomatic off when standing still
    delay( 1500 );
    rcCarLights.loop();
    // wait 500ms until lights are really off (XenonLightBehaviour)
    delay( 500 );
    rcCarLights.loop();

    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( PIN_HEADLIGHT ) );
}

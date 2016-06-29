/*
 * RemoteControlCarAdapterTest.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: ejocsch
 */

#include "gtest/gtest.h"
#include "Arduino.h"
#include "ArduinoMockController.h"
#include "../RemoteControlCarAdapter.h"

#include <stdio.h>

const int THROTTLE_PIN_ID = 0;
const int STEERING_PIN_ID = 1;
const int THIRD_CHANNEL_PIN_ID = 2;

const int ACCELERATION_MEASURE_INTERVAL
    = 200; // has to be in sync with RemoteControllerCarAdapter::ACCELERATION_MEASURE_INTERVAL

class RemoteControlCarAdapterTest : public ::testing::Test
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

    RemoteControlCarAdapter createAndInitializeAdapter();
    RemoteControlCarAdapter createInitializeAndCalibrateAdapter();

    void simulateAdapterIdle( RemoteControlCarAdapter &pAdapter, unsigned long pDelay );
    void accelerateTo( RemoteControlCarAdapter &pAdapter, unsigned long pThrottleValue );
};

RemoteControlCarAdapter RemoteControlCarAdapterTest::createAndInitializeAdapter()
{
    RemoteControlCarAdapter lAdapter( THROTTLE_PIN_ID, false, STEERING_PIN_ID, THIRD_CHANNEL_PIN_ID );
    lAdapter.setupPins();

    return lAdapter;
}

RemoteControlCarAdapter RemoteControlCarAdapterTest::createInitializeAndCalibrateAdapter()
{
    RemoteControlCarAdapter lAdapter = createAndInitializeAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    ArduinoMockController::getInstance().setPulseValue( STEERING_PIN_ID, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    ArduinoMockController::getInstance().setPulseValue( THIRD_CHANNEL_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE );

    lAdapter.calibrate();

    // now refresh and insert small delay to allow all following calculations runs as in real life.
    lAdapter.refresh();
    delay( ACCELERATION_MEASURE_INTERVAL );

    return lAdapter;
}

void RemoteControlCarAdapterTest::simulateAdapterIdle( RemoteControlCarAdapter &pAdapter, unsigned long pDelay )
{
    const static unsigned long DELAY_STEP_IN_MILLI_SEC = 100;

    for ( unsigned long i = 0; i < pDelay / DELAY_STEP_IN_MILLI_SEC; ++i )
    {
        delay( DELAY_STEP_IN_MILLI_SEC );
        pAdapter.refresh();
    }
    delay( pDelay % DELAY_STEP_IN_MILLI_SEC );
    pAdapter.refresh();
}

void RemoteControlCarAdapterTest::accelerateTo( RemoteControlCarAdapter &pAdapter, unsigned long pThrottleValue )
{
    pAdapter.refresh();
    delay( ACCELERATION_MEASURE_INTERVAL );
    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID, pThrottleValue );
    pAdapter.refresh();
}

TEST_F( RemoteControlCarAdapterTest, InitializationTest )
{
    RemoteControlCarAdapter lAdapter = createAndInitializeAdapter();

    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteering() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.get3rdChannelValue() );
    EXPECT_EQ( 0, lAdapter.getDirectionIndependentAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, CalibrationTest )
{
    RemoteControlCarAdapter lAdapter = createAndInitializeAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    ArduinoMockController::getInstance().setPulseValue( STEERING_PIN_ID, ArduinoMockController::PULSE_NEUTRAL_VALUE );
    ArduinoMockController::getInstance().setPulseValue( THIRD_CHANNEL_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE );

    lAdapter.calibrate();

    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteering() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfSteeringSwitch() );
    EXPECT_EQ( ArduinoMockController::PULSE_NEUTRAL_VALUE, lAdapter.get3rdChannelValue() );
    EXPECT_EQ( 0, lAdapter.getDirectionIndependentAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, CalibratedThrottleTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );
}

TEST_F( RemoteControlCarAdapterTest, CalibratedThrottleForwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID, ArduinoMockController::PULSE_MAX_VALUE );
    lAdapter.refresh();

    delay( 50 );
    lAdapter.refresh();

    EXPECT_EQ( RemoteControlCarAdapter::FORWARD, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::UNDEFINED_THROTTLE, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 50, lAdapter.getDurationOfThrottleSwitch() );
}

TEST_F( RemoteControlCarAdapterTest, CalibratedThrottleBackwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID, ArduinoMockController::PULSE_MIN_VALUE );
    lAdapter.refresh();

    delay( 50 );
    lAdapter.refresh();

    EXPECT_EQ( RemoteControlCarAdapter::BACKWARD, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::UNDEFINED_THROTTLE, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 50, lAdapter.getDurationOfThrottleSwitch() );
}

TEST_F( RemoteControlCarAdapterTest, ThrottleSwitchForwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE + 30 );
    lAdapter.refresh();

    delay( 50 );
    lAdapter.refresh();

    EXPECT_EQ( RemoteControlCarAdapter::FORWARD, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::FORWARD, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 50, lAdapter.getDurationOfThrottleSwitch() );
}

TEST_F( RemoteControlCarAdapterTest, ThrottleSwitchBackwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE - 30 );
    lAdapter.refresh();

    delay( 50 );
    lAdapter.refresh();

    EXPECT_EQ( RemoteControlCarAdapter::BACKWARD, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::BACKWARD, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 50, lAdapter.getDurationOfThrottleSwitch() );
}

TEST_F( RemoteControlCarAdapterTest, PositiveAccelerationForwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE + 30 );
    lAdapter.refresh();
    delay( ACCELERATION_MEASURE_INTERVAL );

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE + 100 );
    lAdapter.refresh();

    EXPECT_EQ( 70, lAdapter.getDirectionIndependentAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, PositiveAccelerationBackwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE - 33 );
    lAdapter.refresh();
    delay( ACCELERATION_MEASURE_INTERVAL );

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE - 100 );
    lAdapter.refresh();

    EXPECT_EQ( 67, lAdapter.getDirectionIndependentAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, NegativeAccelerationForwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE + 100 );
    lAdapter.refresh();
    delay( ACCELERATION_MEASURE_INTERVAL );

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE + 45 );
    lAdapter.refresh();

    EXPECT_EQ( -55, lAdapter.getDirectionIndependentAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, NegativeAccelerationBackwardTest )
{
    RemoteControlCarAdapter lAdapter = createInitializeAndCalibrateAdapter();

    ArduinoMockController::getInstance().setPulseValue( THROTTLE_PIN_ID,
                                                        ArduinoMockController::PULSE_NEUTRAL_VALUE - 100 );

    accelerateTo( lAdapter, ArduinoMockController::PULSE_NEUTRAL_VALUE - 99 );

    EXPECT_EQ( -1, lAdapter.getDirectionIndependentAcceleration() );
}

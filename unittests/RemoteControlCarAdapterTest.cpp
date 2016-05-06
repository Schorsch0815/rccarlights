/*
 * RemoteControlCarAdapterTest.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: ejocsch
 */

#include "gtest/gtest.h"
#include "ArduinoMockController.h"
#include "../RemoteControlCarAdapter.h"

class RemoteControlCarAdapterTest : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
        ArduinoMockController::getInstance().reset();
        ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );
    }

    virtual void TearDown() {}
};

TEST_F( RemoteControlCarAdapterTest, InitializationTest )
{
    RemoteControlCarAdapter lAdapter( 0, false, 1, 2 );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteering() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.get3rdChannelValue() );
    EXPECT_EQ( 0, lAdapter.getAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, CalibrationTest )
{
    RemoteControlCarAdapter lAdapter( 0, false, 1, 2 );

    lAdapter.setupPins();
    lAdapter.calibrate();
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteering() );
    EXPECT_EQ( RemoteControlCarAdapter::NEUTRAL, lAdapter.getSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfSteeringSwitch() );
    EXPECT_EQ( 0, lAdapter.get3rdChannelValue() );
    EXPECT_EQ( 0, lAdapter.getAcceleration() );
}

TEST_F( RemoteControlCarAdapterTest, ThrottleTest )
{
    RemoteControlCarAdapter lAdapter( 0, false, 1, 2 );
    lAdapter.setupPins();
    ArduinoMockController::getInstance().setPinValue( 0, 50 );
    lAdapter.calibrate();
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );

    lAdapter.refresh();
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottle() );
    EXPECT_EQ( RemoteControlCarAdapter::STOP, lAdapter.getThrottleSwitch() );
    EXPECT_EQ( 0, lAdapter.getDurationOfThrottleSwitch() );
}

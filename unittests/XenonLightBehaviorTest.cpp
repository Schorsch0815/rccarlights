/*--------------------------------------------------------------------
 * This file is part of the RcCarSwitches arduino library.
 *
 * RcCarSwitches is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RcCarSwitches is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RcCarSwitches.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright: Jochen Schales 2015
 *
 * --------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "Arduino.h"
#include "ArduinoMock.h"

#include "../XenonLightBehavior.h"

// Tests Switch setState and getState.
TEST(XenonLightBehaviorTest, SetGet) {
    XenonLightBehavior lLightBehavior;

    EXPECT_EQ(lLightBehavior.getLightStatus(),LightBehavior::OFF);

    lLightBehavior.setLightStatus(LightBehavior::ON);
    EXPECT_EQ(lLightBehavior.getLightStatus(),LightBehavior::ON);

    lLightBehavior.setLightStatus(LightBehavior::OFF);
    EXPECT_EQ(lLightBehavior.getLightStatus(),LightBehavior::OFF);
}

TEST(DISABLED_XenonLightBehaviorTest, GetBrightnessLightOnRealtimeTimer)
{
    unsigned long lStartTimeStamp;
    XenonLightBehavior lLightBehavior;

    EXPECT_EQ(lLightBehavior.getBrightness(),0);

    lStartTimeStamp = millis();
    lLightBehavior.setLightStatus(LightBehavior::ON);

    EXPECT_LE(lLightBehavior.getBrightness(),100);

    while(millis() -lStartTimeStamp < 60)
        lLightBehavior.getBrightness();

    EXPECT_NEAR(lLightBehavior.getBrightness(),100,5);

    while(millis() -lStartTimeStamp < 65)
        lLightBehavior.getBrightness();

    EXPECT_NEAR(lLightBehavior.getBrightness(),5,5);

    while(millis() -lStartTimeStamp < 250)
        lLightBehavior.getBrightness();

    EXPECT_NEAR(lLightBehavior.getBrightness(),10,5);

    while(millis() -lStartTimeStamp < 400)
        lLightBehavior.getBrightness();

    EXPECT_NEAR(lLightBehavior.getBrightness(),7,5);

    while(millis() -lStartTimeStamp < 1200)
        lLightBehavior.getBrightness();

    EXPECT_NEAR(lLightBehavior.getBrightness(),50,5);

    while(millis() -lStartTimeStamp < 2000)
        lLightBehavior.getBrightness();

    EXPECT_NEAR(lLightBehavior.getBrightness(),100,5);
}


TEST(XenonLightBehaviorTest, GetBrightnessLightOnManualTimer)
{
    ArduinoMockController::getInstance().setTimerMode(ArduinoMockController::MANUAL_TIMER_MODE);

    XenonLightBehavior lLightBehavior;

    ArduinoMockController::getInstance().setMilliSeconds(0);
    EXPECT_EQ(0,lLightBehavior.getBrightness());

    lLightBehavior.setLightStatus(LightBehavior::ON);
    EXPECT_EQ(100,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(60);
    EXPECT_EQ(100,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(63);
    EXPECT_EQ(43,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(65);
    EXPECT_EQ(5,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(250);
    EXPECT_EQ(10,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(400);
    EXPECT_EQ(7,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(600);
    EXPECT_EQ(18,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(2000);
    EXPECT_EQ(100,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setTimerMode(ArduinoMockController::REALTIME_TIMER_MODE);
}


TEST(XenonLightBehaviorTest, GetBrightnessLightOffManualTimer)
{
    ArduinoMockController::getInstance().setTimerMode(ArduinoMockController::MANUAL_TIMER_MODE);

    XenonLightBehavior lLightBehavior;

    ArduinoMockController::getInstance().setMilliSeconds(0);
    lLightBehavior.setLightStatus(LightBehavior::ON);
    ArduinoMockController::getInstance().setMilliSeconds(5000);
    EXPECT_EQ(100,lLightBehavior.getBrightness());

    lLightBehavior.setLightStatus(LightBehavior::OFF);
    EXPECT_EQ(100,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(5058);
    EXPECT_EQ(51,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(5060);
    EXPECT_EQ(50,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(5110);
    EXPECT_EQ(10,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(5500);
    EXPECT_EQ(0,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setMilliSeconds(7000);
    EXPECT_EQ(0,lLightBehavior.getBrightness());

    ArduinoMockController::getInstance().setTimerMode(ArduinoMockController::REALTIME_TIMER_MODE);
}


/*
 * CamaroLightGroupTest.cpp
 *
 *  Created on: Jul 8, 2016
 *      Author: ejocsch
 */

#include "gtest/gtest.h"
#include "Adafruit_NeoPixel.h"
#include "ArduinoMockController.h"
#include "../CamaroNeoPixelLightGroup.h"
#include "Switch.h"

#include <stdio.h>

const int NEOPIXEL_PIN_ID = 0;
const int NUMBER_OF_NEO_PIXELS = 14;

using namespace rccarlights;

class CamaroNeoPixelLightGroupTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        ArduinoMockController::getInstance().reset();
        ArduinoMockController::getInstance().setTimerMode( ArduinoMockController::MANUAL_TIMER_MODE );
    }

    virtual void TearDown()
    {
        destroyLightGroup();
    }

    void createLightGroup( Switch &pLightSwitch,
                           Switch &pFogLightSwitch,
                           Switch &pBreakSwitch,
                           Switch &pLeftBlinkerSwitch,
                           Switch &pRightBlinkerSwitch,
                           Switch &pBackupLightSwtch );

    void destroyLightGroup();

private:
    Adafruit_NeoPixel *mNeoPixelStrip;
    CamaroNeoPixelLightGroup *mNeoPixelGroup;
};

void CamaroNeoPixelLightGroupTest::createLightGroup( Switch &pLightSwitch,
                                                     Switch &pFogLightSwitch,
                                                     Switch &pBreakSwitch,
                                                     Switch &pLeftBlinkerSwitch,
                                                     Switch &pRightBlinkerSwitch,
                                                     Switch &pBackupLightSwtch )
{
    mNeoPixelStrip = new Adafruit_NeoPixel( NUMBER_OF_NEO_PIXELS, NEOPIXEL_PIN_ID, NEO_GRB + NEO_KHZ800 );
    mNeoPixelGroup = new CamaroNeoPixelLightGroup( *mNeoPixelStrip,
                                                   pLightSwitch,
                                                   pFogLightSwitch,
                                                   pBreakSwitch,
                                                   pLeftBlinkerSwitch,
                                                   pRightBlinkerSwitch,
                                                   pBackupLightSwtch );
}

void CamaroNeoPixelLightGroupTest::destroyLightGroup()
{
    delete mNeoPixelGroup;
    delete mNeoPixelStrip;
}

TEST_F( CamaroNeoPixelLightGroupTest, initialization )
{
    Switch lightSwitch;
    Switch fogLightSwitch;
    Switch breakSwitch;
    Switch leftBlinkerSwitch;
    Switch rightBlinkerSwitch;
    Switch backupLightSwtch;

    createLightGroup(
        lightSwitch, fogLightSwitch, breakSwitch, leftBlinkerSwitch, rightBlinkerSwitch, backupLightSwtch );

    EXPECT_NE(NULL, mNeoPixelGroup);
	)
}

TEST_F( CamaroNeoPixelLightGroupTest, wrongInitialization )
{
}

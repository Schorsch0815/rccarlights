/*
 * PinLightGroupTest.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: ejocsch
 */

#include "gtest/gtest.h"
#include "Arduino.h"

#include "../PinLightGroup.h"
#include "../LightBehavior.h"
#include "Switch.h"

using namespace rccarlights;

class SimpleLightBehavior : public LightBehavior
{
  public:
    /**
     * constructor
     */
    SimpleLightBehavior(){};

    /**
     * destructor
     */
    virtual ~SimpleLightBehavior(){};

    virtual unsigned short getBrightness( void );

  private:
    virtual void handlelightStatusChange( LightStatus_t pLightStatus );
};

unsigned short SimpleLightBehavior::getBrightness() { return ( ON == getLightStatus() ) ? 75 : 0; }

void SimpleLightBehavior::handlelightStatusChange( LightStatus_t pLightStatus ) {}

class PinLightGroupTest : public ::testing::Test
{
  protected:
    virtual void SetUp() { ArduinoMockController::getInstance().reset(); }

    virtual void TearDown() {}
};

// Tests Switch setState and getState.
TEST_F( PinLightGroupTest, PinLightGroupWoBehavior )
{
    Switch lSwitch;
    lSwitch.setup();

    PinLightGroup lLightGroup( 0, lSwitch );

    EXPECT_EQ( 0, lSwitch.getState() );
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( 0 ) );

    lSwitch.setState( Switch::ON );
    lLightGroup.refresh();
    EXPECT_EQ( 1, lSwitch.getState() );
    EXPECT_EQ( 255, ArduinoMockController::getInstance().getAnalogValue( 0 ) );
}

// Tests Switch setState and getState.
TEST_F( PinLightGroupTest, PinLightGroupWithBehavior )
{
    SimpleLightBehavior lBehavior;
    Switch lSwitch;
    lSwitch.setup();

    PinLightGroup lLightGroup( 0, lSwitch, &lBehavior );

    EXPECT_EQ( 0, lSwitch.getState() );
    EXPECT_EQ( 0, ArduinoMockController::getInstance().getAnalogValue( 0 ) );

    lSwitch.setState( Switch::ON );
    lLightGroup.refresh();
    EXPECT_EQ( 1, lSwitch.getState() );
    EXPECT_EQ( 191, ArduinoMockController::getInstance().getAnalogValue( 0 ) );
}

/*
 * PinLightGroup.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: ejocsch
 */

#include "PinLightGroup.h"

#include "LightBehavior.h"
#include "rccarswitches/Switch.h"

using namespace std;

namespace rccarlights
{

PinLightGroup::PinLightGroup( uint8_t pPin, Switch &pSwitch, LightBehavior *pLightBehavior )
    : mSwitch( pSwitch )
    , mLightBehavior( pLightBehavior )
    , mPin( pPin )
{
}

PinLightGroup::~PinLightGroup()
{
}

unsigned short PinLightGroup::getBrightness()
{
    if ( hasBehavior() )
    {
        return mLightBehavior->getBrightness();
    }
    else
    {
        return mSwitch.getState() ? 100 : 0;
    }
}

void PinLightGroup::refresh()
{
    if ( hasBehavior() )
    {
        mLightBehavior->setLightStatus( mSwitch.getState() ? LightBehavior::ON : LightBehavior::OFF );
    }
    analogWrite( mPin, 255 * getBrightness() / 100 );
}

} /* namespace rccarlights */

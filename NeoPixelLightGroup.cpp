/*
 * NeoPixelLightGroup.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: ejocsch
 */

#include "NeoPixelLightGroup.h"

#include "Adafruit_NeoPixel.h"

namespace rccarlights
{

NeoPixelLightGroup::NeoPixelLightGroup( Adafruit_NeoPixel *pNeoPixelStrip,
                                        uint16_t pNumLightSubGroups ) :
        mNeoPixelStrip( pNeoPixelStrip ),
        mNumLightSubGroups( pNumLightSubGroups )
{
    mLightSubGroupArray = (LightSubGroup_t *) malloc(
            sizeof(LightSubGroup_t) * mNumLightSubGroups );
}

NeoPixelLightGroup::~NeoPixelLightGroup()
{
    free( mLightSubGroupArray );
}

void NeoPixelLightGroup::addSubGroup( uint16_t pIdentifier,
                                      Switch &pSwitch,
                                      GetColorCallBack_t pColorCallBack,
                                      LightBehavior *pLightBehavior )
{
    if (pIdentifier >= mNumLightSubGroups)
        return;

    mLightSubGroupArray[pIdentifier].mSwitch = &pSwitch;
    mLightSubGroupArray[pIdentifier].mColorCallBack = pColorCallBack;
    mLightSubGroupArray[pIdentifier].mLightBehavior = pLightBehavior;
}

} /* namespace rccarlights */

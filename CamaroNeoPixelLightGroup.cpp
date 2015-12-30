/*
 * CamaroNeoPixelLightGroup.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: ejocsch
 */

#include "CamaroNeoPixelLightGroup.h"

namespace rccarlights
{

CamaroNeoPixelLightGroup::CamaroNeoPixelLightGroup( Adafruit_NeoPixel *pNeoPixelStrip ) :
        NeoPixelLightGroup( pNeoPixelStrip, MaxSubLightGroups )
{
    addSubGroup(MarkerLight,);
}

CamaroNeoPixelLightGroup::~CamaroNeoPixelLightGroup()
{
}

void CamaroNeoPixelLightGroup::refresh()
{
}

} /* namespace rccarlights */

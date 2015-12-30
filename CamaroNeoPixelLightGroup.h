/*
 * CamaroNeoPixelLightGroup.h
 *
 *  Created on: Dec 9, 2015
 *      Author: ejocsch
 */

#ifndef CAMARONEOPIXELLIGHTGROUP_H_
#define CAMARONEOPIXELLIGHTGROUP_H_

#include "NeoPixelLightGroup.h"

namespace rccarlights
{

class CamaroNeoPixelLightGroup : public NeoPixelLightGroup
{
public:
    typedef enum
    {
        MarkerLight = 0,
        BlinkerRight,
        BlinkerLeft,
        TailLight,
        BackUpLight,
        BreakLight,
        MaxSubLightGroups
    } CamaroSubLightGroups_t;

    CamaroNeoPixelLightGroup(Adafruit_NeoPixel *pNeoPixelStrip^);
    virtual ~CamaroNeoPixelLightGroup();

    virtual void refresh();

private:
};

} /* namespace rccarlights */

#endif /* CAMARONEOPIXELLIGHTGROUP_H_ */

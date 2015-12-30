/*
 * NeoPixelLightGroup.h
 *
 *  Created on: Nov 24, 2015
 *      Author: ejocsch
 */

#ifndef NEOPIXELLIGHTGROUP_H_
#define NEOPIXELLIGHTGROUP_H_

#include "LightGroup.h"

#include <Arduino.h>

class Adafruit_NeoPixel;

namespace rccarlights
{

class NeoPixelLightGroup : public LightGroup
{
public:
    typedef uint32_t (*GetColorCallBack_t)();

    NeoPixelLightGroup( Adafruit_NeoPixel *pNeoPixelStrip,
                        uint16_t pNumLightSubGroups );
    virtual ~NeoPixelLightGroup();

    void addSubGroup( uint16_t pIdentifier,
                      Switch &pSwitch,
                      GetColorCallBack_t pColorCallBack,
                      LightBehavior *pLightBehavior = NULL );
private:
    Adafruit_NeoPixel *mNeoPixelStrip;
    uint16_t mNumLightSubGroups;

    typedef struct
    {
        Switch *mSwitch;
        GetColorCallBack_t mColorCallBack;
        LightBehavior *mLightBehavior;
    } LightSubGroup_t;

    LightSubGroup_t *mLightSubGroupArray;
};

} /* namespace arduinomock */

#endif /* NEOPIXELLIGHTGROUP_H_ */

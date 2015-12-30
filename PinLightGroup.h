/*
 * PinLightGroup.h
 *
 *  Created on: Nov 23, 2015
 *      Author: ejocsch
 */

#ifndef PINLIGHTGROUP_H_
#define PINLIGHTGROUP_H_

#include "LightGroup.h"

#include "Arduino.h"

namespace rccarlights
{

class PinLightGroup : public LightGroup
{
public:
    PinLightGroup(uint8_t pPin, Switch &pSwitch, LightBehavior *pLightBehavior = NULL);
    virtual ~PinLightGroup();

    virtual void refresh();

private:

    unsigned short getBrightness();

    bool hasBehavior()
    {
        return (NULL == mLightBehavior);
    }

    Switch &mSwitch; ///< switch that controls the light group
    LightBehavior *mLightBehavior; ///< light behavior
    uint8_t mPin; ///< number of the corresponding arduino output pin
};

} /* namespace arduinomock */

#endif /* PINLIGHTGROUP_H_ */

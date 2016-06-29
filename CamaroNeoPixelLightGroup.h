/*
 * CamaroNeoPixelLightGroup.h
 *
 *  Created on: Dec 9, 2015
 *      Author: ejocsch
 */

#ifndef CAMARONEOPIXELLIGHTGROUP_H_
#define CAMARONEOPIXELLIGHTGROUP_H_

#include "LightGroup.h"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

class Switch;

namespace rccarlights
{

class CamaroNeoPixelLightGroup : public LightGroup
{
public:
    typedef enum
    {
        POSITION_MARKER_FRONT_LEFT_PIXEL = 0,
        FOG_LAMP_LEFT_PIXEL,
        BLINKER_FRONT_LEFT_PIXEL,
        BLINKER_FRONT_RIGHT_PIXEL,
        FOG_LAMP_RIGHT_PIXEL,
        POSITION_MARKER_FRONT_RIGHT_PIXEL,
        POSITION_MARKER_REAR_RIGHT_PIXEL,
        BACK_LIGHT_ONE_RIGHT_PIXEL,
        BACK_LIGHT_TWO_RIGHT_PIXEL,
        BACKUP_LIGHT_RIGHT_PIXEL,
        BACKUP_LIGHT_LEFT_PIXEL,
        BACK_LIGHT_TWO_LEFT_PIXEL,
        BACK_LIGHT_ONE_LEFT_PIXEL,
        POSITION_MARKER_REAR_LEFT_PIXEL
    } NeoPixelPosition_t;

    CamaroNeoPixelLightGroup( uint8_t pArduinoPin,
                              Switch &pLightSwitch,
                              Switch &pFogLightSwitch,
                              Switch &pBreakSwitch,
                              Switch &pLeftBlinkerSwitch,
                              Switch &pRightBlinkerSwitch,
                              Switch &pBackupLightSwtch );

    virtual ~CamaroNeoPixelLightGroup();

    virtual void refresh();

private:
    uint32_t getBackLightColor( Switch &pBlinkerSwitch );

    static const uint16_t NEO_PIXEL_COUNT = 14;

    static const uint32_t BLACK_COLOR;

    static const uint32_t FOG_LIGHT_COLOR;

    static const uint32_t BLINKER_FRONT_COLOR;

    static const uint32_t SIDE_MARKER_FRONT_COLOR;
    static const uint32_t SIDE_MARKER_FRONT_BLINKER_COLOR;

    static const uint32_t SIDE_MARKER_REAR_COLOR;
    static const uint32_t SIDE_MARKER_READ_BLINKER_COLOR;

    static const uint32_t BACKUP_LIGHT_COLOR;

    static const uint32_t BACK_LIGHT_BREAK_BLINKER_COLOR;
    static const uint32_t BACK_LIGHT_COLOR;

    Adafruit_NeoPixel mNeoPixelStrip;
    Switch &mLightSwitch;
    Switch &mFogLightSwitch;
    Switch &mBreakSwitch;
    Switch &mLeftBlinkerSwitch;
    Switch &mRightBlinkerSwitch;
    Switch &mBackupLightSwtch;
};

} /* namespace rccarlights */

#endif /* CAMARONEOPIXELLIGHTGROUP_H_ */

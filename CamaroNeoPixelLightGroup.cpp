/*
 * CamaroNeoPixelLightGroup.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: ejocsch
 */

#include "CamaroNeoPixelLightGroup.h"

#include "Switch.h"

namespace rccarlights
{

CamaroNeoPixelLightGroup::CamaroNeoPixelLightGroup( uint8_t pArduinoPin,
                                                    Switch &pLightSwitch,
                                                    Switch &pFogLightSwitch,
                                                    Switch &pBreakSwitch,
                                                    Switch &pLeftBlinkerSwitch,
                                                    Switch &pRightBlinkerSwitch,
                                                    Switch &pBackupLightSwtch ) :
        mNeoPixelStrip( NEO_PIXEL_COUNT, pArduinoPin, NEO_GRB + NEO_KHZ800 ),
        mLightSwitch( pLightSwitch ),
        mFogLightSwitch( pFogLightSwitch ),
        mBreakSwitch( pBreakSwitch ),
        mLeftBlinkerSwitch( pLeftBlinkerSwitch ),
        mRightBlinkerSwitch( pRightBlinkerSwitch ),
        mBackupLightSwtch( pBackupLightSwtch )
{
}

CamaroNeoPixelLightGroup::~CamaroNeoPixelLightGroup()
{
}

void CamaroNeoPixelLightGroup::refresh()
{
    // left blinker (position marker) front and rear
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_FRONT_LEFT_PIXEL,
            (Switch::ON == mLeftBlinkerSwitch.getState()) ?
                    SIDE_MARKER_FRONT_BLINKER_COLOR : SIDE_MARKER_FRONT_COLOR );
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_REAR_LEFT_PIXEL,
            (Switch::ON == mLeftBlinkerSwitch.getState()) ?
                    SIDE_MARKER_READ_BLINKER_COLOR : SIDE_MARKER_REAR_COLOR );

    // right blinker (position marker) front and rear
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_FRONT_RIGHT_PIXEL,
            (Switch::ON == mRightBlinkerSwitch.getState()) ?
                    SIDE_MARKER_FRONT_BLINKER_COLOR : SIDE_MARKER_FRONT_COLOR );
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_REAR_RIGHT_PIXEL,
            (Switch::ON == mRightBlinkerSwitch.getState()) ?
                    SIDE_MARKER_READ_BLINKER_COLOR : SIDE_MARKER_REAR_COLOR );

    // blinker front
    mNeoPixelStrip.setPixelColor( BLINKER_FRONT_LEFT_PIXEL,
            (Switch::ON == mLeftBlinkerSwitch.getState()) ? BLINKER_FRONT_COLOR : BLACK_COLOR );

    mNeoPixelStrip.setPixelColor( BLINKER_FRONT_RIGHT_PIXEL,
            (Switch::ON == mRightBlinkerSwitch.getState()) ? BLINKER_FRONT_COLOR : BLACK_COLOR );

    // back light, blinker and break light rear
    mNeoPixelStrip.setPixelColor( BACK_LIGHT_ONE_LEFT_PIXEL,
            getBackLightColor(mLeftBlinkerSwitch) );
    mNeoPixelStrip.setPixelColor( BACK_LIGHT_TWO_LEFT_PIXEL,
            getBackLightColor(mLeftBlinkerSwitch) );

    mNeoPixelStrip.setPixelColor( BACK_LIGHT_ONE_RIGHT_PIXEL,
            getBackLightColor( mRightBlinkerSwitch) );
    mNeoPixelStrip.setPixelColor( BACK_LIGHT_TWO_RIGHT_PIXEL,
            getBackLightColor( mRightBlinkerSwitch ) );

    // back up light
    mNeoPixelStrip.setPixelColor( BACKUP_LIGHT_LEFT_PIXEL,
            (Switch::ON == mBackupLightSwtch.getState()) ? BACKUP_LIGHT_COLOR : BLACK_COLOR );

    mNeoPixelStrip.setPixelColor( BACKUP_LIGHT_RIGHT_PIXEL,
            (Switch::ON == mBackupLightSwtch.getState()) ? BACKUP_LIGHT_COLOR : BLACK_COLOR );

    mNeoPixelStrip.show();

}

/**
 * Determine the current color of the back lights. It depends on switch state of passed blinker switch.
 *
 * @param pBlinkerSwitch current blinker switch
 *
 * @return return color of blinker depending on the blinker switch state
 */
uint32_t CamaroNeoPixelLightGroup::getBackLightColor( Switch &pBlinkerSwitch )
{
    if (pBlinkerSwitch.getState() ^ mBreakSwitch.getState())
    {
        return BACK_LIGHT_BREAK_BLINKER_COLOR;
    }
    if ((pBlinkerSwitch.getState() && mBreakSwitch.getState())
            || (mLightSwitch.getState() && mBreakSwitch.getInvertedState() && !pBlinkerSwitch.getState()))
    {
        return BACK_LIGHT_COLOR;
    }
    return BLACK_COLOR;
}

} /* namespace rccarlights */

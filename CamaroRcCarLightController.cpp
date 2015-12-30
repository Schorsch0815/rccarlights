/*
 * CamaroRcCarLightController.cpp
 *
 *  Created on: 08.01.2015
 *      Author: Jochen
 */
#if 0
#include "Arduino.h"

#include "CamaroRcCarLightController.h"

#include "XenonLightBehavior.h"
#include "PinLightGroup.h"
#include "NeoPixelLightGroup.h"

using namespace rccarlights;

#define NEO_PIXEL_COUNT 14

typedef enum
{
    POSITION_MARKER_FRONT_LEFT_PIXEL = 0,
    FOG_LAMP_LEFT_PIXEL,
    BLINKER_FRONT_LEFT,
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

const uint32_t BLACK_COLOR = Adafruit_NeoPixel::Color( 0, 0, 0 );

const uint32_t FOG_LIGHT_COLOR = Adafruit_NeoPixel::Color( 96, 96, 96 );

const uint32_t BLINKER_FRONT_COLOR = Adafruit_NeoPixel::Color( 128, 128, 0 );

const uint32_t SIDE_MARKER_FRONT_COLOR = Adafruit_NeoPixel::Color( 32, 32, 0 );
const uint32_t SIDE_MARKER_FRONT_BLINKER_COLOR = Adafruit_NeoPixel::Color( 128,
        128, 0 );

const uint32_t SIDE_MARKER_REAR_COLOR = Adafruit_NeoPixel::Color( 32, 0, 0 );
const uint32_t SIDE_MARKER_READ_BLINKER_COLOR = Adafruit_NeoPixel::Color( 128,
        0, 0 );

const uint32_t BACKUP_LIGHT_COLOR = Adafruit_NeoPixel::Color( 64, 48, 16 );

const uint32_t BACK_LIGHT_BREAK_BLINKER_COLOR = Adafruit_NeoPixel::Color( 255,
        0, 0 );
const uint32_t BACK_LIGHT_COLOR = Adafruit_NeoPixel::Color( 48, 0, 0 );


/**
 * constructor
 * @param pPinParkingLight specifies pin used for parking light
 * @param pPinHeadlight specifies pin used for headlight
 * @param pPinNeoPixel specifies pin used for NeoPixel signal
 */
CamaroRcCarLightController::CamaroRcCarLightController( int pPinParkingLight,
                                                        int pPinHeadlight,
                                                        int pPinNeoPixel ) :
        AbstractLightController( 0 ),
        mPinParkingLight( pPinParkingLight ),
        mPinHeadlight( pPinHeadlight ),
        mNeoPixelStrip( NEO_PIXEL_COUNT, pPinNeoPixel,
        NEO_GRB + NEO_KHZ800 )
{

}

/**
 * destructor
 */
CamaroRcCarLightController::~CamaroRcCarLightController()
{
}

#if 0
/**
 * configures the required pins for OUTPUT and initialize the NeoPixel stip
 *
 * The method has to be called during setup of the arduino sketch
 */
void CamaroRcCarLightController::setupPins( void )
{
    pinMode( mPinParkingLight, OUTPUT );
    pinMode( mPinHeadlight, OUTPUT );

    mNeoPixelStrip.begin();
    mNeoPixelStrip.show();
}

/**
 * allows to add a specific switch on/off behavior for a light type. At the moment only the head lights are supported.
 * @param pLightType light type
 * @param pLightSwitchBehaviour the new switch behavior of the specified light type.
 */
void CamaroRcCarLightController::addBehaviour( LightType_t pLightType,
                                               LightBehavior *pLightSwitchBehaviour )
{
    // only for headlights a special behaviour was supported by this controller
    if (HEADLIGHT == pLightType)
    {
        mHeadlightBehaviour = pLightSwitchBehaviour;
    }
}

/**
 *  sets the configured pins according to the light status
 *
 *   @param pLightStatus current light status of all the lights
 */
void CamaroRcCarLightController::loop( CarLightsStatus_t pLightStatus )
{
    // angle eyes
    digitalWrite( mPinParkingLight, pLightStatus.parkingLight ? HIGH : LOW );

    // headlights
    if (mHeadlightBehaviour)
    {
        mHeadlightBehaviour->setLightStatus(
                pLightStatus.headlight ?
                        LightBehavior::ON : LightBehavior::OFF );
        analogWrite( mPinHeadlight,
                2.55 * mHeadlightBehaviour->getBrightness() );
    }
    else
    {
        digitalWrite( mPinHeadlight, pLightStatus.headlight ? HIGH : LOW );
    }

    // position lights are always on or blink
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_FRONT_LEFT_PIXEL,
            pLightStatus.leftBlinker ?
                    SIDE_MARKER_FRONT_BLINKER_COLOR : SIDE_MARKER_FRONT_COLOR );
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_FRONT_RIGHT_PIXEL,
            pLightStatus.rightBlinker ?
                    SIDE_MARKER_FRONT_BLINKER_COLOR : SIDE_MARKER_FRONT_COLOR );
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_REAR_LEFT_PIXEL,
            pLightStatus.leftBlinker ?
                    SIDE_MARKER_READ_BLINKER_COLOR : SIDE_MARKER_REAR_COLOR );
    mNeoPixelStrip.setPixelColor( POSITION_MARKER_REAR_RIGHT_PIXEL,
            pLightStatus.rightBlinker ?
                    SIDE_MARKER_READ_BLINKER_COLOR : SIDE_MARKER_REAR_COLOR );

    // blinker front
    mNeoPixelStrip.setPixelColor( BLINKER_FRONT_LEFT,
            pLightStatus.leftBlinker ? BLINKER_FRONT_COLOR : BLACK_COLOR );

    mNeoPixelStrip.setPixelColor( BLINKER_FRONT_RIGHT_PIXEL,
            pLightStatus.rightBlinker ? BLINKER_FRONT_COLOR : BLACK_COLOR );

    // back light, blinker and break light rear
    mNeoPixelStrip.setPixelColor( BACK_LIGHT_ONE_LEFT_PIXEL,
            getBackLightColor( pLightStatus, pLightStatus.leftBlinker ) );
    mNeoPixelStrip.setPixelColor( BACK_LIGHT_TWO_LEFT_PIXEL,
            getBackLightColor( pLightStatus, pLightStatus.leftBlinker ) );

    mNeoPixelStrip.setPixelColor( BACK_LIGHT_ONE_RIGHT_PIXEL,
            getBackLightColor( pLightStatus, pLightStatus.rightBlinker ) );
    mNeoPixelStrip.setPixelColor( BACK_LIGHT_TWO_RIGHT_PIXEL,
            getBackLightColor( pLightStatus, pLightStatus.rightBlinker ) );

    // back up light
    mNeoPixelStrip.setPixelColor( BACKUP_LIGHT_LEFT_PIXEL,
            pLightStatus.backUpLight ? BACKUP_LIGHT_COLOR : BLACK_COLOR );

    mNeoPixelStrip.setPixelColor( BACKUP_LIGHT_RIGHT_PIXEL,
            pLightStatus.backUpLight ? BACKUP_LIGHT_COLOR : BLACK_COLOR );

    mNeoPixelStrip.show();
}
#endif

/**
 * determine the current color of the back lights. It depends on parking light, brake light and blinking status
 * @param pLightStatus current light status
 * @param pBlink is true if a blink light is currently on otherwise false
 * @return
 */
uint32_t CamaroRcCarLightController::getBackLightColor( CarLightsStatus_t pLightStatus,
                                                        bool pBlink )
{
    if (pLightStatus.brakeLight ^ pBlink)
    {
        return BACK_LIGHT_BREAK_BLINKER_COLOR;
    }
    if ((pLightStatus.brakeLight && pBlink)
            || (pLightStatus.parkingLight && !pLightStatus.brakeLight && !pBlink))
    {
        return BACK_LIGHT_COLOR;
    }
    return BLACK_COLOR;
}

#endif

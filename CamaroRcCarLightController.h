/*
 * CamaroRcCarLightController.h
 *
 *  Created on: 08.01.2015
 *      Author: Jochen
 */
#if 0
#ifndef CAMARORCCARLIGHTCONTROLLER_H_
#define CAMARORCCARLIGHTCONTROLLER_H_

#include "AbstractLightController.h"
#include "Adafruit_NeoPixel.h"

class CamaroRcCarLightController : public AbstractLightController
{
public:
    /**
     * Constructor
     * @param pPinParkingLight specifies pin used for parking light
     * @param pPinHeadlight specifies pin used for headlight
     * @param pPinNeoPixel specifies pin used for NeoPixel signal
     */
    CamaroRcCarLightController( int pPinParkingLight,
                                int pPinHeadlight,
                                int pPinNeoPixel );

    /**
     * destructor
     */
    virtual ~CamaroRcCarLightController();

#if 0
    /**
     * configures the required pins for OUTPUT.
     *
     * The method has to be called during setup
     */
    void setupPins(void);

    /**
     * allows to add a behavior for a specific light type. The Controller only support a behavior for the headlights.
     *
     * @param pLightType lights type where a behavior should be assigned
     * @param pLightSwitchBehaviour behavior, which influences the light switching
     */
    void addBehaviour(LightType_t pLightType, LightBehavior *pLightSwitchBehaviour);

    /**
     *  sets the configured pins according to the light status
     */
    void loop(CarLightsStatus_t pLightStatus);
#endif

private:
    /**
     * determine the current color of the back lights. It depends on parking light, brake light and blinking status
     * @param pLightStatus current light status
     * @param pBlink is true if a blink light is currently on otherwise false
     * @return
     */
    uint32_t getBackLightColor( CarLightsStatus_t pLightStatus, bool pBlink );
private:
    // pin for parking lights
    int mPinParkingLight;

    // pin for headlights
    int mPinHeadlight;

    // NeoPixel strip for all other lights
    Adafruit_NeoPixel mNeoPixelStrip;
};

#endif /* CAMARORCCARLIGHTCONTROLLER_H_ */
#endif


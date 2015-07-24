/*--------------------------------------------------------------------
 * This file is part of the RcCarLights arduino application.
 *
 * RcCarLights is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RcCarLights is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RcCarLights.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright: Jochen Schales 2014
 *
 * --------------------------------------------------------------------*/


#ifndef XENONLIGHTSWITCHBEHAVIOUR_H_
#define XENONLIGHTSWITCHBEHAVIOUR_H_

#include "LightSwitchBehaviour.h"

/**
 * class to change the bahaviour of the light switching and try to simulate a Xenon light, with flickering on startup and slow cooldown.
 * The brightness characteristics of a xenon light will be generated by interpolating over few values when switching on or off.
 */
class XenonLightSwitchBehaviour : public LightSwitchBehaviour
{
public:
    /**
     * constructor
     */
    XenonLightSwitchBehaviour();

    /**
     * destructor
     */
    virtual ~XenonLightSwitchBehaviour();

    /**
     * sets the lights status of the xenon light switch
     *
     * @param pLightStatus desired status of the controlled light
     */
    virtual void setLightStatus( LightStatus_t pLightStatus );

    /**
     * This method implements the startup flickering and the cooldown of a xenon light. It has to be call from the main loop and should
     * be called as often as possible to get smooth brightness changes.
     *
     * @return the brightness of light
     */
    virtual unsigned short getBrightness( void );

private:
    /**
     * timestamp used to store the timestamp when the light status changes
     */
    long _switchTimestamp;

    /**
     * index used for internal calculation of the interpolation algorithm to simulate the flickering and cooldown
     */
    short _interpolationIndex;
};

#endif /* XENONLIGHTSWITCHBEHAVIOUR_H_ */
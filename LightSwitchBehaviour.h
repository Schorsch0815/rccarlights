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

#ifndef LIGHTSWITCHBEHAVIOUR_H_
#define LIGHTSWITCHBEHAVIOUR_H_

/**
 * class to change the behavior of the light switching. The base class will react as a normal switch.
 * But subclassing allows to implement different behaviors like dim on/off or flicking like xenon lights.
 */
class LightSwitchBehaviour
{
public:
    /**
     * enumeration type for the light status (ON/OFF)
     */
    typedef enum
    {
        OFF = 0, ON
    } LightStatus_t;

    /**
     * constructor
     */
    LightSwitchBehaviour();

    /**
     * destructor
     */
    virtual ~LightSwitchBehaviour();

    /**
     * sets the lights status of the behavior. Can be overloaded by subclass to ad additional functionality. Every subclass has to call {@link #setLightStatusSelf}.
     *
     * @param pLightStatus desired status of the controlled light
     */
    virtual void setLightStatus( LightStatus_t pLightStatus ) = 0;

    /**
     *
     * @return the current light status
     */
    LightStatus_t getLightStatus( void )
    {
        return mLightStatus;
    }

    /**
     * @return the bright of the lights controlled by the behavior
     */
    virtual unsigned short getBrightness( void ) = 0;

protected:

    /**
     * sets the light status internally
     *
     * @param pLightStatus desired status of the controlled light
     */
    inline void setLightStatusSelf(LightStatus_t pLightStatus)
    {
        mLightStatus = pLightStatus;
    }

private:

    /**
     * light status
     */
    LightStatus_t mLightStatus;

};

#endif /* LIGHTSWITCHBEHAVIOUR_H_ */

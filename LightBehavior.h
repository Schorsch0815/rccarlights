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

#ifndef LIGHTBEHAVIOR_H_
#define LIGHTBEHAVIOR_H_

/**
 * class to change the behavior of the light dimming. The base class ha to be enhanced by a subclass to implement
 * a specialized behaviors like dim on/off or flicking like xenon lights.
 */
class LightBehavior
{
  public:
    /**
     * enumeration type for the light status (ON/OFF)
     */
    typedef enum
    {
        OFF = 0,
        ON
    } LightStatus_t;

    /**
     * constructor
     */
    LightBehavior();

    /**
     * destructor
     */
    virtual ~LightBehavior();

    /**
     * sets the lights status of the behavior.
     *
     * @param pLightStatus desired status of the controlled light
     */
    void setLightStatus( LightStatus_t pLightStatus );

    /**
     *
     * @return the current light status
     */
    LightStatus_t getLightStatus( void ) { return mLightStatus; }

    /**
     * @return the bright of the lights controlled by the behavior
     */
    virtual unsigned short getBrightness( void ) = 0;

  protected:
    /**
     * allows to prepare light status change in subclass. Will be called when setLightStatus was called and status has
     *changed.
     *
     * @param pLightStatus desired status of the controlled light
     */
    virtual void handlelightStatusChange( LightStatus_t pLightStatus ) = 0;

  private:
    /**
     * light status
     */
    LightStatus_t mLightStatus;
};

#endif /* LIGHTBEHAVIOR_H_ */

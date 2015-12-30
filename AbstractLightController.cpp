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

#include <cstdio>
#include <memory>

#include "AbstractLightController.h"

AbstractLightController::AbstractLightController( unsigned short pMaxLightGroups ) :
        mLightGroupArray( NULL ),
        mLightGroupCount( 0 ),
        mMaxLightGroups( pMaxLightGroups )
{
    mLightGroupArray = (LightGroup **) malloc(
            pMaxLightGroups * sizeof(LightGroup *) );
}


AbstractLightController::~AbstractLightController( void )
{
}


void AbstractLightController::addLightGroup( LightGroup &pLightGroup )
{
    if (mLightGroupCount < mMaxLightGroups)
    {
        mLightGroupArray[mLightGroupCount++] = &pLightGroup;
    }
}


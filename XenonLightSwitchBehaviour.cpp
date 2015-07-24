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
#include "Arduino.h"

#include "XenonLightSwitchBehaviour.h"

/**
 * number of interpolation points for switching on
 */
#define NUM_XENON_ON_INTERPOLATION_STEPS   6

typedef struct
{
    float x;
    float y;
} INTERPOLATION_POINT_t;

/**
 * Startup array which holds the interpolation tuples which combines the duration in milliseconds and the brightness in percentage value.
 * The light start with short flash, then a short flickering and a smooth startup.
 */
INTERPOLATION_POINT_t sXenonSwitchOn[NUM_XENON_ON_INTERPOLATION_STEPS] =
        {
                {
                        0, 100
                },
                {
                        60, 100
                },
                {
                        65, 5
                },
                {
                        250, 10
                },
                {
                        400, 7
                },
                {
                       2000, 100
                }
        };


/**
 * number of interpolation points for switching off
 */
#define NUM_XENON_OFF_INTERPOLATION_STEPS   4

/**
 * Switching off array which holds the interpolation tuples which combines the duration in milliseconds and the brightness in percentage value.
 * The light just turns off smoothly.
 */
INTERPOLATION_POINT_t sXenonSwitchOff[NUM_XENON_OFF_INTERPOLATION_STEPS] =
        {
                {
                        0, 100
                },
                {
                       60, 50
                },
                {
                       110, 10
                },
                {
                       500, 0
                }
        };

/**
 * constructor
 */
XenonLightSwitchBehaviour::XenonLightSwitchBehaviour() : LightSwitchBehaviour()
{
    _switchTimestamp = 0;
    _interpolationIndex = -1;
}

/**
 * destructor
 */
XenonLightSwitchBehaviour::~XenonLightSwitchBehaviour()
{
}

/**
 * Sets light of the behaviour. For Xenon lights we have to store the current timestamp and reset the interpolation index to 0
 * @param pLightStatus
 */
void XenonLightSwitchBehaviour::setLightStatus( LightStatus_t pLightStatus )
{
    if (pLightStatus != getLightStatus())
    {
        _switchTimestamp = millis();
        _interpolationIndex = 0;

        setLightStatusSelf(pLightStatus);
    }
}

/**
 * Calculates the current brightness of the light depending on the light status and the interpolation steps used for light status change.
 *
 *  @return the current brightness of the light in percentage (0-100)
 */
unsigned short XenonLightSwitchBehaviour::getBrightness( void )
{
    short value;
    short interpolationSteps = (ON == getLightStatus()) ? NUM_XENON_ON_INTERPOLATION_STEPS : NUM_XENON_OFF_INTERPOLATION_STEPS;
    INTERPOLATION_POINT_t *interpolationTable = (ON == getLightStatus()) ? sXenonSwitchOn: sXenonSwitchOff;

    if (-1 == _interpolationIndex)
    {
        return interpolationTable[interpolationSteps-1].y;
    }

    if (_interpolationIndex < interpolationSteps)
    {
        long currentMillis = millis() - _switchTimestamp;
        while (currentMillis > interpolationTable[_interpolationIndex].x)
        {
            _interpolationIndex++;

            if (_interpolationIndex >= interpolationSteps)
                return (ON == getLightStatus()) ? 100 : 0;
        }


        if (0 ==_interpolationIndex)
        {
            return interpolationTable[_interpolationIndex].y;
        }

        // linear interpolation
        //              f   -  f
        //               1      0
        // f(x) = f  + ----------- (x   -  x )
        //         0    x   -  x            0
        //               1      0
        value = interpolationTable[_interpolationIndex-1].y
                                                     + (interpolationTable[_interpolationIndex].y - interpolationTable[_interpolationIndex-1].y)
                                                     / (interpolationTable[_interpolationIndex].x - interpolationTable[_interpolationIndex-1].x)
                                                     * (currentMillis - interpolationTable[_interpolationIndex-1].x);

/*
        Serial.print( "  f1 = ");
        Serial.print(interpolationTable[_interpolationIndex].y);
        Serial.print( " f0 = ");
        Serial.print(interpolationTable[_interpolationIndex-1].y);
        Serial.print( " x1 = ");
        Serial.print(interpolationTable[_interpolationIndex].x);
        Serial.print( " x0 = ");
        Serial.print(interpolationTable[_interpolationIndex-1].x);

        Serial.print( "index = ");
        Serial.print(_interpolationIndex);
        Serial.print("   current millis: ");
        Serial.print(currentMillis);
        Serial.print("    brightness: ");
        Serial.print(value);
        Serial.print("\n");
*/
        return value;
    }

    return (ON == getLightStatus()) ? 100 : 0;
}

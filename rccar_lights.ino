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

// to include Adafruit_NeoPixel.h it's a hack, otherwise the library path
// will not be included as an include path
//#include <Adafruit_NeoPixel.h>
#include "RcCarLights.h"

RcCarLights g_RcCarLights;

//The setup function is called once at startup of the sketch
void setup()
{
    g_RcCarLights.setup();
}

// The loop function is called in an endless loop
void loop()
{
    g_RcCarLights.loop();
}

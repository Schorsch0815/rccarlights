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
 * Copyright: Jochen Schales 2016
 *
 * --------------------------------------------------------------------*/

#include "RcCarLightsConstants.h"


// pin 7 for pwm input
const int PIN_THROTTLE = 7;

// pin 8 for pwm input
const int PIN_STEERING = 8;

// pin 9 for pwm input
const int PIN_3RD_CHANNEL = 9;

const int PIN_PARKING_LIGHT = 2;
const int PIN_HEADLIGHT = 3;
const int PIN_NEO_PIXEL = 4;

const int PIN_EMERGENCY_LIGHT_SWITCH = 10;
const int PIN_SIRENE_SWITCH = 11;
const int PIN_TRAFFIC_BAR_SWITCH = 12;

const unsigned short MAX_LIGHT_GROUPS = 3;

const bool THROTTLE_REVERSE = true;


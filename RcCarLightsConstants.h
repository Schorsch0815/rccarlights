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

#ifndef RCCARLIGHTSCONTANTS_H
#define RCCARLIGHTSCONTANTS_H

// pin 7 for pwm input
extern const int PIN_THROTTLE;

// pin 8 for pwm input
extern const int PIN_STEERING;

// pin 9 for pwm input
extern const int PIN_3RD_CHANNEL;

extern const int PIN_PARKING_LIGHT;
extern const int PIN_HEADLIGHT;
extern const int PIN_NEO_PIXEL;

extern const int PIN_EMERGENCY_LIGHT_SWITCH;
extern const int PIN_SIRENE_SWITCH;
extern const int PIN_TRAFFIC_BAR_SWITCH;

extern const unsigned short MAX_LIGHT_GROUPS;

extern const bool THROTTLE_REVERSE;

#endif

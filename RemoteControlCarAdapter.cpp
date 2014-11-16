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

#include "limits.h"
#include "Arduino.h"

#include "RemoteControlCarAdapter.h"

#define NUM_CALIBRATION_ITERATION    20

/**
 * Constructor
 *
 * Initialize the adapter with the start values.
 *
 * @param pinThrottle specifies the arduino board pin number used for throttle
 * @param pThrottleReverse
 * @param pinSteering specifies the arduino board pin number used for steering
 */
RemoteControlCarAdapter::RemoteControlCarAdapter(int pinThrottle, bool pThrottleReverse, int pinSteering)
{
    // store pins used for input
    m_PinThrottle = pinThrottle;
    m_PinSteering = pinSteering;

    m_ThrottleReverse = pThrottleReverse;

    // let's start with 0, 0 means uninitialized
    m_RCThrottleNullValue = 0;
    m_RCThrottleValue = 0;
    m_PreviousThrottleRCValue = 0;

    // let's start with 0, 0 means uninitialized
    m_RCSteeringNullValue = 0;
    m_RCSteeringValue = 0;

    // Engine is stopped at start
    m_Throttle = STOP;

    // duration of current throttle is max
    m_DurationOfThrottle = 0;

    // Position for throttle switch is STOP
    m_ThrottleSwitch = STOP;

    // duration of current switch is max
    m_DurationOfThrottleSwitch = 0;

    // give the steering
    m_Steering = NEUTRAL;

    // Position for steering switch is NEUTRAL
    m_SteeringSwitch = NEUTRAL;

    // duration of current switch is max
    m_DurationOfSteeringSwitch = 0;

    // no acceleration at start
    m_Acceleration = 0;

    m_LastAccelerationTimestamp = 0;

    //
    m_LastReadTimestamp = 0L;

    // do calibration first
    m_IsCalibrated = false;
}

/**
 * configure pins to read throttle and steering
 *
 * This method configures 2 pins of the arduino board as input for throttle and
 * steering
 */
void RemoteControlCarAdapter::setupPins(void)
{
    pinMode(m_PinThrottle, INPUT);
    pinMode(m_PinSteering, INPUT);
}

/**
 * before we startup the system the remote controller has to be calibrated
 */
void RemoteControlCarAdapter::calibrate(void)
{
    Serial.println("calibrate");

    if (!isCalibrated())
    {
        m_RCThrottleNullValue = 0;
        m_RCSteeringNullValue = 0;

        // delay calibration to allow remote controller to initialize
        delay(200);

        for (int i = 0; i < NUM_CALIBRATION_ITERATION; ++i)
        {
            delay(10);
            m_LastReadTimestamp = readInputs();
            m_RCThrottleNullValue += m_RCThrottleValue;
            m_RCSteeringNullValue += m_RCSteeringValue;
        }

        m_RCThrottleNullValue /= NUM_CALIBRATION_ITERATION;
        m_RCSteeringNullValue /= NUM_CALIBRATION_ITERATION;

        m_PreviousThrottleRCValue = m_RCThrottleValue;

        m_Acceleration = 0;
        m_LastAccelerationTimestamp = m_LastReadTimestamp;

        m_IsCalibrated = true;

        Serial.println("\nCalibration done.");
        Serial.print("   m_PreviousThrottleRCValue: ");
        Serial.print(m_PreviousThrottleRCValue);
        Serial.print("   m_RCThrottleNullValue: ");
        Serial.print(m_RCThrottleNullValue);
        Serial.print("  m_RCSteeringNullValue: ");
        Serial.print(m_RCSteeringNullValue);
    }
}

unsigned long RemoteControlCarAdapter::determineDuration(unsigned long oldDuration, unsigned long oldValue,
                                                         unsigned long newValue, unsigned long lDeltaT)
{
    // handle/increase duration
    if (oldValue != newValue)
    {
        return 0;
    }
    else
    {
        return oldDuration + lDeltaT;
    }
}

void RemoteControlCarAdapter::refreshThrottle(long lDeltaT)
{
    // determine current throttle
    Throttle_t newThrottle = calculateThrottle();
    // handle/increase duration of throttle position
    m_DurationOfThrottle = determineDuration(m_DurationOfThrottle, m_Throttle, newThrottle, lDeltaT);
    m_Throttle = newThrottle;
}

void RemoteControlCarAdapter::refreshThrottleSwitch(long lDeltaT)
{
    // determine current throttle switch
    Throttle_t newThrottleSwitch = calculateThrottleSwitch();
    // handle/increase duration of throttle position
    m_DurationOfThrottleSwitch = determineDuration(m_DurationOfThrottleSwitch, m_ThrottleSwitch, newThrottleSwitch,
                                                   lDeltaT);
    m_ThrottleSwitch = newThrottleSwitch;
}

void RemoteControlCarAdapter::refreshSteeringSwitch(long lDeltaT)
{
    // determine current throttle switch
    Steering_t newSteeringSwitch = calculateSteeringSwitch();
    // handle/increase duration of throttle position
    m_DurationOfSteeringSwitch = determineDuration(m_DurationOfSteeringSwitch, m_SteeringSwitch, newSteeringSwitch,
                                                   lDeltaT);
    m_SteeringSwitch = newSteeringSwitch;
}

void RemoteControlCarAdapter::refreshAcceleration(long lDeltaT)
{
    if (ACCELERATION_MEASURE_INTERVAL < m_LastReadTimestamp + lDeltaT - m_LastAccelerationTimestamp)
    {
        m_Acceleration = (m_RCThrottleValue - m_PreviousThrottleRCValue) *calcAccelerationFactor();
        m_PreviousThrottleRCValue = m_RCThrottleValue;
        m_LastAccelerationTimestamp = m_LastReadTimestamp + lDeltaT;
    }
}

int RemoteControlCarAdapter::calcAccelerationFactor()
{
    if ((m_ThrottleReverse ? BACKWARD : FORWARD) == m_Throttle)
    {
        return -1;
    }
    else if ((m_ThrottleReverse ? FORWARD : BACKWARD) == m_Throttle)
    {
        return 1;
    }
    return 0;
}

/**
 *
 */
void RemoteControlCarAdapter::refresh(void)
{
//    Serial.println("refresh");

if (!isCalibrated())
    calibrate();

long lReadTimestamp = readInputs();

long lDeltaT = lReadTimestamp - m_LastReadTimestamp;

// determine acceleration
refreshAcceleration(lDeltaT);

// determine current throttle
refreshThrottle(lDeltaT);

// determine current throttle switch
refreshThrottleSwitch(lDeltaT);

// determine current throttle
m_Steering = calculateSteering();

// determine current throttle switch
refreshSteeringSwitch(lDeltaT);

// store timestamp from current input read
m_LastReadTimestamp = lReadTimestamp;
}

/**
 * Reads input values from configured pins
 *
 * This method reads the values provided by the remote controller to the arduino board
 * at the configured pins for throttle and steering
 */
unsigned long RemoteControlCarAdapter::readInputs(void)
{
m_RCThrottleValue = pulseIn(m_PinThrottle, HIGH, 20000);
m_RCSteeringValue = pulseIn(m_PinSteering, HIGH, 20000);

//#ifdef DEBUG
//    Serial.print ("\nThrottle value: ");
//    Serial.print (m_RCThrottleValue);
//    Serial.print ("    Steering value: ");
//    Serial.println (m_RCSteeringValue);
//#endif

return millis();
}

RemoteControlCarAdapter::Throttle_t RemoteControlCarAdapter::calculateThrottle(void)
{
if (m_RCThrottleNullValue + EPLSILON_NULL_THROTTLE < m_RCThrottleValue)
    return (m_ThrottleReverse ? FORWARD : BACKWARD);
else if (m_RCThrottleNullValue - EPLSILON_NULL_THROTTLE > m_RCThrottleValue)
    return (m_ThrottleReverse ? BACKWARD : FORWARD);
else
    return STOP;
}

RemoteControlCarAdapter::Throttle_t RemoteControlCarAdapter::calculateThrottleSwitch(void)
{
if (m_RCThrottleValue < m_RCThrottleNullValue - DELTA_THROTTLE_SWITCH
        || m_RCThrottleNullValue + DELTA_THROTTLE_SWITCH < m_RCThrottleValue)
{
    return UNDEFINED_THROTTLE;
}
else if (m_RCThrottleNullValue + EPLSILON_NULL_THROTTLE < m_RCThrottleValue)
{
    return (m_ThrottleReverse ? FORWARD : BACKWARD);
}
else if (m_RCThrottleNullValue - EPLSILON_NULL_THROTTLE > m_RCThrottleValue)
{
    return (m_ThrottleReverse ? BACKWARD : FORWARD);
}

return STOP;
}

RemoteControlCarAdapter::Steering_t RemoteControlCarAdapter::calculateSteering(void)
{
if (m_RCSteeringNullValue + EPLSILON_NULL_STEERING < m_RCSteeringValue)
    return LEFT;
else if (m_RCSteeringNullValue - EPLSILON_NULL_STEERING > m_RCSteeringValue)
    return RIGHT;
else
    return NEUTRAL;
}

RemoteControlCarAdapter::Steering_t RemoteControlCarAdapter::calculateSteeringSwitch(void)
{
if (m_RCSteeringValue < m_RCSteeringNullValue - DELTA_STEERING_SWITCH
        || m_RCSteeringNullValue + DELTA_STEERING_SWITCH < m_RCSteeringValue)
{
    return UNDEFINED_STEERING;
}
else if (m_RCSteeringNullValue + EPLSILON_NULL_STEERING < m_RCSteeringValue)
{
    return LEFT;
}
else if (m_RCSteeringNullValue - EPLSILON_NULL_STEERING > m_RCSteeringValue)
{
    return RIGHT;
}

return NEUTRAL;
}


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
 * @param pPinThrottle specifies the arduino board pin number used for throttle
 * @param pThrottleReverse
 * @param pPinSteering specifies the arduino board pin number used for steering
 */
RemoteControlCarAdapter::RemoteControlCarAdapter(int pPinThrottle, bool pThrottleReverse, int pPinSteering,
                                                 int pPin3rdChannel) :
        mThrottle(STOP), // Engine is stopped at start
        mThrottleReverse(pThrottleReverse), // controls, whether throttle channel is reverse or not
        mDurationOfThrottle(0), // duration of current throttle is 0
        mThrottleSwitch(STOP), // Position for throttle switch is STOP
        mDurationOfThrottleSwitch(0), // duration of current switch is 0
        mSteering(NEUTRAL), // Position for steering is NEUTRAL
        mSteeringSwitch(NEUTRAL), // Position for steering switch is NEUTRAL
        mDurationOfSteeringSwitch(0), // duration of current switch is 0
        mAcceleration(0), // no acceleration at start
        mRCThrottleNullValue(0), // Let's start with 0, 0 means uninitialized
        mRCThrottleValue(0), //
        mPreviousThrottleRCValue(0), //
        mRCSteeringNullValue(0), // let's start with 0, 0 means uninitialized
        mRCSteeringValue(0), //
        mRC3rdChannelValue(0), //
        mLastReadTimestamp(0L), //
        mLastAccelerationTimestamp(0L), //
        mIsCalibrated(false), //
        mPinThrottle(pPinThrottle), // store pins used for input
        mPinSteering(pPinSteering), //store pins used for input
        mPin3rdChannel(pPin3rdChannel) // third channel used for emergency bar
{
}

/**
 * configure pins to read throttle and steering
 *
 * This method configures 2 pins of the arduino board as input for throttle and
 * steering
 */
void RemoteControlCarAdapter::setupPins(void)
{
    pinMode(mPinThrottle, INPUT);
    pinMode(mPinSteering, INPUT);
    pinMode(mPin3rdChannel, INPUT);
}

/**
 * before we startup the system the remote controller has to be calibrated
 */
void RemoteControlCarAdapter::calibrate(void)
{
#ifdef DEBUG
    Serial.println("calibrate");
#endif

    // TODO check if we should implement a real calibrating functionality (check max throttle and steering values from
    // remote control)
    if (!isCalibrated())
    {
        mRCThrottleNullValue = 0;
        mRCSteeringNullValue = 0;

        // delay calibration to allow remote controller to initialize
        delay(200);

        for (int i = 0; i < NUM_CALIBRATION_ITERATION; ++i)
        {
            delay(10);
            mLastReadTimestamp = readInputs();
            mRCThrottleNullValue += mRCThrottleValue;
            mRCSteeringNullValue += mRCSteeringValue;
        }

        mRCThrottleNullValue /= NUM_CALIBRATION_ITERATION;
        mRCSteeringNullValue /= NUM_CALIBRATION_ITERATION;

        mPreviousThrottleRCValue = mRCThrottleValue;

        mAcceleration = 0;
        mLastAccelerationTimestamp = mLastReadTimestamp;

        mIsCalibrated = true;

#ifdef DEBUG
        Serial.println("\nCalibration done.");
        Serial.print("   mPreviousThrottleRCValue: ");
        Serial.print(mPreviousThrottleRCValue);
        Serial.print("   mRCThrottleNullValue: ");
        Serial.print(mRCThrottleNullValue);
        Serial.print("  mRCSteeringNullValue: ");
        Serial.print(mRCSteeringNullValue);
#endif
    }
}

/**
 * Determines duration of a remote control signal based on the old and the new value of the signal. If the values are
 * equal the last duration is increased by the passed delta. otherwise it's set to 0.
 *
 * @param pOldValue old value of the signal
 * @param pNewValue new value of the signal
 * @param pPreviousDuration previous duration in milliseconds from last measurement
 * @param pDeltaT time in milliseconds passed since last measurement
 *
 * @return new duration in milliseconds
 */
unsigned long RemoteControlCarAdapter::determineDuration(int pOldValue, int pNewValue, unsigned long pPreviousDuration,
                                                         unsigned long pDeltaT)
{
    // handle/increase duration
    if (pOldValue != pNewValue)
    {
        return 0;
    }
    else
    {
        return pPreviousDuration + pDeltaT;
    }
}

/**
 * refreshes the current throttle status from the throttle channel. Status can be FORWARD, STOP and BACKWARD. In case the
 * status has changed the duration was reseted to zero, otherwise it will be increased.
 * @param pDeltaT in milliseconds between last refresh and current refresh
 */
void RemoteControlCarAdapter::refreshThrottle(unsigned long pDeltaT)
{
    // determine current throttle
    Throttle_t newThrottle = calculateThrottle();
    // handle/increase duration of throttle position
    mDurationOfThrottle = determineDuration(mThrottle, newThrottle, mDurationOfThrottle, pDeltaT);
    mThrottle = newThrottle;
}

/**
 * refreshes the current throttle switch status from the throttle channel. Status can be FORWARD, STOP and BACKWARD. In case the
 * status has changed the duration was reseted to zero, otherwise it will be increased.
 * @param pDeltaT in milliseconds between last refresh and current refresh
 */
void RemoteControlCarAdapter::refreshThrottleSwitch(unsigned long pDeltaT)
{
    // determine current throttle switch
    Throttle_t newThrottleSwitch = calculateThrottleSwitch();
    // handle/increase duration of throttle position
    mDurationOfThrottleSwitch = determineDuration(mThrottleSwitch, newThrottleSwitch, mDurationOfThrottleSwitch,
                                                  pDeltaT);
    mThrottleSwitch = newThrottleSwitch;
}

/**
 * refreshes the current steering switch status from the throttle channel. Status can be FORWARD, STOP and BACKWARD. In case the
 * status has changed the duration was reseted to zero, otherwise it will be increased.
 * @param pDeltaT in milliseconds between last refresh and current refresh
 */
void RemoteControlCarAdapter::refreshSteeringSwitch(unsigned long pDeltaT)
{
    // determine current throttle switch
    Steering_t newSteeringSwitch = calculateSteeringSwitch();
    // handle/increase duration of throttle position
    mDurationOfSteeringSwitch = determineDuration(mSteeringSwitch, newSteeringSwitch, mDurationOfSteeringSwitch,
                                                  pDeltaT);
    mSteeringSwitch = newSteeringSwitch;
}

/**
 * refreshes the acceleration attribute from the remote control throttle setting. For measurement of the acceleration an individual
 * acceleration interval is used. The current acceleration value is calculated as a difference between the current throttle and
 * the throttle value from the last acceleration measuring point. The sign of the value is negative when the car slows down,
 * independent if car is moving forward or backward.
 * @param pDeltaT  in milliseconds between last refresh and current refresh
 */
void RemoteControlCarAdapter::refreshAcceleration(unsigned long pDeltaT)
{
    if (ACCELERATION_MEASURE_INTERVAL < mLastReadTimestamp + pDeltaT - mLastAccelerationTimestamp)
    {
        mAcceleration = (mPreviousThrottleRCValue - mRCThrottleValue) * calcAccelerationFactor();
        mPreviousThrottleRCValue = mRCThrottleValue;
        mLastAccelerationTimestamp = mLastReadTimestamp + pDeltaT;
    }
}

/**
 * returns a factor to correct the algebraic sign of the acceleration value. In case the car moves forward, it returns -1 if
 * car moves backwards it returns 1 otherwise 0.
 * @return the acceleration factor (-1, 0 or 1)
 */
int RemoteControlCarAdapter::calcAccelerationFactor()
{
    if ((mThrottleReverse ? BACKWARD : FORWARD) == mThrottle)
    {
        return 1;
    }
    else if ((mThrottleReverse ? FORWARD : BACKWARD) == mThrottle)
    {
        return -1;
    }
    return 0;
}

/**
 * refresh the values for throttle and steering from remote controller and calculate
 * all dependent values like, switch position for throttle and steering, acceleration, etc
 */
void RemoteControlCarAdapter::refresh(void)
{
    if (!isCalibrated())
        calibrate();

    unsigned long lReadTimestamp = readInputs();

    unsigned long lDeltaT = lReadTimestamp - mLastReadTimestamp;

// determine acceleration
    refreshAcceleration(lDeltaT);

// determine current throttle
    refreshThrottle(lDeltaT);

// determine current throttle switch
    refreshThrottleSwitch(lDeltaT);

// determine current throttle
    mSteering = calculateSteering();

// determine current throttle switch
    refreshSteeringSwitch(lDeltaT);

// store timestamp from current input read
    mLastReadTimestamp = lReadTimestamp;
}

/**
 * Reads input values from configured pins
 *
 * This method reads the values provided by the remote controller to the arduino board
 * at the configured pins for throttle and steering
 */
unsigned long RemoteControlCarAdapter::readInputs(void)
{
    mRCThrottleValue = pulseIn(mPinThrottle, HIGH, 20000);
    mRCSteeringValue = pulseIn(mPinSteering, HIGH, 20000);
    mRC3rdChannelValue = pulseIn(mPin3rdChannel, HIGH, 20000);

//#ifdef DEBUG
//    Serial.print ("\nThrottle value: ");
//    Serial.print (mRCThrottleValue);
//    Serial.print ("    Steering value: ");
//    Serial.println (mRCSteeringValue);
//#endif

    return millis();
}

/**
 *
 * @return current throttle value (FORWARD, STOP or BACKWARD)
 */
RemoteControlCarAdapter::Throttle_t RemoteControlCarAdapter::calculateThrottle(void)
{
    if (mRCThrottleNullValue + EPLSILON_NULL_THROTTLE < mRCThrottleValue)
        return (mThrottleReverse ? FORWARD : BACKWARD);
    else if (mRCThrottleNullValue - EPLSILON_NULL_THROTTLE > mRCThrottleValue)
        return (mThrottleReverse ? BACKWARD : FORWARD);
    else
        return STOP;
}

/**
 *
 * @return current throttle switch value (FORWARD, STOP, BACKWARD or UNDEFINED_THROTTLE if throttle value is
 * out of throttle switch range)
 */
RemoteControlCarAdapter::Throttle_t RemoteControlCarAdapter::calculateThrottleSwitch(void)
{
    if (mRCThrottleValue < mRCThrottleNullValue - DELTA_THROTTLE_SWITCH
            || mRCThrottleNullValue + DELTA_THROTTLE_SWITCH < mRCThrottleValue)
    {
        return UNDEFINED_THROTTLE;
    }
    else if (mRCThrottleNullValue + EPLSILON_NULL_THROTTLE < mRCThrottleValue)
    {
        return (mThrottleReverse ? FORWARD : BACKWARD);
    }
    else if (mRCThrottleNullValue - EPLSILON_NULL_THROTTLE > mRCThrottleValue)
    {
        return (mThrottleReverse ? BACKWARD : FORWARD);
    }

    return STOP;
}

/**
 *
 * @return current steering value (LEFT, NEUTRAL or RIGHT)
 */
RemoteControlCarAdapter::Steering_t RemoteControlCarAdapter::calculateSteering(void)
{
    if (mRCSteeringNullValue + EPLSILON_NULL_STEERING < mRCSteeringValue)
        return LEFT;
    else if (mRCSteeringNullValue - EPLSILON_NULL_STEERING > mRCSteeringValue)
        return RIGHT;
    else
        return NEUTRAL;
}

/**
 *
 * @return current steering switch value (LEFT, NEUTRAL, RIGHT or UNDEFINED_STEERING if steering value is
 * out of steering switch range)
 */
RemoteControlCarAdapter::Steering_t RemoteControlCarAdapter::calculateSteeringSwitch(void)
{
    if (mRCSteeringValue < mRCSteeringNullValue - DELTA_STEERING_SWITCH
            || mRCSteeringNullValue + DELTA_STEERING_SWITCH < mRCSteeringValue)
    {
        return UNDEFINED_STEERING;
    }
    else if (mRCSteeringNullValue + EPLSILON_NULL_STEERING < mRCSteeringValue)
    {
        return LEFT;
    }
    else if (mRCSteeringNullValue - EPLSILON_NULL_STEERING > mRCSteeringValue)
    {
        return RIGHT;
    }

    return NEUTRAL;
}


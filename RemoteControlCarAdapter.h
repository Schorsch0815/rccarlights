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

#ifndef RemoteControlCarAdapter_h
#define RemoteControlCarAdapter_h

class RemoteControlCarAdapter
{
public:
    typedef enum
    {
        FORWARD, STOP, BACKWARD, UNDEFINED_THROTTLE
    } Throttle_t;

    typedef enum
    {
        LEFT, NEUTRAL, RIGHT, UNDEFINED_STEERING
    } Steering_t;

    /**
     * Constructor
     * @param pinThrottle defines the (digital) arduino pin, which is used to read the throttle channel
     * @param pinSteering defines the (digital) arduino pin, which is used to read the steering value
     */
    RemoteControlCarAdapter(int pinThrottle, bool pThrottleReverse, int pinSteering);

    /**
     * configure the arduino board to use specified pins for throttle and steering
     */
    void setupPins(void);

    /**
     * claibrates the system and has to be called once before calling refresh in a loop
     */
    void calibrate(void);

    /**
     * refresh the values for throttle and steering from remote controller and calculate
     * all dependent values like, switch position for throttle and steering, acceleration, etc
     */
    void refresh(void);

    /**
     * @return the current throttle value could be FORWARD, STOP or BACKWARD
     */
    inline Throttle_t getThrottle(void)
    {
        return mThrottle;
    }

    /**
     * @return the current value of the switch provided by the throttle channel. The value could be FORWARD, STOP,
     * BACKWARD or UNDEFINED. The value is UNDEFINED in case the throttle is outside the switch region
     */
    inline Throttle_t getThrottleSwitch(void)
    {
        return mThrottleSwitch;
    }

    /**
     * @return the duration in milliseconds since when the throttle switch did not change
     */
    inline unsigned long getDurationOfThrottleSwitch(void)
    {
        return mDurationOfThrottleSwitch;
    }

    /**
     * @return the current throttle value could be LEFT, NEUTRAL or RIGHT
     */
    inline Steering_t getSteering(void)
    {
        return mSteering;
    }

    /**
     * @return the current value of the switch provided by the steering channel. The value could be LEFT, NEUTRAL,
     * RIGHTFORWARD or UNDEFINED. The value is UNDEFINED in case the steering is outside the switch region
     */
    inline Steering_t getSteeringSwitch(void)
    {
        return mSteeringSwitch;
    }

    /**
     * @return the duration in milliseconds since when the steering switch did not change
     */
    inline unsigned long getDurationOfSteeringSwitch(void)
    {
        return mDurationOfSteeringSwitch;
    }

    /**
     * @return the current acceleration of the throttle (measured between the last and the current throttle position)
     */
    inline short getAcceleration(void)
    {
        return mAcceleration;
    }

    /*
     inline short getDurationOfStop(void)
     {
     return mDurationOfStop;
     }
     */

private:
    /**
     * Reads input values from configured pins
     *
     * This method reads the values provided by the remote controller to the arduino board
     * at the configured pins for throttle and steering
     */
    unsigned long readInputs(void);

    /**
     *
     * @return if remote controller is calibrated
     */
    inline bool isCalibrated(void)
    {
        return mIsCalibrated;
    }

    /**
     *
     * @return current throttle value (FORWARD, STOP or BACKWARD)
     */
    Throttle_t calculateThrottle(void);
    /**
     *
     * @return current throttle switch value (FORWARD, STOP, BACKWARD or UNDEFINED_THROTTLE if throttle value is
     * out of throttle switch range)
     */
    Throttle_t calculateThrottleSwitch(void);
    /**
     *
     * @return current steering value (LEFT, NEUTRAL or RIGHT)
     */
    Steering_t calculateSteering(void);
    /**
     *
     * @return current steering switch value (LEFT, NEUTRAL, RIGHT or UNDEFINED_STEERING if steering value is
     * out of steering switch range)
     */
    Steering_t calculateSteeringSwitch(void);

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
    unsigned long determineDuration(int pOldValue, int pNewValue, unsigned long pPreviousDuration,
                                    unsigned long pDeltaT);

    /**
     * refreshes the current throttle status from the throttle channel. Status can be FORWARD, STOP and BACKWARD. In case the
     * status has changed the duration was reseted to zero, otherwise it will be increased.
     * @param pDeltaT in milliseconds between last refresh and current refresh
     */
    void refreshThrottle(unsigned long pDeltaT);
    /**
     * refreshes the current throttle switch status from the throttle channel. Status can be FORWARD, STOP and BACKWARD. In case the
     * status has changed the duration was reseted to zero, otherwise it will be increased.
     * @param pDeltaT in milliseconds between last refresh and current refresh
     */
    void refreshThrottleSwitch(unsigned long pDeltaT);
    /**
     * refreshes the current steering switch status from the throttle channel. Status can be FORWARD, STOP and BACKWARD. In case the
     * status has changed the duration was reseted to zero, otherwise it will be increased.
     * @param pDeltaT in milliseconds between last refresh and current refresh
     */
    void refreshSteeringSwitch(unsigned long pDeltaT);
    /**
     * refreshes the acceleration attribute from the remote control throttle setting. For measurement of the acceleration an individual
     * acceleration interval is used. The current acceleration value is calculated as a difference between the current throttle and
     * the throttle value from the last acceleration measuring point. The sign of the value is negative when the car slows down,
     * independent if car is moving forward or backward.
     * @param pDeltaT  in milliseconds between last refresh and current refresh
     */
    void refreshAcceleration(unsigned long pDeltaT);
    /**
     * returns a factor to correct the algebraic sign of the acceleration value. In case the car moves forward, it returns 1 if
     * car moves backwards it returns -1 otherwise 0.
     * @return the acceleration factor (-1, 0 or 1)
     */
int calcAccelerationFactor();

private:
    // measure interval between to values of throttle to determine acceleration
    const unsigned long ACCELERATION_MEASURE_INTERVAL = 200;

    // epsilon for the null point of throttle
    const unsigned long EPLSILON_NULL_THROTTLE = 25;

    // epsilon for the null point of steering
    const unsigned long EPLSILON_NULL_STEERING = 25;

    // delta to border the switch on the throttle channel
    const unsigned long DELTA_THROTTLE_SWITCH = 60;

    // delta to border the switch on the steering channel
    const unsigned long DELTA_STEERING_SWITCH = 60;

    // status of throttle, could be FORWARD, STOP or BACKWARD
    Throttle_t mThrottle;

    bool mThrottleReverse;

    // duration of the current throttle in milli seconds
    unsigned long mDurationOfThrottle;

    // status of throttle switch, could be FORWARD, STOP or BACKWARD
    // The throttle switch, means the that throttle was minimal out of neutral position,
    // the limit is the value defined by DELTA_THROTTLE_SWITCH
    // but the engine is not running (may be needs additional configuration of speed
    // controller)
    Throttle_t mThrottleSwitch;

    // duration in milli seconds of the current throttle value, will be
    // reset to zero in case value changes
    unsigned long mDurationOfThrottleSwitch;

    // status of the steering, could be LEFT, NEUTRAL or RIGHT
    Steering_t mSteering;

    // status of steering switch, could be LEFT, NEUTRAL or RIGHT
    // The steering switch, means the that steering was minimal out of neutral position
    // the limit is the value defined by DELTA_STEERING_SWITCH
    Steering_t mSteeringSwitch;

    // duration in milli seconds of the current steering value, will be
    // reset to zero in case value changes
    unsigned long mDurationOfSteeringSwitch;

    // acceleration of the car:
    //  - positive values mean that the car speed up when driving forward
    //    and slow down when driving backward
    //  - negative values mean that the car speed up when driving backward
    //    and slow down when driving forward
    int mAcceleration;

    // holds the number of seconds the car stand still since last motion
    // short mDurationOfStop;

    // throttle value read from pulseIn
    unsigned long mRCThrottleNullValue;

    // throttle value read from pulseIn
    unsigned long mRCThrottleValue;

    // throttle value stored from pulseIn read mPreviousThrottleDelay
    unsigned long mPreviousThrottleRCValue;

    // steering null value read from pulseIn
    unsigned long mRCSteeringNullValue;

    // steering value read from pulseIn
    unsigned long mRCSteeringValue;

    // timestamp when the pins were read the last time in milli seconds
    unsigned long mLastReadTimestamp;

    // timestamp when the last acceleration was calculated milli seconds
    unsigned long mLastAccelerationTimestamp;

    // is false at start and true after system is calibrated
    bool mIsCalibrated;

    // pin used for pwm input for throttle
    int mPinThrottle;

    // pin used for pwm input for steering
    int mPinSteering;

};

#endif


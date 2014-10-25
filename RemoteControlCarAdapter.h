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
    RemoteControlCarAdapter(int pinThrottle, int pinSteering);

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
        return m_Throttle;
    }

    /**
     * @return the current value of the switch provided by the throttle channel. The value could be FORWARD, STOP,
     * BACKWARD or UNDEFINED. The value is UNDEFINED in case the throttle is outside the switch region
     */
    inline Throttle_t getThrottleSwitch(void)
    {
        return m_ThrottleSwitch;
    }

    /**
     * @return the duration in milliseconds since when the throttle switch did not change
     */
    inline unsigned long getDurationOfThrottleSwitch(void)
    {
        return m_DurationOfThrottleSwitch;
    }

    /**
     * @return the current throttle value could be LEFT, NEUTRAL or RIGHT
     */
    inline Steering_t getSteering(void)
    {
        return m_Steering;
    }

    /**
     * @return the current value of the switch provided by the steering channel. The value could be LEFT, NEUTRAL,
     * RIGHTFORWARD or UNDEFINED. The value is UNDEFINED in case the steering is outside the switch region
     */
    inline Steering_t getSteeringSwitch(void)
    {
        return m_SteeringSwitch;
    }

    /**
     * @return the duration in milliseconds since when the steering switch did not change
     */
    inline unsigned long getDurationOfSteeringSwitch(void)
    {
        return m_DurationOfSteeringSwitch;
    }

    /**
     * @return the current acceleration of the throttle (measured between the last and the current throttle position)
     */
    inline short getAcceleration(void)
    {
        return m_Acceleration;
    }

    /*
     inline short getDurationOfStop(void)
     {
     return m_DurationOfStop;
     }
     */

private:
    unsigned long readInputs(void);

    inline bool isCalibrated(void)
    {
        return m_IsCalibrated;
    }

    Throttle_t calculateThrottle(void);
    Throttle_t calculateThrottleSwitch(void);
    Steering_t calculateSteering(void);
    Steering_t calculateSteeringSwitch(void);

    unsigned long determineDuration(unsigned long oldDuration, unsigned long oldValue, unsigned long newValue,
                                    unsigned long lDeltaT);
    void refreshThrottle(long lDeltaT);
    void refreshThrottleSwitch(long lDeltaT);
    void refreshSteeringSwitch(long lDeltaT);
    void refreshAcceleration(long lDeltaT);
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
    Throttle_t m_Throttle;

    // duration of the current throttle in milli seconds
    unsigned long m_DurationOfThrottle;

    // status of throttle switch, could be FORWARD, STOP or BACKWARD
    // The throttle switch, means the that throttle was minimal out of neutral position,
    // the limit is the value defined by DELTA_THROTTLE_SWITCH
    // but the engine is not running (may be needs additional configuration of speed
    // controller)
    Throttle_t m_ThrottleSwitch;

    // duration in milli seconds of the current throttle value, will be
    // reset to zero in case value changes
    unsigned long m_DurationOfThrottleSwitch;

    // status of the steering, could be LEFT, NEUTRAL or RIGHT
    Steering_t m_Steering;

    // status of steering switch, could be LEFT, NEUTRAL or RIGHT
    // The steering switch, means the that steering was minimal out of neutral position
    // the limit is the value defined by DELTA_STEERING_SWITCH
    Steering_t m_SteeringSwitch;

    // duration in milli seconds of the current steering value, will be
    // reset to zero in case value changes
    unsigned long m_DurationOfSteeringSwitch;

    // acceleration of the car:
    //  - positive values mean that the car speed up when driving forward
    //    and slow down when driving backward
    //  - negative values mean that the car speed up when driving backward
    //    and slow down when driving forward
    int m_Acceleration;

    // holds the number of seconds the car stand still since last motion
    // short m_DurationOfStop;

    // throttle value read from pulseIn
    unsigned long m_RCThrottleNullValue;

    // throttle value read from pulseIn
    unsigned long m_RCThrottleValue;

    // throttle value stored from pulseIn read m_PreviousThrottleDelay
    unsigned long m_PreviousThrottleRCValue;

    // steering null value read from pulseIn
    unsigned long m_RCSteeringNullValue;

    // steering value read from pulseIn
    unsigned long m_RCSteeringValue;

    // timestamp when the pins were read the last time in milli seconds
    unsigned long m_LastReadTimestamp;

    // timestamp when the last acceleration was calculated milli seconds
    unsigned long m_LastAccelerationTimestamp;

    // is false at start and true after system is calibrated
    bool m_IsCalibrated;

    // pin used for pwm input for throttle
    int m_PinThrottle;

    // pin used for pwm input for steering
    int m_PinSteering;

};

#endif


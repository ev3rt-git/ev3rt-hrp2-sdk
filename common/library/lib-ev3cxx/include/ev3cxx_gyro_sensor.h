/**
 * \file    ev3cxx_gyro_sensor.h
 * \brief	EV3RT CPP API for gyro sensor
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_sensor.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class GyroSensor. API for working with gyro sensor.
 */
class GyroSensor : public Sensor
{
public:
    /**
     * \~English
     * \brief       Constructor of class GyroSensor. 
     * \param port  Port index (1-4).      
     */  
    GyroSensor(SensorPort port) : Sensor(port, GYRO_SENSOR) {
        resetHard();
    }

    /**
     * \~English
     * \brief       Get angle in degrees from gyro.
     * \details     Not recommended use angle() and rate() in same time => call rate() reset initial position in angle().
     * \return      Number of degrees of robot rotation from initial position. 
     *              Range: from -32768 to 32767 (after overflow, the value goes to the opposite range: +32767 +1 => -32768)  
     */  
    int angle() {
        return ev3_gyro_sensor_get_angle(m_port);
    }

    /**
     * \~English
     * \brief       Get angular speed from gyro. Speed = degrees per seconde.
     * \details     Not recommended use angle() and rate() in same time => call rate() reset initial position in angle().
     * \return      Range: 0 - 100     
     */  
	int rate() {
        return ev3_gyro_sensor_get_rate(m_port);
    }

    /**
     * \~English
     * \brief       Reset gyro.   
     * \details     Set the zero value on gyro when the robot not move => affect the angle() and rate() values
     *              Doesn't work everytime: https://bricks.stackexchange.com/questions/7115/how-can-ev3-gyro-sensor-drift-be-handled
     *              Then you need resetHard().
     */  
	void reset() {
        ev3_gyro_sensor_reset(m_port);
    }

    /**
     * \~English
     * \brief       Hard reset gyro. Solve the problem with drifting value after standard reset().  
     * \details     Set firstly gyro to one mode (angle) => then reset() => second mode (rate) => reset() => first mode (angle)
     */  
	void resetHard() {
        angle();
        reset();
        rate();
        reset();
        angle();
    }

}; // class GyroSensor

} // namespace ev3cxx

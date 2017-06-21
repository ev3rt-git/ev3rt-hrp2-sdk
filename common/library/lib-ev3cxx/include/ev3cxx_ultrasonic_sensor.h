/**
 * \file    ev3cxx_ultrasonic_sensor.h
 * \brief	EV3RT CPP API for ultrasonic sensor
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_sensor.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class UltrasonicSensor. API for working with ultrasonic sensor.
 */
class UltrasonicSensor : public Sensor
{
public:
    /**
     * \~English
     * \brief       Constructor of class UltrasonicSensor. 
     * \param port  Port index (1-4).      
     */  
    UltrasonicSensor(SensorPort port) : Sensor(port, ULTRASONIC_SENSOR) {
        centimeters(); // By read one value immediate activate the sensor
    }

    /**
     * \~English
     * \brief       Get distance in centimeters. 
     * \return      Distance in cm. Range: 3 - 255 (too near/far => 255)      
     */  
    int centimeters() {
        return ev3_ultrasonic_sensor_get_distance(m_port);
    }

    /**
     * \~English
     * \brief       Get millimeters in centimeters. 
     * \return      Distance in mm. Range: 30 - 2550 (too near/far => 2550)      
     */  
    int millimeters() {
        return ev3_ultrasonic_sensor_get_raw_data(m_port, US_DIST_METRIC);
    }

    /**
     * \~English
     * \brief       Get measure distance in inches. 
     * \return      Distance in in. Range: 1 - 100 (too near/far => 100)      
     */  
    int inches() {
        return ev3_ultrasonic_sensor_get_raw_data(m_port, US_DIST_IMPERIAL) / 10;
    }

    /**
     * \~English
     * \brief       Get measure distance in line (1/12 inch). 
     * \return      Distance in ln. Range: 13 - 1200 (too near/far => 1200)      
     */  
    int inchesLine() {
        return (ev3_ultrasonic_sensor_get_raw_data(m_port, US_DIST_IMPERIAL) * 12) / 10;
    }


    /**
     * \~English
     * \brief       Listen and return \a true if catch ultrasonic signal. 
     * \return      Return \a true if ultrasonic signal get, else \a false.    
     */  
    bool_t listen() {
        return ev3_ultrasonic_sensor_listen(m_port);
    }

}; // class UltrasonicSensor


} // namespace ev3cxx


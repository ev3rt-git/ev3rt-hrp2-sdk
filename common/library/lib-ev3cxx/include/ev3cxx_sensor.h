/**
 * \file    ev3cxx_sensor.h
 * \brief	EV3RT CPP API for sensor
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"

namespace ev3cxx {
    
/**
* \~English
* \brief    Enum with index of sensor ports
*/
enum class SensorPort {
    S1 = 0,
    S2,
    S3,
    S4
};

/**
 * \~English
 * \brief    Class Sensor
 * \details  API for working with sensor.
 */
class Sensor
{
protected:
    /**
     * \~English
     * \brief       Constructor of class Sensor. 
     * \param port  Port index (1-4).      
     */  
    Sensor(SensorPort port, sensor_type_t type)
     : m_port(static_cast<sensor_port_t>(port)), m_type(type) 
    {
        ev3_sensor_config(m_port, m_type);
    }
    
    /**
     * \~English
     * \brief 	    Destructor of class Sensor.
     */  
    virtual ~Sensor() {}
    
    /**
     * \~English
     * \brief       Get sensor port set in constructor. 
     * \return      Index of set port: 1 - 4      
     */  
    sensor_port_t getPort() {
        return m_port;
    }

    /**
     * \~English
     * \brief       Get actual sensor type. 
     * \return      Index of set type (sensor_type_t)      
     */
    sensor_type_t getType() {
        return static_cast<sensor_type_t>(ev3_sensor_get_type(m_port));
    }

    sensor_port_t m_port;
    sensor_type_t m_type;
}; // class Sensor


} // namespace ev3cxx


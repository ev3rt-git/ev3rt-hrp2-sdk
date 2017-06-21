/**
 * \file    ev3cxx_touch_sensor.h
 * \brief	EV3RT CPP API for touch sensor
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_sensor.h"
#include "ev3cxx_button.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class TouchSensor. API for working with touch sensor.
 */
class TouchSensor : public Sensor, public detail::Button
{
public:
    /**
     * \~English
     * \brief       Constructor of class TouchSensor. 
     * \param port  Port index (1-4).      
     */  
    TouchSensor(SensorPort port) : Sensor(port, TOUCH_SENSOR) {}

    /**
     * \~English
     * \brief       Get state of touch sensor. 
     * \return      If is pressed return \true, else \false. 
     */  
    int isPressed() {
        return ev3_touch_sensor_is_pressed(m_port);
    }

}; // class TouchSensor


} // namespace ev3cxx


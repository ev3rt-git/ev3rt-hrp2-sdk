/**
 * \file    ev3cxx_touch_sensor.h
 * \brief	EV3RT CPP API for touch sensor
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_sensor.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class TouchSensor
 * \details  API for working with touch sensor.
 */
class TouchSensor : public Sensor
{
public:
    /**
     * \~English
     * \brief       Constructor of class TouchSensor. 
     * \param port  Port index (1-4).      
     */  
    TouchSensor(Port port) : Sensor(port, TOUCH_SENSOR) {}

    /**
     * \~English
     * \brief       Get measure distance in centimeters. 
     * \return      Distance in cm. Range: 4 - 255 (to near/far = 255) [TODO: check range]      
     */  
    int isPressed() {
        return ev3_touch_sensor_is_pressed(m_port);
    }

	/**
	 * \~English
     * \brief       Blocks until sensor is pressed
	 */
	void waitForPress() {
		while (!isPressed())
			tslp_tsk(1);
	}

	/**
	 * \~English
     * \brief       Blocks until sensor is release
	 */
	void waitForRelease() {
		while (isPressed())
			tslp_tsk(1);
	}

	/**
	 * \~English
     * \brief       Blocks until sensor is clicked
	 */
	void waitForClick() {
		waitForPress();
		waitForRelease();
	}
    

}; // class TouchSensor


} // namespace ev3cxx


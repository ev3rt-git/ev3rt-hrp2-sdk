/**
 * \file    ev3cxx_color_sensor.h
 * \brief	EV3RT CPP API for color sensor
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_sensor.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class ColorSensor. API for working with color sensor.
 */
class ColorSensor : public Sensor
{
public:
	struct ColorRaw_t { int r; int g; int b; };       

    /**
     * \~English
     * \brief       Constructor of class ColorSensor. 
     * \param port  Port index (1-4).      
     */  
    ColorSensor(SensorPort port) : Sensor(port, COLOR_SENSOR) {}

    /**
     * \~English
     * \brief       Get reflected light intensity. Red LED on.
     * \return      Range: 0 - 100   
     */  
    int reflected() {
        return ev3_color_sensor_get_reflect(m_port);
    }

    /**
     * \~English
     * \brief       Get raw reflected light intensity (red, green, blue). Red LED on.
     * \return      Return rgb_raw_t (r, g, b)    
     */  
	rgb_raw_t reflectedRawRgb() {
		rgb_raw_t color;
		ev3_color_sensor_get_rgb_raw(m_port, &color);
		return color;
	}

    /**
     * \~English
     * \brief       Get ambient light intensity. Red LEDs off. 
     * \return      Range: 0 - 100     
     */  
	int ambient() {
        return ev3_color_sensor_get_ambient(m_port);
    }

    /**
     * \~English
     * \brief       Get color. 
     * \return      Return colorid_t (COLOR_NONE, COLOR_BLACK, COLOR_BLUE).     
     */  
	colorid_t color() {
        return ev3_color_sensor_get_color(m_port);
    }
}; // class ColorSensor

} // namespace ev3cxx

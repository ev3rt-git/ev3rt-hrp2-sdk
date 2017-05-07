/**
 * \file    ev3cxx_brick_led.h
 * \brief	EV3RT CPP API for brick LED
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"

namespace ev3cxx {

enum class StatusLightColor {
    OFF = LED_OFF,
    RED = LED_RED,
    GREEN = LED_GREEN,
    ORANGE = LED_ORANGE
};

namespace detail {

/**
 * \~English
 * \brief    Class BrickLED. API for working with brick status light.
 */
class BrickLED
{
public:
	/**
	 * \~English
     * \brief       set status light color
	 */
	void setColor(StatusLightColor color) {
		ev3_led_set_color(static_cast<ledcolor_t>(color));
	}

}; // class BrickLED

} // namespace detail

} // namespace ev3cxx

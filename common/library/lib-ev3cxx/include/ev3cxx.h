/**
 * \file    ev3cxx.h
 * \brief	EV3RT CPP API
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

/**
 * EV3 hardware independent libraries
 */
#include "ev3cxx_format.h"
#include "ev3cxx_file.h"

/**
 * EV3 hardware dependent libraries
 */
#include "ev3cxx_display.h"
#include "ev3cxx_bluetooth.h"
#include "ev3cxx_motor.h"
#include "ev3cxx_sensor.h"
#include "ev3cxx_touch_sensor.h"
#include "ev3cxx_color_sensor.h"
#include "ev3cxx_ultrasonic_sensor.h"


/**
 * EV3CXX global objects
 */
namespace ev3cxx {

extern ::ev3cxx::detail::Display display;

} // ev3cxx
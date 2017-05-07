/*
 * ev3cxx_display.cpp
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"

namespace ev3cxx {

/**
 * Global object ev3cxx::display for working with LCD
 */
::ev3cxx::detail::Display display;

/**
 * Global object ev3cxx::statusLight for working with brick LED
 */
::ev3cxx::detail::BrickLED statusLight;

namespace detail {
    
/**
 * Global object ev3cxx::detail::us_counter for internal time handling
 */
us_counter_t us_counter;

} // namespace detail

} // namespace ev3cxx
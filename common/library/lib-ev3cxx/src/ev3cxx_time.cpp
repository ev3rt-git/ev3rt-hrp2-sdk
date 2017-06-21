/*
 * ev3cxx_time.cpp
 * Author: Jaroslav Páral (jarekparal)
 */


#include "ev3cxx_time.h"

namespace ev3cxx {

void wait(detail::us_counter_t::time_type time) { detail::wait(detail::us_counter, time * 1000); }
void uwait(detail::us_counter_t::time_type time) { detail::wait(detail::us_counter, time); }

void delayMs(unsigned int ms) { tslp_tsk(ms); }
void delayUs(unsigned int us) { wait(us); }

detail::us_counter_t::time_type usec(detail::us_counter_t::time_type t) { return t *       1; }
detail::us_counter_t::time_type msec(detail::us_counter_t::time_type t) { return t *    1000; }
detail::us_counter_t::time_type  sec(detail::us_counter_t::time_type t) { return t * 1000000; }

} // namespace ev3cxx

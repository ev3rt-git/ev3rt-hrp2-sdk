/**
 * \file    ev3cxx_utils.h
 * \brief	EV3RT CPP utilities
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

namespace ev3cxx {

template <typename T>
T abs(T v) { return v >= 0 ? v : -v; }

} // namespace ev3cxx

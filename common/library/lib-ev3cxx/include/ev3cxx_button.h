/**
 * \file    ev3cxx_button.h
 * \brief   EV3RT CPP API for general button
 * \author  Jaroslav Páral (jarekparal)
 */

#pragma once

#include "ev3api.h"

namespace ev3cxx {

namespace detail {

/**
 * \~English
 * \brief    Class Button. API for working with general button buttons.
 */
class Button
{
public:
    /**
     * \~English
     * \brief       Get state of touch sensor. 
     * \return      If is pressed return \true, else \false. 
     */  
    virtual int isPressed() = 0;

    /**
     * \~English
     * \brief       Blocks until button is pressed
     */
    void waitForPress() {
        while (!isPressed())
            tslp_tsk(1);
    }

    /**
     * \~English
     * \brief       Blocks until button is release
     */
    void waitForRelease() {
        while (isPressed())
            tslp_tsk(1);
    }

    /**
     * \~English
     * \brief       Blocks until button is clicked (press and release)
     */
    void waitForClick() {
        waitForPress();
        waitForRelease();
    }
    

}; // class Button

} // namespace detail

} // namespace ev3cxx

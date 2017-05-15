/**
 * \file    ev3cxx_brick_button.h
 * \brief   EV3RT CPP API for brick button
 * \author  Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_button.h"

namespace ev3cxx {

/**
* \~English
* \brief    Enum with index of sensor port
*/
enum class BrickButtons {
    LEFT    = LEFT_BUTTON,  //!< \~English Left brick button
    RIGHT   = RIGHT_BUTTON, //!< \~English Right brick button
    UP      = UP_BUTTON,    //!< \~English Up brick button
    DOWN    = DOWN_BUTTON,  //!< \~English Down brick button
    ENTER   = ENTER_BUTTON, //!< \~English Enter brick button
    BACK    = BACK_BUTTON   //!< \~English Back brick button
};

/**
 * \~English
 * \brief    Class BrickButton. API for working with brick buttons.
 */
class BrickButton : public detail::Button
{
public:
    /**
     * \~English
     * \brief         Constructor of class BrickButton. 
     * \param button  Button identificator.
     */  
    BrickButton(BrickButtons button)
        : m_button(button)
    {}

    /**
     * \~English
     * \brief       Get state of brick button. 
     * \return      If is pressed return \true, else \false. 
     */  
    int isPressed() {
        return ev3_button_is_pressed(static_cast<button_t>(m_button));
    }

private:
    const BrickButtons m_button;

}; // class BrickButton

} // namespace ev3cxx

/**
 * \file    ev3cxx_bluetooth.h
 * \brief	EV3RT CPP API for bluetooth
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_format.h"
#include "ev3cxx_file.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class Bluetooth. API for working with bluetooth.
 */
class Bluetooth : public File
{
public:
    /**
     * \~English
     * \brief 	    Constructor of class Bluetooth. Don't open bluetooth port. For opening port call \a open().
     */  
    Bluetooth() : File() {}
    
    Bluetooth(bool_t open_bt) : File() {
        if(open_bt) {
            open();
        }
    }

    /**
     * \~English
     * \brief 	    Destructor of class Bluetooth - DO NOTHING.  
     */  
    ~Bluetooth() {}
    
    /**
     * \~English
     * \brief 	    Opening serial port on bluetooth. Params are not used. 
     * \return      Return \a true if is open, else \a false.
     */  
    virtual bool_t open(const char * filename = "", const char * mode = "w+") {
        if(m_filedesc == NULL) {
            m_filedesc = ev3_serial_open_file(EV3_SERIAL_BT);
        }
        return (m_filedesc != NULL);
    }

    /**
     * \~English
     * \brief   Return if is bluetooth port open.
     * \return  If the bluetooth port is open return \a true, else \a false.
     */   
    virtual bool_t isOpen() {
        return (m_filedesc != NULL);
    }

    /**
    * \~English
    * \brief            It is checked whether Bluetooth (Serial Port Profile) is connected.
    * \retval true      Connected. It can communicate with the Bluetooth virtual serial port.
    * \retval false     Unconnected.
    */
    bool_t isConnected() {
        return ev3_bluetooth_is_connected();
    }

    /**
     * \~English
     * \brief  Close the Bluetooth - DO NOTHING. 
     * \return Everytime return zero.
     */   
    virtual int close() {
        return 0;
    }
}; // class Bluetooth


} // namespace ev3cxx


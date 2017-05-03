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
 * \brief    Class Bluetooth
 * \details  API for working with bluetooth.
 */
class Bluetooth : public File
{
public:
   
    /**
     * \~English
     * \brief 	    Constructor of class Bluetooth. Don't open bluetooth port. For opening port call \a open().
     */  
    Bluetooth() : File("")
    {
        m_filedesc = NULL;
    }
    
    /**
     * \~English
     * \brief 	    Destructor of class Bluetooth - DO NOTHING.  
     */  
    ~Bluetooth() {}
    
    /**
     * \~English
     * \brief 	    Opening serial port on bluetooth. Params are not used. 
     * \return      Return file descriptor to open bluetooth or NULL. 
     */  
    virtual FILE * open(const char * filename = "", const char * mode = "w+") {
        if(m_filedesc == NULL) {
            m_filedesc = ev3_serial_open_file(EV3_SERIAL_BT);
        }
        return m_filedesc;
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


private:
     FILE * m_filedesc;               //!< \~English file of this class 
   
}; // class File


} // namespace ev3cxx


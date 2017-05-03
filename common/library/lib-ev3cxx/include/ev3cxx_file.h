/**
 * \file    ev3cxx_file.h
 * \brief	EV3RT CPP API for file
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include <stdio.h>

#include "ev3api.h"
#include "ev3cxx_format.h"

namespace ev3cxx {
    
/**
 * \~English
 * \brief    Class File
 * \details  API for working with file.
 */
class File
{
public:
   
    /**
     * \~English
     * \brief 	       Constructor of class File.
     * \param filename Path to file.
     * \param mode     Mode in which will be file open. More info http://www.cplusplus.com/reference/cstdio/fopen/
     *                 Default mode is "w+": Create an empty file and open it for update (both for input and output). 
     *                 If a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
     */  
    File(const char * filename, const char * mode = "w+") {
        open(filename, mode);
    }

    /**
     * \~English
     * \brief 	    Destructor of class File. Close the file if is open.
     */
    ~File() {
        close();
    }

    /**
     * \~English
     * \brief    Return file descriptor to open file or NULL.
     */
    FILE * operator () () {
        return m_filedesc;
    }

    /**
     * \~English
     * \brief          Return file descriptor to open file or NULL.
     * \param filename Path to file.
     * \param mode     Mode in which will be file open. More info http://www.cplusplus.com/reference/cstdio/fopen/
     *                 Default mode is "w+": Create an empty file and open it for update (both for input and output). 
     *                 If a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
     */
    virtual FILE * open(const char * filename, const char * mode = "w+") {
        return (m_filedesc = fopen(filename, mode));
    }
    
    /**
     * \~English
     * \brief   Return if is file open. 
     * \return  If the file is open return \a true, else \a false.
     */   
    virtual bool_t isOpen(){
        return (m_filedesc != NULL);
    }

    /**
     * \~English
     * \brief   Close the file if is open. 
     * \return  If the file is successfully closed, a zero value is returned. On failure, EOF is returned.
     */   
    virtual int close() {
        if(isOpen()) { 
            int ret = fclose(m_filedesc);
            if(0 == ret) {
                m_filedesc = NULL;
            }
            return ret;
        }
        return 0;
    }

    /**
     * \~English
     * \brief   Write character to file. 
     * \return  Return the character written as an unsigned char cast to an int or EOF on error.
     */   
    int write(char ch) {
        return fputc(ch, m_filedesc);
    }

    /**
     * \~English
     * \brief   Read character from file. 
     * \return  On success, the character read is returned (promoted to an int value).
     *          The return type is int to accommodate for the special value EOF, which indicates failure:
     *          If the position indicator was at the end-of-file, the function returns EOF and sets the eof indicator (feof) of stream.
     *          If some other reading error happens, the function also returns EOF, but sets its error indicator (ferror) instead.
     */   
    int readChar() {
        return fgetc(m_filedesc);
    } 

    /**
     * \~English
     * \brief   Read number from file. 
     * \return  On success, the function returns the number of items of the argument list successfully filled. 
     *          This count can match the expected number of items or be less (even zero) due to a matching failure, 
     *          a reading error, or the reach of the end-of-file.
     *          If a reading error happens or the end-of-file is reached while reading, the proper indicator is set (feof or ferror). 
     *          And, if either happens before any data could be successfully read, EOF is returned.
     */     
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, int>::type 
    readNumber(T &val) 
    {
        long long integral;
        int er = fscanf(m_filedesc, "%lld", &integral);
        val = integral;
        return er;
    }  

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, int>::type 
    readNumber(T &val) 
    {
        double floating_point;
        int er = fscanf(m_filedesc, "%lf", &floating_point);
        val = floating_point;
        return er;
    }  

    /**
     * \~English    
     * \brief 	    Set position of stream to the beginning.
     * \details     Vraper around standard function rewind(). Sets the position indicator associated with stream to the beginning of the file.
     */
    void rewind() {
        ::rewind(m_filedesc);
    }

private:
     FILE * m_filedesc;               //!< \~English file of this class 
   
}; // class File


} // namespace ev3cxx


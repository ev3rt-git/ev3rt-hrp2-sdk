/**
 * \file    ev3cxx_display.h
 * \brief	EV3RT CPP API for display
 * \author	Jaroslav Páral (jarekparal)
 */

#pragma once

#include "ev3api.h"
#include "ev3cxx_format.h"

namespace ev3cxx {

namespace detail {
    
/**
 * \~English
 * \brief    Class Display. API for working with text on display.
 */
class Display
{
public:
    static const uint8_t width = EV3_LCD_WIDTH;     //!< \~English Width of LCD in pixels
    static const uint8_t height = EV3_LCD_HEIGHT;   //!< \~English Height of LCD in pixels

    static const bool_t white = EV3_LCD_WHITE;      //!< \~English White color of write to LCD
    static const bool_t black = EV3_LCD_BLACK;      //!< \~English Black color of write to LCD

    /**
     * \~English
     * \brief 	    Constructor of class Display.
     * \param font  Type of font \a lcdfont_t (EV3_FONT_SMALL / EV3_FONT_MEDIUM). Default is EV3_FONT_MEDIUM.
     */
    Display(lcdfont_t font = EV3_FONT_MEDIUM)
    :m_x(0), m_y(0)
    {
        setFont(font);
    }
    
    /**
     * \~English
     * \brief 	    Destructor of class Display.
     */
    ~Display() {}

    /**
     * \~English    
     * \brief 	      Write formated text to LCD.
     * \param pattern String with pattern for final output text.
     */
    format_impl<Display, string_literal_range> format(char const *pattern) {
        return format_impl<Display, string_literal_range>(
            *this, string_literal_range(pattern));
    }

    /**
     * \~English    
     * \brief 	      Write formated text to LCD.
     * \param line    Line index to write text on LCD.
     * \param pattern String with pattern for final output text.
     */
    format_impl<Display, string_literal_range> format(int8_t line,
                                                  char const *pattern) {
        setTextLine(line);                                                
        return format_impl<Display, string_literal_range>(
            *this, string_literal_range(pattern));
    }

    /**
     * \~English
     * \brief        Write one character on screen of class Display.
     * \details      Char "\n" set next text line.
     */
    void write(char ch) {
        if(ch == '\n') {
            setTextLine(getTextLine() + 1);   
        } else if(ch == '\t') {
            write(' ');
            write(' ');
        } else if(ch == '\r') {
            setTextLine(getTextLine(), width);  
        }
        else {
            ev3_lcd_draw_character(ch, m_x, m_y);
            m_x += getFontWidth();

            if(m_x >= width) { 
                m_y += getFontHeight(); 
                m_x = 0;    

                if(m_y >= height) { m_y = 0; } 
            }          
        }   
    }

    /**
     * \~English
     * \brief        Get actual text line index.
     * \details      Range depend on actual font type (EV3_FONT_MEDIUM => 0 - 7). 
     */
    int32_t getTextLine() const { return m_y / getFontHeight(); }
    
    /**
     * \~English
     * \brief           Set actual text line index of cursor for next write. 
     * \details         Also clear all line (\a clearTextLine()).
     * \param line      Line index. Range depend on actual font type (EV3_FONT_MEDIUM => 0 - 7). 
     *                  If out of range, then result is \a "line % m_max_line_on_height". 
     * \param x_offset  Offset for clearing the line in x coordinate. When you want clear just part of display, then you set this offset.
     *                  If you want to set offset depend on character, use function \a getFontWidth() and multiple by require number of char.
     */
    void setTextLine(uint8_t line, int32_t x_offset = 0) {
        if((line * getFontHeight()) < height) {
            clearTextLine(line, white, x_offset);
        }
        m_y = (line * getFontHeight()) % height;
        m_x = x_offset;
    }

    /**
     * \~English
     * \brief           Clear a line. 
     * \param line      Line index. Range depend on actual font type (EV3_FONT_MEDIUM => 0 - 7). 
     *                  If out of range, then result is \a "line % m_max_line_on_height".    
     * \param color     Color the cleared with. You can set \a white or \a black. 
     * \param x_offset  Offset for clearing the line in x coordinate. When you want clear just part of display, then you set this offset.
     *                  If you want to set offset depend on character, use function \a getFontWidth() and multiple by require number of char.
     */
    void clearTextLine(uint8_t line, bool_t color = white, int32_t x_offset = 0) {
        ev3_lcd_fill_rect(0 + x_offset, (line * getFontHeight()) % height, width,
                        getFontHeight(), static_cast<lcdcolor_t>(color));
    }

    /**
     * \~English
     * \brief           Clear whole LCD. 
     * \param color     Color of screen after reset. You can set \a white or \a black. 
     */
    void resetScreen(bool_t color = white) {
        ev3_lcd_fill_rect(0, 0, width, height, static_cast<lcdcolor_t>(color));
        setTextLine(0);
    }

    /**
     * \~English
     * \brief       Set font type.
     * \details     After changing font set variables \a m_font_width => getFontWidth() and \a m_font_height -> getFontHeight(). 
     * \param font  Type of font \a lcdfont_t (EV3_FONT_SMALL / EV3_FONT_MEDIUM). 
     */   
    void setFont(lcdfont_t font) {
        m_font = font;
        ev3_lcd_set_font(font);
        ev3_font_get_size(font, &m_font_width, &m_font_height);
        m_max_char_on_line = width / m_font_width;
        m_max_line_on_height = height / m_font_height;
    }

    /**
     * \~English
     * \brief       Get actual font type. 
     */   
    lcdfont_t getFont() const { return m_font; }

    /**
     * \~English
     * \brief       Get actual font width.
     */   
    int32_t getFontWidth() const { return m_font_width; }

    /**
     * \~English
     * \brief       Get actual font height.
     */   
    int32_t getFontHeight() const { return m_font_height; }

    /**
     * \~English
     * \brief       Get actual X coordinates.
     */   
    int32_t getX() const { return m_x; }

    /**
     * \~English
     * \brief       Get actual Y coordinates.
     */   
    int32_t getY() const { return m_y; }

private:
    lcdfont_t m_font;               //!< \~English actual font 
    int32_t m_font_width;           //!< \~English width of actual font 
    int32_t m_font_height;          //!< \~English height of actual font 
    int32_t m_max_char_on_line;     //!< \~English max number of character on one line
    int32_t m_max_line_on_height;   //!< \~English max number of line on screen
    int32_t m_x;                    //!< \~English actual horizontal coordinate from upper left corner 
    int32_t m_y;                    //!< \~English actual vertical coordinate from upper left corner 
}; // class Display

} // namespace detail

} // namespace ev3cxx


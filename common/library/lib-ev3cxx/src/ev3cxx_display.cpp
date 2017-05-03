/*
 * ev3cxx_display.cpp
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx_display.h"

using namespace ev3cxx::detail;

Display::Display(lcdfont_t font)
:m_x(0), m_y(0)
{
    setFont(font);
}

Display::~Display() {}

void Display::write(char ch) {
    if(ch == '\n')
        setTextLine(getTextLine() + 1);    
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

void Display::setTextLine(uint8_t line, int32_t x_offset) {
    clearTextLine(line, white, x_offset);
    m_y = (line * getFontHeight()) % height;
    m_x = x_offset;
}

int32_t Display::getTextLine() const { return m_y / getFontHeight(); }

void Display::clearTextLine(uint8_t line, bool_t color, int32_t x_offset) {
    ev3_lcd_fill_rect(0 + x_offset, (line * getFontHeight()) % height, width,
                      getFontHeight(), static_cast<lcdcolor_t>(color));
}

void Display::resetScreen(bool_t color) {
  ev3_lcd_fill_rect(0, 0, width, height, static_cast<lcdcolor_t>(color));
  setTextLine(0);
}

void Display::setFont(lcdfont_t font) {
    m_font = font;
    ev3_lcd_set_font(font);
    ev3_font_get_size(font, &m_font_width, &m_font_height);
    m_max_char_on_line = width / m_font_width;
    m_max_line_on_height = height / m_font_height;
}

lcdfont_t Display::getFont() const { return m_font; }

int32_t Display::getFontWidth() const { return m_font_width; }

int32_t Display::getFontHeight() const { return m_font_height; }

int32_t Display::getX() const { return m_x; }

int32_t Display::getY() const { return m_y; }

namespace ev3cxx {

/**
 * Global object ev3cxx::display for working with LCD
 */
::ev3cxx::detail::Display display;

} // ev3cxx
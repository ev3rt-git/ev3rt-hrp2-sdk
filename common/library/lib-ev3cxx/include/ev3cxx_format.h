/**
 * \file    ev3cxx_format.h
 * \brief	API for formating data to output (display, bluetooth, file)
 * \author	Jakub Streit, Jaroslav Páral (jarekparal)
 */

#pragma once

#include "ev3api.h"

namespace ev3cxx {

template <typename Stream>
void send(Stream & s, char const * str)
{
    for (; *str != 0; ++str)
        s.write(*str);
}

template <typename Stream>
void send_bool(Stream & s, bool value)
{
    send(s, value? "true": "false");
}

template <typename Stream, typename Unsigned>
void send_bin_text(Stream & s, Unsigned v, uint8_t width = 0, char fill = '0')
{
    char buf[32];
    uint8_t i = 0;

    if (v == 0)
    {
        buf[i++] = '0';
    }
    else if (v < 0)
    {
        buf[i++] = '*';
    }
    else
    {
        for (; v != 0; v >>= 1)
        {
            buf[i++] = '0' + (v & 0x1);
        }
    }

    while (i < width)
        buf[i++] = fill;
    
    for (; i > 0; --i)
        s.write(buf[i - 1]);
}

template <typename Stream, typename Unsigned>
void send_hex(Stream & s, Unsigned v, uint8_t width = 0, char fill = '0')
{
    static char const digits[] = "0123456789ABCDEF";

    char buf[32];
    uint8_t i = 0;

    if (v == 0)
    {
        buf[i++] = '0';
    }
    else if (v < 0)
    {
        buf[i++] = '*';
    }
    else
    {
        for (; v != 0; v >>= 4)
        {
            buf[i++] = digits[v & 0xF];
        }
    }

    while (i < width)
        buf[i++] = fill;
    
    for (; i > 0; --i)
        s.write(buf[i - 1]);
}

template <typename Stream, typename Signed>
void send_shex(Stream & s, Signed v, uint8_t width = 0, char fill = ' ')
{
    static char const digits[] = "0123456789ABCDEF";

    char buf[32];
    uint8_t i = 0;

    bool negative = (v < 0);
    if (negative)
        v = -v;

    if (v == 0)
    {
        buf[i++] = '0';
    }
    else
    {
        for (; v != 0; v >>= 4)
        {
            buf[i++] = digits[v & 0xF];
        }
    }

    if (negative)
        buf[i++] = '-';

    while (i < width)
        buf[i++] = fill;
    
    for (; i > 0; --i)
        s.write(buf[i - 1]);
}

template <typename Stream, typename Integer>
void send_int(Stream & s, Integer v, uint8_t width = 0, char fill = ' ')
{
    char buf[32];
    uint8_t i = 0;
    bool negative = false;

    if (v == 0)
    {
        buf[i++] = '0';
    }
    else 
    {
        if (v < 0)
        {
            negative = true;
            v = -v;
        }
        
        for (; v != 0; v /= 10)
        {
            buf[i++] = (v % 10) + '0';
        }
    }
    
    if (negative)
        buf[i++] = '-';

    while (i < width)
        buf[i++] = fill;
    
    for (; i > 0; --i)
        s.write(buf[i - 1]);
}

template <typename Stream, typename T>
void send_bin(Stream & s, T const & t)
{
    char const * ptr = reinterpret_cast<char const *>(&t);
    for (uint8_t i = 0; i < sizeof t; ++i)
        s.write(ptr[i]);
}

template <typename Stream>
uint8_t readline(Stream & s, uint8_t * buffer, uint8_t len)
{
    uint8_t i = 0;

    while (i < len)
    {
        while (s.empty())
        {
            s.process_rx();
            s.process_tx();
        }

        uint8_t ch = s.read();
        buffer[i++] = ch;

        if (ch == '\n')
            break;
    }

    return i;
}

template <int N>
bool bufcmp(uint8_t const * buf, uint8_t len, char const (&pattern)[N])
{
    for (int i = 0; i < N - 1; ++i)
        if (buf[i] != pattern[i])
            return false;
    return true;
}

template <typename Stream, typename Pattern>
class format_impl
{
public:
    format_impl(Stream & out, Pattern const & pattern)
        : m_out(out), m_pattern(pattern)
    {
        this->write_literal();
    }
    
    format_impl & operator%(const char& ch)
    {
        if(m_pattern.empty())
            return *this;
        m_out.write(ch);
        m_pattern.pop();
        this->write_literal();
        return *this;
    }

    format_impl & operator%(char const * str)
    {
        if(m_pattern.empty())
            return *this;
        while (*str)
            m_out.write(*str++);
        m_pattern.pop();
        this->write_literal();
        return *this;
    }

    format_impl & operator%(const bool& v)
    {
        if(m_pattern.empty())
            return *this;
        m_out.write(v ? '1' : '0');
        m_pattern.pop();
        this->write_literal();
        return *this;
    }

    format_impl & operator%(const float& v)
    {
        if(m_pattern.empty())
            return *this;
        
        static const int ft_size = 12;
        char float_text[ft_size];
        char * float_text_ptr = float_text;
        snprintf(float_text, ft_size,"%g", v);    
        while (*float_text_ptr)
            m_out.write(*float_text_ptr++);

        m_pattern.pop();
        this->write_literal();
        return *this;
    }

    format_impl & operator%(const double& v)
    {
        if(m_pattern.empty())
            return *this;
    
        static const int dt_size = 20;
        char double_text[dt_size];
        char * double_text_ptr = double_text;
        snprintf(double_text, dt_size,"%f", v);    
        while (*double_text_ptr)
            m_out.write(*double_text_ptr++);

        m_pattern.pop();
        this->write_literal();
        return *this;
    }

    template <typename T>
    format_impl & operator%(T const & t)
    {
        if(m_pattern.empty())
            return *this;
        char f = m_pattern.top();
        bool hex =  false;
        bool bin =  false;
        uint8_t width = 0;
        if(f == 'x')
        {
            hex = true;
            m_pattern.pop();
            f = m_pattern.top();
        }
        else if(f == 'b')
        {
            bin = true;
            m_pattern.pop();
            f = m_pattern.top();
        }
        if(f >='0' && f <= '9')
            width = f - '0';
        if (hex)
            send_hex(m_out, t, width);
        else if (bin)
            send_bin_text(m_out, t, width);
        else
            send_int(m_out, t, width);
        if(!m_pattern.empty())
            m_pattern.pop();
        this->write_literal();
        return *this;
    }

private:
    void write_literal()
    {
        bool escape = false;
        for (; !m_pattern.empty(); m_pattern.pop())
        {
            char ch = m_pattern.top();

            if (ch == '%')
            {
                if (escape)
                {
                    m_out.write('%');
                    m_out.write('%');
                    escape = false;
                }
                else
                {
                    escape = true;
                }
            }
            else
            {
                if (escape)
                    break;
                m_out.write(ch);
            }
        }
    }

    Stream & m_out;
    Pattern m_pattern;
};

class string_literal_range
{
public:
    string_literal_range(char const * pattern)
        : m_pattern(pattern)
    {
    }

    bool empty() const
    {
        return *m_pattern == 0;
    }

    char top() const
    {
        return *m_pattern;
    }

    void pop()
    {
        ++m_pattern;
    }

private:
    char const * m_pattern;
};

template <typename Stream>
format_impl<Stream, string_literal_range> format(Stream & out, char const * pattern)
{
    return format_impl<Stream, string_literal_range>(out, string_literal_range(pattern));
}

} // end of namespace ev3cxx
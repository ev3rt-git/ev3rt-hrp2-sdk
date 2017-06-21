#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include <stdint.h>
#ifndef ATOMS_NO_EXCEPTION
    #include <stdexcept>
#endif

// Be aware! ARM is dumb! It doesn't support unaligned access to memory and
// instead of emulation of failure it silently rounds the address to the first
// aligned address available. Because of this, new dependency to memcpy 
// has to be introduced
#ifdef __arm__
    #include <cstring>
#endif

namespace atoms {

// Avakar packet structure:
//     0x80 (8bit), size (4bit), command (4bit), data (up to 15 bytes)
// Warning: if symbol ATOMS_NO_EXCEPTION is defined, no range checks are
// performed!
template <class CharType>
class AvakarPacketTempl {
public:
    AvakarPacketTempl() : buffer{0x80, 0}, state(State::EMPTY), idx(2) {}

    // clears the packet and allows its re-usage
    void clear() {
        buffer[0] = 0x80;
        buffer[1] = 0;
        state = State::EMPTY;
        idx = 2;
    }
    
    // size of the whole formatted packet
    CharType raw_size() const {
        return 2 + size();
    }
    
    // size of the data part
    CharType size() const {
        return buffer[1] & 0xF;
    }
    
    // pointer to buffer with formatted packet
    const CharType* raw() const {
        return reinterpret_cast<const CharType*>(&buffer[0]);
    }
    
    // pointer to begin of buffer with formatted packet
    const CharType* begin() const {
        return raw();
    }

    // pointer to end of buffer with formatted packet
    const CharType* end() const {
        return raw() + raw_size();
    }

    // retrieve command from formatted packet
    CharType get_command() const {
        return buffer[1] >> 4;
    }
    
    // set command for packet, range 0-15
    // marks packet as completed
    void set_command(CharType command) {
        state = State::DONE;
        buffer[1] &= 0xF;
        buffer[1] |= command << 4;
    }
    
    // appends given data to the end of the packet
    // if packet size overflows, exception is thrown (unless symbol
    // ATOMS_NO_EXCEPTION is defined - then it leads to undefined behavior)
    // marks packet as completed
    template <class T>
    void push(T data) {
        #ifndef ATOMS_NO_EXCEPTION
            if (state != State::DONE && state != State::EMPTY)
                throw std::runtime_error("Avakar packet: get on incomplete packet");
        #endif
        CharType o = raw_size();
        increase_size(sizeof(T));
        state = State::DONE;

        #ifdef __arm__
            memcpy(buffer + o, &data, sizeof(T));
        #else
            T* ptr = reinterpret_cast<T*>(buffer + o);
            *ptr = data;
        #endif
    }

    // appends array of n same vales to packet, see push
    // marks packet as completed
    template <class T>
    void push_n(const T* data, uint8_t n) {
         #ifndef ATOMS_NO_EXCEPTION
            if (state != State::DONE && state != State::EMPTY)
                throw std::runtime_error("Avakar packet: get on incomplete packet");
        #endif
        CharType o = raw_size();
        increase_size(n * sizeof(T));
        state = State::DONE;

        #ifdef __arm__
            memcpy(buffer + o, &data, n * sizeof(T));
        #else
            T* ptr = reinterpret_cast<T*>(buffer + o);
            for (uint8_t i = 0; i != n; i++, ptr++)
                *ptr = data[i]; 
        #endif
    }
    
    // deserialize the packet using the byte. If the byte leads to complete
    // deserialization, true is returned.
    // calling on completed packet without symbol ATOMS_NO_EXCEPTION defined
    // causes exception throw, otherwise leads to undefined behavior
    bool push_byte(CharType c) {
        #ifndef ATOMS_NO_EXCEPTION
            if (state == State::DONE)
                throw std::runtime_error("Avakar packet: push_byte on completed packet");
        #endif
        uint8_t cc = c;
        switch(state) {
        case State::EMPTY:
            if (cc == 0x80)
                state = State::HEADER;
            break;
        case State::HEADER:
            buffer[1] = cc;
            state = State::COMMAND;
            break;
        case State::COMMAND:
        case State::IN_PROGRESS:
            buffer[idx] = cc;
            idx++;
            if (idx == raw_size())
                state = State::DONE;
        case State::DONE:
            break; // Do nothing
        }
        return complete();
    }
    
    // true if the packet is completed and consistent
    bool complete() {
        return state == State::DONE;
    }

// Since ARM handles unaligned access in a wrong way, it isn't safe to keep 
// this method around
#ifdef __arm__
private:
#endif
    // retrieves pointer to element in packet to access array of values in packet
    // weak out of range check
    // see get
    template <class T>
    const T* get_ptr(uint8_t index) {
        #ifndef ATOMS_NO_EXCEPTION
            if (state != State::DONE)
                throw std::runtime_error("Avakar packet: get on incomplete packet");
            if (index + sizeof(T) > static_cast<uint8_t>(size()))
                throw std::runtime_error("Avakar packet: index out of range");
        #endif
        return reinterpret_cast<T*>(buffer + 2 + index);
    }
#ifdef __arm__
public:
#endif
    
    // retrieve item from completed packet at given index
    // can be called only on completed package
    // if data is out of range, exception is thrown unless symbol ATOMS_NO_EXCEPTION
    // is defined
    template <class T>
    T get(uint8_t index) {
         #ifndef ATOMS_NO_EXCEPTION
            if (state != State::DONE)
                throw std::runtime_error("Avakar packet: get on incomplete packet");
            if (index + sizeof(T) > static_cast<uint8_t>(size()))
                throw std::runtime_error("Avakar packet: index out of range");
        #endif
        #ifdef __arm__
            T ret;
            memcpy(&ret, get_ptr<CharType>(index), sizeof(T));
            return ret;
        #else
            return *get_ptr<T>(index);
        #endif
    }

private:
    enum class State { EMPTY, HEADER, COMMAND, IN_PROGRESS, DONE };
    
    uint8_t buffer[2 + 15];
    State state;
    uint8_t idx;
    
    void increase_size(CharType s) {
        #ifndef ATOMS_NO_EXCEPTION
            if (size() + s > 15)
                throw std::runtime_error("Avakar packet: size overflow!");
        #endif
        buffer[1] += s;
    }
};

using AvakarPacketS = AvakarPacketTempl<int8_t>;
using AvakarPacketU = AvakarPacketTempl<uint8_t>;
using AvakarPacket = AvakarPacketTempl<char>;
 
}
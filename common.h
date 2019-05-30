// generic tools
// Created by dell on 2019/4/17.
//

#ifndef GAMEGIRL_COMMON_H
#define GAMEGIRL_COMMON_H
#include <cstdint>

    using Byte = uint8_t;
    using Word = uint16_t;
    using SByte = int8_t;

    inline constexpr Word operator "" _kb(const unsigned long long w){
        return w << 10;
    };

    template <Byte p>
    Byte getBit(const Byte b){
        return (b & (1 << p) ) >> p;
    };
    template <Byte p>
    void setBit(Byte & b){
        b |= (1 << p);
    }
    template <Byte p>
    void resetBit(Byte & b){
        b &= ~(1 << p);
    };

    enum class RegistersName: Word{
            IF = 0xFFFF
        };

#endif //GAMEGIRL_COMMON_H

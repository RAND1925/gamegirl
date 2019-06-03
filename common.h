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

    Byte getBit(const Byte b, const Byte p);
    void setBit(Byte & b, const Byte p);
    void resetBit(Byte & b, const Byte p);


#endif //GAMEGIRL_COMMON_H

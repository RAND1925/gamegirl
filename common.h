// generic tools
// Created by dell on 2019/4/17.
//

#ifndef GAMEGIRL_COMMON_H
#define GAMEGIRL_COMMON_H
#include <cstdint>

    using Byte = std::uint8_t;
    using Word = std::uint16_t;
    using SByte = std::int8_t;
    using std::uint32_t;
    inline constexpr unsigned long long operator "" _kByte(const unsigned long long w){
        return w << 10;
    };
    Byte getBit(const Byte b, Byte p);
    void setBit(Byte & b, Byte p);
    void resetBit(Byte & b, Byte p);

#endif //GAMEGIRL_COMMON_H

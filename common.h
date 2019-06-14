// generic tools
// Created by dell on 2019/4/17.
//

#ifndef GAMEGIRL_COMMON_H
#define GAMEGIRL_COMMON_H
#include <cstdint>

    using Byte = uint8_t;
    using Word = uint16_t;
    using SByte = int8_t;

    inline constexpr unsigned long long operator "" _kByte(const unsigned long long w){
        return w << 10;
    };
    inline constexpr unsigned long long operator "" _kbit(const unsigned long long w){
        return w << 7;
    };
/*constexpr unsigned long long RAM_BYTE_SIZE{8_kByte};
constexpr unsigned long long ROM_BYTE_SIZE{16_kByte};
inline constexpr unsigned long long operator "" _romBanks(const unsigned long long w){
    return w * ROM_BYTE_SIZE;
};
inline constexpr unsigned long long operator "" _ramBanks(const unsigned long long w){
    return w * RAM_BYTE_SIZE;
};*/
    Byte getBit(const Byte b, Byte p);
    void setBit(Byte & b, Byte p);
    void resetBit(Byte & b, Byte p);

#endif //GAMEGIRL_COMMON_H

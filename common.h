// generic tools
// Created by dell on 2019/4/17.
//

#ifndef CPPGB_COMMON_H
#define CPPGB_COMMON_H
#include <stdint.h>

    using Byte = uint8_t;
    using Word = uint16_t;
    using SByte = int8_t;

    inline constexpr Word operator "" _kb(const unsigned long long w){
        return w << 10;
    }

#endif //CPPGBUI_COMMON_H

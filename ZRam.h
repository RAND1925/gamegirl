// FE00 - FFFF will be advanced after other parts completed
// Created by dell on 2019/5/8.
//

#ifndef CPPGB_ZRAM_H
#define CPPGB_ZRAM_H

#include "common.h"
#include "AddressSpace.h"
#include <array>

template <Word offset, Word length>
class ZRam: public AddressSpace {
    std::array<Byte, length> bytes;
    bool accepts(Word address) override{
        return address >= offset && address < offset + length;
    }
    Byte getByte(Word address) override {
        return  bytes[address - offset];
    }
    void setByte(Word address, Byte value) override {
        bytes[address - offset] = value;
    }
};
#endif //CPPGB_ZRAM_H

// FE00 - FFFF will be advanced after other parts completed
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ZRAM_H
#define GAMEGIRL_ZRAM_H

#include "common.h"
#include "AddressSpace.h"
#include <array>

template <Word offset, Word length>
class ZRam: public AddressSpace {
    std::array<Byte, length> bytes;
public:
    bool accepts(Word address) override{
        return address >= offset && address < offset + length;
    }
    Byte getByte(Word address) override {
        return  bytes[address - offset];
    }
    void setByte(Word address, Byte value) override {
        bytes[address - offset] = value;
    }

    Byte & bindRegister(Word address){
        return bytes[address];
    }
};
#endif //GAMEGIRL_ZRAM_H

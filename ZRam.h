// FE00 - FFFF will be advanced after other parts completed
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ZRAM_H
#define GAMEGIRL_ZRAM_H

#include <array>
#include "common.h"
#include "AddressSpace.h"

class ZRam: public AddressSpace {
    const static Word offset = 0xFF80;
    const static Word length = 0x007F;
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
};
extern ZRam zRam;
#endif //GAMEGIRL_ZRAM_H

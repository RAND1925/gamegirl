// C000-FE00 include work ram & echo ram(fake)
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_WRAM_H
#define GAMEGIRL_WRAM_H

#include "common.h"
#include "AddressSpace.h"
#include <array>

class WRam: public AddressSpace {
    const static Word offset = 0xC000;
    const static Word length = 0x3E00 ;
    std::array<Byte, length> bytes;
public:
    bool accepts(Word address) override{
        return address >= offset && address < offset + length;
    }
    Byte getByte(Word address) override {
        return  bytes[address & 0x1FFF];
    }
    void setByte(Word address, Byte value) override {
        bytes[address & 0x1FFF] = value;
    }
    WRam(){};
};

extern WRam wRam;


#endif //GAMEGIRL_WRAM_H

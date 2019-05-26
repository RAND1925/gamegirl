// C000-FE00 include work ram & echo ram(fake)
// Created by dell on 2019/5/8.
//

#ifndef CPPGB_WRAM_H
#define CPPGB_WRAM_H

#include "common.h"
#include "AddressSpace.h"
#include <array>

template <Word offset, Word length>
class WRam: public AddressSpace {
    
    
    std::array<Byte, length> bytes;

    
    bool accepts(Word address) override{
        return address >= offset && address < offset + length;
    }
    Byte getByte(Word address) override {
        return  bytes[address & 0x1FFF];
    }
    void setByte(Word address, Byte value) override {
        bytes[address & 0x1FFF] = value;
    }
};


#endif //CPPGB_WRAM_H

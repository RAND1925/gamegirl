//
// Created by dell on 2019/5/8.
// C000-FE00 include work ram & echo ram

#ifndef CPPGB_WRAM_H
#define CPPGB_WRAM_H

#include "common.h"
#include "AddressSpace.h"
#include <vector>

using std::vector;

template <Word offset, Word length>
class WRam: public AddressSpace {
    vector<Byte> bytes;
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


#endif //CPPGB_WRAM_H

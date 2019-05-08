//
// Created by dell on 2019/5/7.
//

#ifndef CPPGB_MEMORY_H
#define CPPGB_MEMORY_H

#include "common.h"
#include "AddressSpace.h"
#include <vector>
#include <iterator>

using std::vector;
using std::begin;
using std::end;
class Mem: public AddressSpace {
private:
    vector<Byte> bytes;
    Word offset;
    Word length;
public:
    Mem(Word o, Word l): offset(o), length(l){};
    Mem(Word o, Word l, char* c): offset(o), length(l), bytes(c, c + l){};

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


#endif //CPPGB_MEMORY_H

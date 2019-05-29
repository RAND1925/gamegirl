//
// Created by dell on 2019/4/17.
//

#include "MMU.h"

Byte MMU::readByte(Word address){
    for (AddressSpace *s : spaces){
        if (s->accepts(address)){
            return s->getByte(address);
        }
    }

}

Word MMU::readWord(Word address){
    return readByte(address) | (readByte(address + 1) << 8);
}

SByte MMU::readSByte(Word address){
    Byte b = readByte(address);
    return b > 127 ? b - 256: b;
}

void MMU::writeByte(Word address, Byte value){
    for (auto & s: spaces){
        if (s->accepts(address)){
            s->setByte(address, value);
        }
    }
}

void MMU::writeWord(Word address, Word value){
    writeByte(address, (value & 0xFF));
    writeByte(address + 1, (value >> 8));
}
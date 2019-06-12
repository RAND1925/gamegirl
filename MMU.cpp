//
// Created by dell on 2019/4/17.
//

#include <iostream>
#include "MMU.h"

MMU mmu;
Byte MMU::readByte(Word address){
    if (address < 0x100){
        return bios[address];
    }
    for (AddressSpace *s : spaces){
        if (s->accepts(address)){
            return s->getByte(address);
        }
    }
    //std::cout << address << ":unused[read]" << std::endl;
    return unusedSpaces[address];
}

Word MMU::readWord(Word address){
    return readByte(address) | readByte((Word)(address + 1)) << 8;
}

void MMU::writeByte(Word address, Byte value){
    for (auto & s: spaces){
        if (s->accepts(address)){
            s->setByte(address, value);
            return;
        }
    }
    //std::cout << address << ":unused[write]" << std::endl;
    unusedSpaces[address] = value;
}

void MMU::writeWord(Word address, Word value){
    writeByte(address, (value & 0xFF));
    writeByte(address + 1, (value >> 8));
}
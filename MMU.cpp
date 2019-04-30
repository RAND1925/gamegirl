//
// Created by dell on 2019/4/17.
//

#include "MMU.h"
Byte MMU::readByte(Word address)
{
    return (this->rom.memory)[address];
}
Word MMU::readWord(Word address){
    return readByte(address) + (readByte(address + 1) << 8);
}
SByte MMU::readSByte(Word address) {
	return (this->rom.memory)[address];
}
void MMU::writeByte(Word address, Byte value){
    (this->rom.memory)[address] = value;
}

void MMU::writeWord(Word address, Word value){
    writeByte(address, (value & 0xFF));
    writeByte(address, (value >> 8));
}
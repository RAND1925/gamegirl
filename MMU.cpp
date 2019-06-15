//
// Created by dell on 2019/4/17.
//

#include <algorithm>
#include "MMU.h"
#include "Exceptions.h"

MMU mmu;

AddressSpace *MMU::findAddressSpace(Word address) {
    for (auto s: spaces){
        if (s->accepts(address))
            return s;
    }
    return nullptr;
}

Byte MMU::readByte(Word address){
    try{
        auto s = findAddressSpace(address);
        if (s == nullptr) {
            throw WrongAddressException("mmu[read]", address);
        }
        return s->getByte(address);
    } catch (...) {
        return unusedSpaces[address];
    }
}

Word MMU::readWord(Word address){
    return readByte(address) | readByte((Word)(address + 1)) << 8;
}

void MMU::writeByte(Word address, Byte value){
    try{
        auto s = findAddressSpace(address);
        if (s == nullptr) {
            throw WrongAddressException("mmu[write]", address);
        }
        s->setByte(address, value);
    } catch (...){
        unusedSpaces[address] = value;
    }
}

void MMU::writeWord(Word address, Word value){
    writeByte(address, (value & 0xFF));
    writeByte(address + 1, (value >> 8));
}

void MMU::addAddressSpace(AddressSpace *s) {
    spaces.push_back(s);
}

void MMU::removeAddressSpace(AddressSpace *s) {
    spaces.erase(std::remove(spaces.begin(), spaces.end(), s), spaces.end());
}

void MMU::init() {
    unusedSpaces = new Byte[0x10000];
}

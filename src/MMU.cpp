//
// Created by dell on 2019/4/17.
//

#include <algorithm>
#include <functional>
#include "MMU.h"
#include "Exceptions.h"

AddressSpace* MMU::findAddressSpace(Word address) {
    for (auto s: spaces){
        if (s->accepts(address))
            return s;
    }
    return nullptr;
}

Byte MMU::readByte(Word address){
#ifndef NO_ADDRESS_ERROR
    try{
        auto s = findAddressSpace(address);
        if (s == nullptr) {
            throw WrongAddressException("mmu[read]", address);
        }
        return s->getByte(address);
    } catch (...) {
        return unusedSpaces[address];
    }
#endif
    assert(findAddressSpace(address));
    return findAddressSpace(address)->getByte(address);
}

Word MMU::readWord(Word address){
    return readByte(address) | (readByte((address + 1)) << 8);
}

void MMU::writeByte(Word address, Byte value){
#ifndef NO_ADDRESS_ERROR
    try{
        auto s = findAddressSpace(address);
        if (s == nullptr) {
            throw WrongAddressException("mmu[write]", address);
        }
        s->setByte(address, value);
    } catch (...){
        unusedSpaces[address] = value;
    }
#endif
    assert(findAddressSpace(address));
    findAddressSpace(address)->setByte(address, value);
}

void MMU::writeWord(Word address, Word value){
    writeByte(address, (value & 0xFFu));
    writeByte(address + 1, (value >> 8u));
}

void MMU::addAddressSpace(AddressSpace *s) {
    spaces.push_back(s);
}

void MMU::removeAddressSpace(AddressSpace *s) {
    spaces.erase(std::remove(spaces.begin(), spaces.end(), s), spaces.end());
}

void MMU::init() {
#ifndef NO_ADDRESS_ERROR
    unusedSpaces = new Byte[0x10000];
#endif
}

MMU *MMU::getMMU() {
    static MMU mmu;
    return &mmu;
}

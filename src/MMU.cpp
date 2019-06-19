//
// Created by dell on 2019/4/17.
//

#include <algorithm>
#include "MMU.h"
#include "Exceptions.h"

AddressSpace *MMU::findAddressSpace(Word address) {
    for (auto s: spaces){
        if (s->accepts(address))
            return s;
    }
    return nullptr;
}

Byte MMU::readByte(Word address){
    auto s = findAddressSpace(address);
    return s->getByte(address);
}

Word MMU::readWord(Word address){
    return readByte(address) | readByte((Word)(address + 1)) << 8;
}

void MMU::writeByte(Word address, Byte value){
    auto s = findAddressSpace(address);
    s->setByte(address, value);

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
}

MMU *MMU::getMMU() {
    static MMU mmu;
    return &mmu;
}

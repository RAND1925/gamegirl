//
// Created by dell on 2019/6/15.
//

#include "Cartridge_ROM0.h"

bool Cartridge_ROM0::accepts(Word address) {
    return address < 0x8000;
}

Byte Cartridge_ROM0::getByte(Word address) {
    return  rom[address];
}

void Cartridge_ROM0::setByte(Word address, Byte value) {
    rom[address] = value;
    // throw WrongAddressException("Rom0[write]", address);
}

Cartridge_ROM0::Cartridge_ROM0(std::ifstream &s) {
    char buffer[0x8000];
    s.read(buffer, 0x8000);
    std::copy(buffer, buffer + 0x8000, rom.begin());
}

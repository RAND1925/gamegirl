//
// Created by dell on 2019/6/15.
//

#include "Cartridge_ROM0.h"
#include <fstream>

bool Cartridge_ROM0::accepts(Word address) {
    return address < 0x8000;
}

Byte Cartridge_ROM0::getByte(Word address) {
    return rom[address];
}

void Cartridge_ROM0::setByte(Word address, Byte value) {
    rom[address] = value;
}

Cartridge_ROM0::Cartridge_ROM0(const std::string & filePath):filePath(filePath) {
    char* buffer = new char[0x8000];
    std::ifstream s{filePath, std::ios::binary};
    s.read(buffer, 0x8000);
    std::copy(buffer, buffer + 0x8000, rom.begin());
    delete [] buffer;
}

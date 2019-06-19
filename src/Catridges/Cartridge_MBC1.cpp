//
// Created by dell on 2019/6/15.
//

#include "Cartridge_MBC1.h"

Cartridge_MBC1::Cartridge_MBC1(const std::string &filePath, size_t romSize,
                               size_t ramSize):filePath(filePath), romSize(romSize),ramSize(ramSize) {

    rom.reserve(romSize);
    if (ramSize == 0) {
        ram.reserve(0x2000);
    } else {
        ram.reserve(ramSize);
    }
    char *buffer = new char[romSize];
    std::ifstream s(filePath, std::ios::binary);
    s.read(buffer, romSize);
    std::copy(buffer, buffer + romSize, rom.begin());
    delete [] buffer;
    std::string savePath{filePath};
    savePath.reserve(savePath.length()+1);
    auto dotPos = savePath.rfind('.');
    savePath.replace(dotPos + 1,3, "sav");
    loadData(savePath);
}
bool Cartridge_MBC1::accepts(Word address) {
    return address < 0x8000 || (address >= 0xA000 && address < 0xC000);
}

Byte Cartridge_MBC1::getByte(Word address) {
    Byte page = address >> 12u;
    if (page < 0x4){
        return rom[address];
    } else if (page >= 0x4 && page < 0x8){
        size_t trueAddress = static_cast<size_t>(romBank << 14u) | (address - 0x4000u);

        if (mode == 0) {
            trueAddress += (uint32_t)ramBank << 19u;
        }
        return rom[trueAddress];
    } else if(page == 0xA || page == 0xB){
        return ram[((uint64_t)ramBank << 13u) | (address & 0x1FFFu)];
    }
}

void Cartridge_MBC1::setByte(Word address, Byte value) {
    Byte page = address >> 12u;
    if (page == 0xA || page == 0xB){
        ram[(uint64_t)ramBank << 13u | (address & 0x1FFFu)] = value;
    } else if (page == 0x0 || page == 0x1){
        ramEnabled = (value & 0xFu) == 0xAu;
    } else if (page == 0x2 || page == 0x3){
        value &= 0x1Fu;
        if (value == 0) {
            romBank = 1;
        } else {
            romBank = value;
        }
    }
    else if (page == 0x4 || page == 0x5){
        ramBank = value & 0x3u;
    }
    else if(page == 0x6 || page == 0x7){
        mode = value & 0x1u;
    }
}

void Cartridge_MBC1::loadData(const std::string &savePath) {
    std::ifstream loadFile(savePath, std::ios::binary);
    char* buffer = new char[ramSize];
    loadFile.read(buffer, ramSize);
    std::copy(buffer, buffer + ramSize, ram.begin());
    delete [] buffer;
}

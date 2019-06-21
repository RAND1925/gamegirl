//
// Created by dell on 2019/6/15.
//

#include "Cartridge_ROM0.h"
#include <fstream>


Byte Cartridge_ROM0::getByte(Word address) {
    if (address >= 0xA000)
        return ram[address - RAM_OFFSET];
    else
        return rom[address];
}

void Cartridge_ROM0::setByte(Word address, Byte value) {
    if (address >= 0xA000)
        ram[address - RAM_OFFSET] = value;
    else
        rom[address] = value;
}

Cartridge_ROM0::Cartridge_ROM0(const std::string & filePath, bool enableRam, bool enableBattery)
/*:Cartridge(filePath, enableRam, enableBattery) */{
    char* buffer = new char[0x8000];
    std::ifstream s{filePath, std::ios::binary};
    s.read(buffer, 0x8000);
    std::copy(buffer, buffer + 0x8000, rom.begin());
    delete [] buffer;
    if (enableBattery) {
        batteryLoad();
    }
}


void Cartridge_ROM0::batteryLoad() {
    std::ifstream saveStream{savePath, std::ios::binary};
    if (!saveStream){
        return;
    }
    char * buffer = new char[RAM_BANK_SIZE];
    saveStream.read(buffer, RAM_BANK_SIZE);
    std::copy(buffer, buffer + RAM_BANK_SIZE, ram.begin());
    delete [] buffer;
    saveStream.close();
}

void Cartridge_ROM0::batterySave() {
    std::ofstream saveStream{savePath, std::ios::binary};
    char * buffer = new char[RAM_BANK_SIZE];
    std::copy(ram.begin(), ram.end(), buffer);
    saveStream.write(buffer, RAM_BANK_SIZE);
    delete [] buffer;
    saveStream.close();
}

Cartridge_ROM0::~Cartridge_ROM0() {
    if (enableBattery) {
        batterySave();
    }
}

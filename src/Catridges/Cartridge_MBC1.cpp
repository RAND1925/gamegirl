//
// Created by dell on 2019/6/15.
//

#include "Cartridge_MBC1.h"

Cartridge_MBC1::Cartridge_MBC1(const std::string & filePath, bool enableRam, bool enableBattery, size_t romBankNum,
                               size_t ramBankNum): romBankNum(romBankNum), ramBankNum(ramBankNum) {

    this->filePath = filePath;
    this->enableRam = enableRam;
    this->enableBattery = enableBattery;
    std::ifstream s{filePath, std::ios::binary};
    char *buffer = new char[ROM_BANK_SIZE];
    for (size_t i = 0; i < romBankNum; ++i){
        s.read(buffer, ROM_BANK_SIZE);
        rom.push_back(RomBank{});
        std::copy(buffer, buffer + ROM_BANK_SIZE, rom[i].begin());
    }
    delete [] buffer;
    s.close();
    currRomBank = rom[0].begin();
    romBank0 = rom[0].begin();

    ram.reserve(ramBankNum);
    if (enableBattery){
        batteryLoad();
    } else {
        for (size_t i = 0; i < (ramBankNum > 1 ? ramBankNum : 1); ++i) {
            ram.at(i) = RamBank{};
        }
    }
    currRamBank = ram.at(0).begin();

}

Byte Cartridge_MBC1::getByte(Word address) {
    Byte page = address >> 12u;
    if (page < 0x4){
        return romBank0[address];
    } else if (page >= 0x4 && page < 0x8){
        return currRomBank[address - SWITCH_ROM_OFFSET];
    } else if(page == 0xA || page == 0xB){
        return currRamBank[address & 0x1FFFu];
    }
}

void Cartridge_MBC1::setByte(Word address, Byte value) {
    Byte page = address >> 12u;
    if (page == 0xA || page == 0xB ){
        if (ramEnabled){
            currRamBank[address & 0x1FFFu] = value;
        }
    } else if (page == 0x0 || page == 0x1){
        ramEnabled = (value & 0xFu) == 0xAu;
    } else {
        // change mapping mode
        if (page == 0x2 || page == 0x3) {
            value &= 0x1Fu;
            if (value == 0) {
                romBankCode = 1;
            } else {
                romBankCode = value;
            }
        } else if (page == 0x4 || page == 0x5) {
            ramBankCode = value & 0x3u;
        } else if (page == 0x6 || page == 0x7) {
            mode = value & 0x1u;
            // 0 - use ram 1 - use upper rom
        }
        // change current bank
        if (mode == 0) {
            currRomBank = rom[romBankCode].begin();
        } else {
            currRomBank = rom[(ramBankCode << 5u) | romBankCode].begin();
        }
        currRamBank = ram[ramBankCode].begin();
    }
}



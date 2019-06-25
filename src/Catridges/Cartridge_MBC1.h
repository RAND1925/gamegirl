//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGEMBC1_H
#define GAMEGIRL_CARTRIDGEMBC1_H

#include <fstream>
#include <vector>
#include "Cartridge.h"
#include "../common.h"
#include "../Exceptions.h"
class Cartridge_MBC1: public Cartridge{
public:
    Cartridge_MBC1(const std::string & filePath, bool enableRam, bool enableBattery, size_t romSize, size_t ramSize);
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;

private:
    size_t romBankNum = 0;
    std::vector<RomBank> rom{};
    RomBank::iterator currRomBank;
    RomBank::iterator romBank0;
    size_t ramBankNum = 0;
    std::vector<RamBank> ram{};
    RamBank::iterator currRamBank;

    bool ramEnabled;
    Byte romBankCode = 1;
    Byte ramBankCode = 0;// also upprombank
    Byte mode = 0;
};



#endif //GAMEGIRL_CARTRIDGEMBC1_H

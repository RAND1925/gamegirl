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
    Cartridge_MBC1(std::ifstream & s, size_t romSize, size_t ramSize);
    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;

private:
    std::vector<Byte> rom;
    std::vector<Byte> ram;
    bool ramEnabled = false;
    Byte romBank = 1;
    Byte ramBank = 0;// also upprombank
    Byte mode = 0;
};



#endif //GAMEGIRL_CARTRIDGEMBC1_H

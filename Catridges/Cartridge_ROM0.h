//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGE_ROM0_H
#define GAMEGIRL_CARTRIDGE_ROM0_H

#include <fstream>
#include "Cartridge.h"

class Cartridge_ROM0: public Cartridge{
private:
    std::array<Byte, 0x8000> rom;
public:
    bool accepts(Word address) override{
        return address < 0x8000;
    };
    Byte getByte(Word address) override {
        return  rom[address];
    };
    void setByte(Word address, Byte value) override {
        rom[address] = value;
        // throw WrongAddressException("Rom0[write]", address);
    };
    Cartridge_ROM0(std::ifstream & s){
        char buffer[0x8000];
        s.read(buffer, 0x8000);
        std::copy(buffer, buffer + 0x8000, rom.begin());
    };
};



#endif //GAMEGIRL_CARTRIDGE_ROM0_H

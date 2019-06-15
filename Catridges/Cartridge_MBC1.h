//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGEMBC1_H
#define GAMEGIRL_CARTRIDGEMBC1_H

#include <fstream>
#include "Cartridge.h"
#include "../Exceptions.h"

class Cartridge_MBC1: public Cartridge{
private:
    std::vector<Byte> rom;
    std::vector<Byte> ram;
    bool ramEnabled = false;
    Byte romBank = 1;
    Byte ramBank = 0;// also upprombank
    Byte mode = 0;
public:
    Cartridge_MBC1(std::ifstream & s, size_t romSize, size_t ramSize){
        rom.reserve(romSize);
        if (ramSize == 0) {
            ram.reserve(0x8000);
        } else{
            ram.reserve(ramSize);
        }
        char* buffer = new char[romSize];
        if (!buffer)
          return;
        s.read(buffer, romSize);
        std::copy(buffer, buffer + romSize, rom.begin());
        delete [] buffer;
    };
    bool accepts(Word address) override {
        return address < 0x8000 || (address >= 0xA000 && address < 0xC000);
    };
    Byte getByte(Word address) override{
        Byte page = address >> 12;
        if (page < 0x4){
            return rom[address];
        } else if (page >= 0x4 && page < 0x8){
            uint32_t trueAddress = ((uint32_t)romBank << 14u) | (address - 0x4000);

            if (mode == 0) {
                trueAddress += (uint32_t)ramBank << 19u;
            }
            return rom[trueAddress];
        } else if(page >= 0xA && page < 0xC){
            return ram[((uint64_t)ramBank << 13u) | (address & 0x1FFFu)];
        }
        throw WrongAddressException("mbc1[read]", address);
    };
    void setByte(Word address, Byte value) override {
        Byte page = address >> 12;
        if (page == 0xA || page == 0xB){
            ram[(uint64_t)ramBank << 13u | (address & 0x1FFFu)] = value;
        } else if (page == 0x0 || page == 0x1){
            ramEnabled = (value & 0xF) == 0xA;
        } else if (page == 0x2 || page == 0x3){
            value &= 0x1F;
            if (value == 0) {
                romBank = 1;
            } else {
                romBank = value;
            }
        }
        else if (page == 0x4 || page == 0x5){
            ramBank = value & 0x3;
        }
        else if(page == 0x6 || page == 0x7){
            mode = value & 0x1;
        }
        //throw WrongAddressException("mbc1[write]", address);
    };
};



#endif //GAMEGIRL_CARTRIDGEMBC1_H

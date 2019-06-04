// Cartridge and ex-ram
// Created by dell on 2019/6/3.
//

#ifndef GAMEGIRL_CATRIDGE_H
#define GAMEGIRL_CATRIDGE_H

#include "AddressSpace.h"
#include "common.h"
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

class Cartridge: public AddressSpace{
};

class Cartridge_Rom: public Cartridge{
private:
    std::array<Byte, 32_kByte> rom;
public:
    bool accepts(Word address) override{
        return address < 0x8000;
    };
    Byte getByte(Word address) override {
        return  rom[address];
    };
    void setByte(Word address, Byte value) override {
        //  warning("write to rom");
        rom[address] = value;
    };
    Cartridge_Rom(std::ifstream & s){
        char buffer[0x8000];
        s.read(buffer, 0x8000);
        std::copy(buffer, buffer + 0x8000, rom.begin());
   };
};

class Cartridge_Mbc1: public Cartridge{
private:
    std::array<Byte, 0x200000> rom;
    std::array<Byte, 0x8000> ram;
    bool ramEnabled = false;
    Byte romBank = 0;
    Byte ramBank = 0;// also upprombank
    Byte mode = 0;
public:
    Cartridge_Mbc1(std::ifstream & s): ram(){
        char buffer[0x200000];
        s.read(buffer, 0x200000);
        std::copy(buffer, buffer + 0x200000, rom.begin());

    };
    bool accepts(Word address) override {
        return address < 0x8000 || (address >= 0xA000 && address < 0xC000);
    };
    Byte getByte(Word address) override{
        Byte page = address >> 12;
        if (page < 0x4){
            return rom[address];
        } else if (page >= 0x4 && page < 0x8){
            uint32_t trueAddress;
            if (mode == 0) {
                Byte upperRomBank = ramBank;
                if (upperRomBank == 0x60 || upperRomBank == 0x40 || upperRomBank == 0x20){
                    ++upperRomBank;
                }
                trueAddress = (upperRomBank * 0x80000) | (romBank * 0x4000) | address;

            } else{
                trueAddress = (romBank * 0x4000) | address;
            }
            return rom[trueAddress];
        } else if(page >= 0xA && page < 0xC){
            if (mode == 1){
                if (ramEnabled){
                    return ram[ramBank * 0x2000 | address];
                }
            }
            // todo : address not found
        }
    };
    void setByte(Word address, Byte value) override {
        Byte page = address >> 13;
        if (page >= 0xA && page < 0xC){
            ram[ramBank * 0x2000 | address] = value;
        }
        else if (page == 0x0){
            ramEnabled = (value & 0xF) == 0xA;
        }
        else if (page == 0x1){
            value &= 0x1F;
            if (value == 0)
                romBank = 1;
            romBank = value;
          }
        else if (page == 0x2){
            ramBank = value & 0x3;
        }
        else if(page == 0x3){
            mode = value & 0x1;
        }
    };
};


class CartridgeDriver: public AddressSpace{
private:
    Byte cartridgeType;
    Byte romSizeCode;
    Byte ramSizeCode;
    std::string title;
    Cartridge * cartridgePointer;
    constexpr static int ramSizeMap[5] = {0, 2, 8, 32, 128};
    constexpr static int romSizeMap[9] = {32, 64, 128, 256, 512};

    void genCartridge(std::ifstream & stream){
        switch (cartridgeType){
            case 0:
                cartridgePointer = new Cartridge_Rom(stream);
                break;
            case 1:
                cartridgePointer = new Cartridge_Mbc1(stream);
                break;
        }
    }
    void loadRom(std::ifstream & stream){
        char header[0x150] = {0};

        stream.read(header, 0x150);
        cartridgeType = header[0x147];
        romSizeCode = header[0x148];
        ramSizeCode = header[0x149];
        std::copy(header + 0x134, header + 0x142, std::back_insert_iterator<decltype(title)>(title));
        stream.clear();
        stream.seekg(0, std::ios::beg);
        genCartridge(stream);
    };
public:
    CartridgeDriver(std::ifstream & stream) {
        loadRom(stream);
    }
    void reloadRom(std::ifstream & stream){
        delete cartridgePointer;
        loadRom(stream);
    }

    ~CartridgeDriver(){
        delete cartridgePointer;
    }
    Cartridge* getCartridgePointer(){
        return cartridgePointer;
    }
    std::string getTitle(){
        return title;
    }

    bool accepts(Word address) override{
        return cartridgePointer->accepts(address);
    };
    Byte getByte(Word address) override {
        return cartridgePointer->getByte(address);
    };
    void setByte(Word address, Byte value) override {
        cartridgePointer->setByte(address, value);
    };

};
#endif //GAMEGIRL_CATRIDGE_H

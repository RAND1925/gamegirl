// Cartridge and ex-ram
// Created by dell on 2019/6/3.
//

#ifndef GAMEGIRL_CATRIDGE_H
#define GAMEGIRL_CATRIDGE_H


#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "common.h"
#include "Exceptions.h"
#include "AddressSpace.h"

class Cartridge: public AddressSpace{
};

class Cartridge_Rom: public Cartridge{
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
   //     throw WrongAddressException("Rom0[write]", address);
    };
    Cartridge_Rom(std::ifstream & s){
        char buffer[0x8000];
        s.read(buffer, 0x8000);
        std::copy(buffer, buffer + 0x8000, rom.begin());
   };
};

class Cartridge_Mbc1: public Cartridge{
private:
    std::vector<Byte> rom;
    std::vector<Byte> ram;
    bool ramEnabled = false;
    Byte romBank = 1;
    Byte ramBank = 0;// also upprombank
    Byte mode = 0;
public:
    Cartridge_Mbc1(std::ifstream & s, size_t romSize, size_t ramSize){
        rom.reserve(romSize);
        ram.reserve(0x8000);
        char* buffer = new char[romSize];
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
        }
        else if (page == 0x0 || page == 0x1){
            ramEnabled = (value & 0xF) == 0xA;
        }
        else if (page == 0x2 || page == 0x3){
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

constexpr unsigned long long RAM_BYTE_SIZE{8_kByte};
constexpr unsigned long long ROM_BYTE_SIZE{16_kByte};
inline constexpr unsigned long long operator "" _romBanks(const unsigned long long w){
    return w * ROM_BYTE_SIZE;
};
inline constexpr unsigned long long operator "" _ramBanks(const unsigned long long w){
    return w * RAM_BYTE_SIZE;
};

class CartridgeDriver: public AddressSpace{
private:
    Byte cartridgeType;
    Byte romBankCode;
    Byte ramBankCode;
    size_t romSize;
    size_t ramSize;
    std::string title;
    Cartridge * cartridgePointer;
    int ramSizeMap[5] = {0, 2, 8, 32, 128};
    int romSizeMap[3] = {72, 80,96};

    void genCartridge(std::ifstream & stream,size_t romSize,size_t ramSize){
        switch (cartridgeType){
            case 0:
                cartridgePointer = new Cartridge_Rom(stream);
                break;
            case 1:
                cartridgePointer = new Cartridge_Mbc1(stream, romSize, ramSize);
                break;
        }
    }

public:
    void loadRom(std::ifstream & stream);;

    void openFile(const std::string& filePath){
        std::ifstream romStream;

        try {
            romStream.open(filePath, std::ios::binary);
            if (!romStream){
                throw 0;
            }

        } catch(int) {
            std::cerr << "fileNotFound" << std::endl;
            throw;
        } catch(std::ios::failure & e) {
            std::cerr << e.what() << std::endl;
            throw;
        }

        loadRom(romStream);
        std::cout << "loading game:" << getTitle() << std::endl;
        romStream.close();
    }
    void reopenFile(const std::string filePath){
        delete cartridgePointer;
        openFile(filePath);
    }

    CartridgeDriver(std::ifstream & stream) {
        loadRom(stream);
    }
    void reloadRom(std::ifstream & stream){
        delete cartridgePointer;
        loadRom(stream);
    }
    CartridgeDriver(){};
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

extern CartridgeDriver cartridgeDriver;
#endif //GAMEGIRL_CATRIDGE_H

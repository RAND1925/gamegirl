// Cartridge and ex-ram
// Created by dell on 2019/6/3.
//

#ifndef GAMEGIRL_CATRIDGE_H
#define GAMEGIRL_CATRIDGE_H
#include <string>
#include <fstream>
#include "common.h"
#include "AddressSpace.h"
#include "Catridges/Cartridge.h"
#include "Catridges/Cartridge_ROM0.h"
#include "Catridges/Cartridge_MBC1.h"


class CartridgeDriver: public AddressSpace{
public:
    void loadRom(std::ifstream & stream);;
    void openFile(const std::string & filePath);
    void reopenFile(const std::string & filePath);
    Cartridge* getCartridgePointer();
    std::string getTitle();

    bool accepts(Word address) override;;
    Byte getByte(Word address) override;;
    void setByte(Word address, Byte value) override;;
    ~CartridgeDriver();
    static CartridgeDriver *getCartridgeDriver(){
        static CartridgeDriver cartridgeDriver;
        return &cartridgeDriver;
    }

protected:
    CartridgeDriver(){};
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
    void genCartridge(std::ifstream & stream);

};


#endif //GAMEGIRL_CATRIDGE_H

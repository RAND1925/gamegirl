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


class CartridgeDriver {
public:
    void openFile(std::string filePath);

   // void reopenFile(const std::string &filePath);

    Cartridge *getCartridgePointer();

    std::string getTitle();

    ~CartridgeDriver();

    static CartridgeDriver *getCartridgeDriver() {
        static CartridgeDriver cartridgeDriver;
        return &cartridgeDriver;
    }

protected:
    CartridgeDriver() = default;

private:
    Cartridge *cartridgePointer{};
    Byte cartridgeTypeCode{};
    Byte romBankCode{};
    Byte ramBankCode{};
    Byte romBankNum{1};
    Byte ramBankNum{0};
    std::string title;
    std::string filePath;
    const static int ramSizeMap[5];
    const static int romSizeMap[3];
    void genCartridge();

#endif //GAMEGIRL_CATRIDGE_H
};
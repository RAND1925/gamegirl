//
// Created by dell on 2019/6/3.
//

#include "CartridgeDriver.h"
#include "Catridges/Cartridge_ROM0.h"
#include "Catridges/Cartridge_MBC1.h"
#include "Exceptions.h"

const int CartridgeDriver::ramSizeMap[]{0, 1, 1, 4, 16};
const int CartridgeDriver::romSizeMap[]{72, 80, 96};

void CartridgeDriver::openFile(std::string newFilePath) {
    filePath = newFilePath;
    std::ifstream romStream;
    romStream.open(filePath, std::ios::binary);
    if (!romStream) {
        throw FileNotFoundException("driver[open file]", filePath);
    }
    char header[0x150] = {0};
    romStream.read(header, 0x150);
    romStream.close();
#ifndef NLOG
    logger << "game loaded:" << getTitle() << std::endl;
#endif
    cartridgeTypeCode = header[0x147];
    romBankCode = header[0x148];
    if (romBankCode < 0x10) {
        romBankNum = 2u << romBankCode;
    } else {
        romBankNum = romSizeMap[romBankCode - 0x52];
    }

    ramBankCode = header[0x149];
    ramBankNum = ramSizeMap[ramBankCode];
    title.reserve(0x142 - 0x134);
    std::copy(header + 0x134, header + 0x142, title.begin());
    genCartridge();
}

void CartridgeDriver::genCartridge() {
    std::string newPath{filePath};
    switch (cartridgeTypeCode) {
        //ROM0
      /*  case 0:
            cartridgePointer = new Cartridge_ROM0(filePath, false, false);
            break;
        case 8:
            cartridgePointer = new Cartridge_ROM0(filePath, true, false);
            break;
        case 9:
            cartridgePointer = new Cartridge_ROM0(filePath, false, false);
            break;
        //MBC1
        case 1:
            cartridgePointer = new Cartridge_MBC1(filePath, false, false, romBankNum, 0);
            break;
        case 2:
            cartridgePointer = new Cartridge_MBC1(filePath, true, false, romBankNum, ramBankNum);
            break;*/
        case 3:
            cartridgePointer = new Cartridge_MBC1(newPath, true, true, romBankNum, ramBankNum);
            break;
    }
}

CartridgeDriver::~CartridgeDriver() {
    delete cartridgePointer;
}

Cartridge *CartridgeDriver::getCartridgePointer() {
    return cartridgePointer;
}

std::string CartridgeDriver::getTitle() {
    return title;
}

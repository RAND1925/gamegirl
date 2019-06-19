//
// Created by dell on 2019/6/3.
//

#include "CartridgeDriver.h"
#include "Catridges/Cartridge_ROM0.h"
#include "Catridges/Cartridge_MBC1.h"
#include "Exceptions.h"


void CartridgeDriver::openFile(const std::string &filePath) {
    this->filePath = filePath;
    std::ifstream romStream;
    try {
        romStream.open(filePath, std::ios::binary);
        if (!romStream) {
            throw FileNotFoundException("driver[open file]", filePath);
        }
    } catch (...) {
        throw;
    }
#ifndef NLOG
    logger << "game loaded:" << getTitle() << std::endl;
#endif
    char header[0x150] = {0};
    romStream.read(header, 0x150);
    romStream.close();
    cartridgeType = (Byte) header[0x147];
    romBankCode = (Byte) header[0x148];
    if (romBankCode < 0x10) {
        romSize = (32_kByte) << romBankCode;
    } else {
        romSize = romSizeMap[romBankCode - 0x52] * 16_kByte;
    }

    ramBankCode = (Byte) header[0x149];
    ramSize = ramSizeMap[ramBankCode] * 1024;
    title.reserve(0x142 - 0x134);
    std::copy(header + 0x134, header + 0x142, title.begin());

    genCartridge();
}

void CartridgeDriver::genCartridge() {
    switch (cartridgeType) {
        case 0:
            cartridgePointer = new Cartridge_ROM0(filePath);
            break;
        case 1:
        case 2:
        case 3:
            cartridgePointer = new Cartridge_MBC1(filePath, romSize, ramSize);
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

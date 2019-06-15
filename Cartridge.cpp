//
// Created by dell on 2019/6/3.
//

#include "Cartridge.h"

CartridgeDriver cartridgeDriver;

void CartridgeDriver::loadRom(std::ifstream &stream) {
    char header[0x150] = {0};

    stream.read(header, 0x150);
    cartridgeType = (Byte)header[0x147];
    romBankCode = (Byte)header[0x148];
    if (romBankCode < 0x10){
        romSize = 32_kByte << romBankCode;
    } else {
        romSize = romSizeMap[romBankCode - 0x52] * 16_kByte;
    }
    ramBankCode = (Byte)header[0x149];
    ramSize = ramSizeMap[ramBankCode];
    std::copy(header + 0x134, header + 0x142, std::back_insert_iterator<decltype(title)>(title));
    stream.clear();
    stream.seekg(0, std::ios::beg);
    genCartridge(stream, romSize, ramSize);
}

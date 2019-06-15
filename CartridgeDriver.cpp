//
// Created by dell on 2019/6/3.
//

#include "CartridgeDriver.h"

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
    genCartridge(stream);
}
void CartridgeDriver::genCartridge(std::ifstream &stream) {
  switch (cartridgeType){
  case 0:
    cartridgePointer = new Cartridge_ROM0(stream);
    break;
  case 1:
    cartridgePointer = new Cartridge_MBC1(stream, romSize, ramSize);
    break;
  }
}
void CartridgeDriver::openFile(const std::string &filePath) {
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
void CartridgeDriver::reopenFile(const std::string & filePath) {
  delete cartridgePointer;
  openFile(filePath);
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
bool CartridgeDriver::accepts(Word address) {
  return cartridgePointer->accepts(address);
}
Byte CartridgeDriver::getByte(Word address) {
  return cartridgePointer->getByte(address);
}
void CartridgeDriver::setByte(Word address, Byte value) {
  cartridgePointer->setByte(address, value);
}
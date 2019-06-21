#include <utility>

//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGE_H
#define GAMEGIRL_CARTRIDGE_H

#include "../AddressSpace.h"


class Cartridge : public AddressSpace {
protected:
    const static Word ROM_BANK_SIZE = 0x4000;
    const static Word RAM_BANK_SIZE = 0x2000;
    using RomBank = std::array<Byte, ROM_BANK_SIZE>;
    using RamBank = std::array<Byte, RAM_BANK_SIZE>;

    const static Word RAM_OFFSET = 0xA000;
    const static Word SWITCH_ROM_OFFSET = 0x4000;
    const static Word ROM_LENGTH = 0x8000;
    std::string filePath{};
    bool enableRam{false};
    bool enableBattery{false};
    std::string savePath{};
public:
    bool accepts(Word address) final {
        return (address < ROM_LENGTH) || (address >= RAM_OFFSET && address < RAM_OFFSET + RAM_BANK_SIZE);
    }
    Cartridge(){};
  /*  Cartridge(const std::string & filePath, bool enableRam = false, bool enableBattery = false):
    filePath(filePath), enableRam(enableRam), enableBattery(enableBattery){

    };*/
    virtual void batteryLoad(){};
    virtual void batterySave(){};
    virtual ~Cartridge() = default;
};


#endif //GAMEGIRL_CARTRIDGE_H

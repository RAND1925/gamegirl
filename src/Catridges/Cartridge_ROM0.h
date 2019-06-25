//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGE_ROM0_H
#define GAMEGIRL_CARTRIDGE_ROM0_H

#include "Cartridge.h"
#include <array>
#include <fstream>

class Cartridge_ROM0: public Cartridge{
private:
    std::array<Byte, 0x8000> rom{};
    RamBank ram{};
public:
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
    explicit Cartridge_ROM0(const std::string & filePath, bool enableRam, bool enableBattery);
    ~Cartridge_ROM0() override;
    void batterySave() override;
    void batteryLoad() override;
};



#endif //GAMEGIRL_CARTRIDGE_ROM0_H

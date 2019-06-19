//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_CARTRIDGEMBC1_H
#define GAMEGIRL_CARTRIDGEMBC1_H

#include <fstream>
#include <vector>
#include "Cartridge.h"
#include "../common.h"
#include "../Exceptions.h"
#include "Battery.h"
class Cartridge_MBC1: public Cartridge, public Battery{
public:
    Cartridge_MBC1(std::ifstream & s, size_t romSize, size_t ramSize, std::string filePath);
    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;

private:
    size_t ramSize = 0;
    size_t romSize = 0;
    std::string filePath;
    std::vector<Byte> rom;
    std::vector<Byte> ram;
    void saveFile(std::string) override{};
    void loadFile(std::string savePath) override{
        std::ifstream loadFile(savePath, std::ios::binary);
        char* buffer = new char[ramSize];
        loadFile.read(buffer, ramSize);
        std::copy(buffer, buffer + ramSize, ram.begin());
        delete[](buffer);
    };
    bool ramEnabled = false;
    Byte romBank = 1;
    Byte ramBank = 0;// also upprombank
    Byte mode = 0;
};



#endif //GAMEGIRL_CARTRIDGEMBC1_H

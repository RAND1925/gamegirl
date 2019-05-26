#include <iostream>
#include <bitset>
#include <fstream>
#include <memory>

#include "common.h"
#include "MMU.h"
#include "CPU.h"
#include "Rom.h"
#include "WRam.h"
#include "ZRam.h"
#include "GRam.h"
const Word ROM_VOL = 16_kb;
#define FILE_PATH "D:\\Tetris.gb"

int main()
{

    std::ifstream gameFile;
    gameFile.open(FILE_PATH, std::ios::binary);
    char gameBinaryString[ROM_VOL];
    gameFile.read(gameBinaryString, ROM_VOL);
    gameFile.close();

    MMU mmu;
    Rom<0x0000, 32_kb> rom(gameBinaryString, ROM_VOL);
    mmu.addAddressSpace(&rom);
    WRam<0xC000, 8_kb + 0x1E00> wRam;
    mmu.addAddressSpace(&wRam);

    ZRam<0xFF80, 0xFFFF - 0xFF80> zRam;
    mmu.addAddressSpace(&zRam);

    GRam<0xFF00,0xFF00-0xFF7F> gRam;
    mmu.addAddressSpace(&gRam);


    CPU cpu(mmu);
    cpu.cycle();
}
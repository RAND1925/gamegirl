#include <iostream>
#include <bitset>
#include <fstream>
#include <memory>

#include "Mem.h"
#include "MMU.h"
#include "CPU.h"
const Word ROM_VOL = 1 << 14;
#define FILE_PATH "D:\\Tetris.gb"

int main() {

    std::ifstream gameFile;
    gameFile.open(FILE_PATH, std::ios::binary);
    char gameRom[ROM_VOL];
    gameFile.read(gameRom, ROM_VOL);
    gameFile.close();

    MMU mmu;
    Mem rom(0x00, ROM_VOL, gameRom);

    mmu.addAddressSpace(&rom);
    CPU cpu(mmu);
    int p = mmu.readByte(0);
    while (1){
        cpu.step();
    }
    Byte a = mmu.readByte(0x50);
    return 0;

}
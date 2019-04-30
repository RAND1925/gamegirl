#include <iostream>
#include <bitset>
#include <fstream>


#include "Memory.h"
#include "MMU.h"
#include "CPU.h"
const int ROM_VOL = 1 << 16;

int main() {

    std::ifstream gameFile;
    gameFile.open("D:\\Tetris.gb", std::ios::binary);
    char gameRom[ROM_VOL];
    gameFile.read(gameRom, ROM_VOL);
    gameFile.close();

    MMU mmu(gameRom);
    CPU cpu(mmu);

    while (1){
        cpu.step();
    }
    Byte a = mmu.readByte(0x50);
    return 0;

}
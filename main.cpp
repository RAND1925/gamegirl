#include <iostream>
#include <bitset>
#include <fstream>
#include <memory>

#include "common.h"
#include "MMU.h"
#include "CPU.h"
#include "WRam.h"
#include "ZRam.h"
#include "Timer.h"
#include "Cartridge.h"
#define FILE_PATH "D:\\Tetris.gb"

int main() {

    std::ifstream gameFile;
    gameFile.open(FILE_PATH, std::ios::binary);
    CartridgeDriver cartridgeDriver(gameFile);
    gameFile.close();

    std::cout << "loading game:" << cartridgeDriver.getTitle() << std::endl;
    MMU mmu;
    WRam<0xC000, 8_kByte + 0x1E00> wRam;
    mmu.addAddressSpace(&wRam);
    ZRam<0xFF80, 127> zRam;
    mmu.addAddressSpace(&zRam);
    Timer timer(mmu);
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&cartridgeDriver);
    CPU cpu(mmu);
    while(true) {
        Byte timing=cpu.cycle();
        timer.increase(timing);
    }

}
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
#define FILE_PATH "D:\\bgtest.gb"

int main() {

    std::ifstream gameFile;
    gameFile.open("D:\\bgbtest.gb", std::ios::binary);
    CartridgeDriver cartridgeDriver(gameFile);
    std::cout << "loading game:" << cartridgeDriver.getTitle() << std::endl;
    gameFile.close();

    MMU mmu;

    WRam<0xC000, 8_kByte + 0x1E00> wRam;
    ZRam<0xFF40, 0xC0> zRam;
    Timer timer(mmu);
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&cartridgeDriver);
    CPU cpu(mmu);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&cpu);
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&zRam);
    while(true) {
        Byte timing=cpu.cycle();
        timer.increase(timing);
    }
}
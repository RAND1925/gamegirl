#include <iostream>
#include <bitset>
#include <fstream>
#include <memory>

#include "common.h"
#include "MMU.h"
#include "WRam.h"
#include "ZRam.h"
#include "gpu.h"
#include "Timer.h"
#include "Cartridge.h"
#include "InterruptManager.h"
#include "CPU.h"
#include "CycleCounter.h"
#include "Logger.h"
#include "SDLManager.h"

int main(int argc,char** argv) {

    std::ios::sync_with_stdio(false);

    const std::string FILE_PATH("../testRom/Tetris.gb");
    cartridgeDriver.openFile(FILE_PATH);
    sdlManager.init("Tetris");
    cpu.initMap();
    logger.open("a.txt");
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&interruptManager);
    mmu.addAddressSpace(&zRam);
    mmu.addAddressSpace(&gpu);
    cycleCounter.cycle();

}
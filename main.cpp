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

int main(int argc,char** argv) {

    std::cin.sync_with_stdio(0);
    std::cout.sync_with_stdio(0);

    const std::string FILE_PATH("E:/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    cartridgeDriver.openFile(FILE_PATH);

    cpu.initMap();
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&interruptManager);
    mmu.addAddressSpace(&zRam);
    mmu.addAddressSpace(&gpu);
    cycleCounter.cycle();

}
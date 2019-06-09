#include <iostream>
#include <bitset>
#include <fstream>
#include <memory>

#include "common.h"
#include "MMU.h"
#include "WRam.h"
#include "ZRam.h"
#include "Timer.h"
#include "Cartridge.h"
#include "InterruptManager.h"
#include "CPU.h"
#include "CycleCounter.h"

int main() {

    const std::string FILE_PATH("D:\\bgbtest.gb");
    cartridgeDriver.openFile(FILE_PATH);

    cpu.initMap();
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&interruptManager);
    mmu.addAddressSpace(&zRam);

    cycleCounter.cycle();

}
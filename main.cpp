#include "common.h"
#include "MMU.h"
#include "WRam.h"
#include "ZRam.h"
#include "gpu.h"
#include "Timer.h"
#include "Cartridge.h"
#include "InterruptManager.h"
#include "CPU.h"
#include "Logger.h"
#include "SDLManager.h"
#include "Boot.h"


//#define USE_BOOT
uint64_t step();
int main(int argc,char** argv) {

    std::ios::sync_with_stdio(false);

    //const std::string FILE_PATH("../testRom/Tetris.gb");
    const std::string FILE_PATH("E:\\C++project\\gb-test-roms-master\\interrupt_time\\interrupt_time.gb");
    //cpu.initRegisters();
    cartridgeDriver.openFile(FILE_PATH);
    sdlManager.init("Tetris");
    cpu.initMap();
    uint64_t allCycle = 0;
#ifndef NDEBUG
    logger.open("a.txt");
#endif

#ifdef USE_BOOT
    cpu.initRegisters();
    mmu.addAddressSpace(&boot);
#endif
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&gpu);
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&interruptManager);
    mmu.addAddressSpace(&zRam);

#ifdef USE_BOOT
    while (cpu.getPc() < 0x100){
        allCycle += step();
    }
    mmu.removeAddressSpace(&boot);
#else
    cpu.initRegisters();
    cpu.setPc(0x100);
#endif
    while(true){
        allCycle += step();
#ifndef NDEBUG
        logger << "clk:" << allCycle << std::endl;
#endif
    }
}

uint64_t step(){
    Byte cpuCycle = cpu.step();
    timer.addTime(cpuCycle);
    gpu.addTime(cpuCycle);
    return cpuCycle;
}
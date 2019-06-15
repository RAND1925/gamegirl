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
#include "JoyPad.h"

bool isQuit = false;
uint64_t step(){
    isQuit = sdlManager.handleInput();
    joypad.update();
    Byte cpuCycle = cpu.step();
    timer.addTime(cpuCycle);
    gpu.addTime(cpuCycle);
    return cpuCycle;
};

int main(int argc,char** argv) {

    //MODE
    // 00 -boot(run)
    // 01 -boot(no run) initAfterBoot
    // 02 -noboot init (tests)
    // 03 -noboot initAfterBoot (bgbtest)

    int runMode = 3;
    int zoomTime = 4;
    mmu.init();
    std::ios::sync_with_stdio(false);

    const std::string FILE_PATH("../testRom/Tetris.gb");
   // const std::string FILE_PATH("E:\\C++project\\gb-test-roms-master\\mem_timing-2\\rom_singles\\01-read_timing.gb");
    //cpu.initRegisters();
    cartridgeDriver.openFile(FILE_PATH);

    sdlManager.init(cartridgeDriver.getTitle(),zoomTime);
    cpu.initMap();
    uint64_t allCycle = 0;
#ifndef NLOG
    logger.open("a.txt");
#endif

    cpu.initRegisters();

    if (runMode == 0){
        mmu.addAddressSpace(&boot);
    }
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&gpu);
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&joypad);
    mmu.addAddressSpace(&interruptManager);
    mmu.addAddressSpace(&zRam);

    if (runMode == 0) {
        while (cpu.getPc() < 0x100){
            allCycle += step();
        }
        mmu.removeAddressSpace(&boot);
    }
    if (runMode != 2) {
        cpu.initRegistersAfterBoot();
    } else {
        cpu.setPc(0x100);
    }
    while(!isQuit){

        allCycle += step();
#ifndef NLOG
        logger << "clk:" << allCycle << std::endl;
#endif
    }
    return 0;
}

extern "C" {
    #include "getopt.h"
}

#include <cstdlib>
#include <string>
#include "common.h"
#include "MMU.h"
#include "WRam.h"
#include "ZRam.h"
#include "GPU.h"
#include "Timer.h"
#include "CartridgeDriver.h"
#include "InterruptManager.h"
#include "CPU.h"
#include "SDLManager.h"
#include "Boot.h"
#include "Joypad.h"
#include "EmptySpace.h"
#include "Logger.h"


bool isQuit = false;
uint64_t step();
bool init(int argc, char **argv);

int main(int argc, char **argv) {
    int allCycle = 0;
    bool useBoot = init(argc, argv);
    if (useBoot) {
        while (cpu.getPC() < 0x100) {
            allCycle += step();
        }
        mmu.removeAddressSpace(&boot);
    }
    while (!isQuit) {
        allCycle += step();
#ifndef NLOG
        logger << "clk:" << allCycle << std::endl;
#endif
    }
    return 0;
}


bool init(int argc, char **argv) {
    if (argc == 1) {
        throw FileNotFoundException("argument wrong", "no file path ");
    }
    const std::string FILE_PATH(argv[1]);
    uint8_t fps = 60;
    auto xPos = SDL_WINDOWPOS_UNDEFINED;
    auto yPos = SDL_WINDOWPOS_UNDEFINED;
    uint8_t zoomTime = 1;
    bool useBoot = false;
    bool useSprite = true;
    optind++;
    const std::string optString{"bSf:x:y:z:"};
    const char* const optPtr = optString.c_str();
    int option;
    while ((option = getopt(argc, argv, optPtr)) != -1){
        switch (option) {
            case 'b':
                useBoot = true;
                break;
            case 'z':
                zoomTime = std::atoi(optarg);
                break;
            case 'f':
                fps = std::atoi(optarg);
                break;
            case 'x':
                xPos = std::atoi(optarg);
                break;
            case 'y':
                yPos = std::atoi(optarg);
                break;
            case 'S':
                useSprite = false;
            default:
                break;
        }
    }
    std::ios::sync_with_stdio(false);
    cartridgeDriver.openFile(FILE_PATH);
    sdlManager.init(cartridgeDriver.getTitle(), zoomTime, xPos, yPos, fps);
    gpu.init(useSprite);
    cpu.initMap();
    mmu.init();
#ifndef NLOG
    logger.open("gamegirl.log");
#endif
    if (useBoot){
        mmu.addAddressSpace(&boot);
        cpu.initRegisters();
    } else {
        cpu.initRegistersAfterBoot();
    }
    mmu.addAddressSpace(&cartridgeDriver);
    mmu.addAddressSpace(&wRam);
    mmu.addAddressSpace(&zRam);
    mmu.addAddressSpace(&gpu);
    mmu.addAddressSpace(&timer);
    mmu.addAddressSpace(&joypad);
    mmu.addAddressSpace(&interruptManager);
    mmu.addAddressSpace(&emptySpace);
    return useBoot;
}

uint64_t step() {
    isQuit = sdlManager.handleInput();
    joypad.update();
    Byte cpuCycle = cpu.step();
    timer.addTime(cpuCycle);
    gpu.addTime(cpuCycle);
    return cpuCycle;
};


//
// Created by dell on 2019/6/5.
// unit : machine_cycle

#ifndef GAMEGIRL_CYCLECOUNTER_H
#define GAMEGIRL_CYCLECOUNTER_H

#include "MMU.h"
#include "CPU.h"
#include "Timer.h"
#include "gpu.h"
#include "Logger.h"
/*
 *
 *unit
 *clock cycle: 41944304Hz
 *the same as the doc GBCPUman
 *it means cpu needs 4 cycles in NOP
 *while most other emulators use machine cycles...
 * -divider: 41944304
 *
 *tac
 *00 4.096kHz =  64 * 16 cycles
 *01 262.144kHz =  16 cycles about
 *10 65.536kHz=   4 * 16 cycles
 *11 16.384kHz = 16 * 16 cycles
 *div
 *    16384Hz = 16 * 16 cycles

 */

class CycleCounter {
    unsigned long long allCycle;
public:
    void step(){
        Byte cpuCycle = cpu.step();
        allCycle += cpuCycle;
        timer.increase(cpuCycle);
        gpu.addTime(cpuCycle);
#ifndef NDEBUG
        logger << "clk:" << allCycle << std::endl;
#endif
    }
    void cycle(){
        while(true){
            step();
        }
    }

};

extern CycleCounter cycleCounter;
#endif //GAMEGIRL_CYCLECOUNTER_H

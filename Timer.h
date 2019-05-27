//
// Created by jjf09 on 2019/5/24.
//

#ifndef GAMEGIRL_TIMER_H
#define GAMEGIRL_TIMER_H

#include "CPU.h"
#include "common.h"
#include "MMU.h"
#include "AddressSpace.h"
#include <array>

class Timer: public AddressSpace {
private:
    Byte mainClock=0;
    Byte subClock=0;
    Byte divider=0;
    Byte threshold=0;
    Byte regDiv;//reg means register divider ff04
    Byte regTima;//counter ff05
    Byte regTma;//modulator ff06
    Byte regTac;//control ff07
    MMU &mmu;
    bool check();
    bool step();
public:
    Timer(MMU &mmu1):mmu(mmu1){}
    Byte increase(Byte cycle);

    bool accepts(Word address) override{
        return address>=0xFF04 && address<=0xFF07;
    }
    Byte getByte(Word address) override {
        switch (address)
        {
            case 0xFF04: return regDiv;
            case 0xFF05: return regTima;
            case 0xFF06: return regTma;
            case 0xFF07: return regTac;
        }
    }
    void setByte(Word address, Byte value) override {
        switch (address)
        {
            case 0xFF04: regDiv=0;break;
            case 0xFF05: regTima=value;break;
            case 0xFF06: regTma=value;break;
            case 0xFF07: regTac=(value & 7);break;
        }
    }
};


#endif //GAMEGIRL_TIMER_H

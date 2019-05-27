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
    int mainClock=0;
    int counter=0;
    int divider=0;
    int threshold=0;
    Word regDiv=0xFF04;//reg means register divider
    Word regTima=0xFF05;//counter
    Word regTma=0xFF06;//modulator
    Word regTac=0xFF07;//control
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
            case 0xFF04: return mmu.readByte(regDiv);
            case 0xFF05: return mmu.readByte(regTima);
            case 0xFF06: return mmu.readByte(regTma);
            case 0xFF07: return mmu.readByte(regTac);
        }
    }
    void setByte(Word address, Byte value) override {
        switch (address)
        {
            case 0xFF04: mmu.writeByte(regDiv,0);break;
            case 0xFF05: mmu.writeByte(regTima,value);break;
            case 0xFF06: mmu.writeByte(regTma,value);break;
            case 0xFF07: mmu.writeByte(regTac,value & 7);break;
        }
    }
};


#endif //GAMEGIRL_TIMER_H

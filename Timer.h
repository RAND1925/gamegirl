//
// Created by jjf09 on 2019/5/24.
//

#ifndef GAMEGIRL_TIMER_H
#define GAMEGIRL_TIMER_H

#include "CPU.h"
#include "common.h"
#include "AddressSpace.h"
#include <array>

class Timer: public AddressSpace {
private:
    uint64_t counter=0;
    uint64_t divider=0;
    Byte regDiv=0;//reg means register divider ff04
    Byte regTima=0;//counter ff05
    Byte regTma=0;//modulator ff06
    Byte regTac=0;//control ff07
public:
    void addTime(Byte cycle);
    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
};
 extern Timer timer;

#endif //GAMEGIRL_TIMER_H

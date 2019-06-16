//
// Created by jjf09 on 2019/5/24.
//

#include "Timer.h"
#include "InterruptManager.h"
#include "Exceptions.h"

void Timer::addTime(Byte cycle) {
    divider+=cycle;
    if(divider >= 256){
        divider -=256;
        regDiv++;
    }

    if(getBit(regTac,2)==0){//enabled
        return;
    }
    counter+=cycle;
    uint64_t currClock;
    switch(regTac & 0x3){
        case 0: currClock=1024;break;
        case 1: currClock=16;break;
        case 2: currClock=64;break;
        case 3: currClock=256;
    }
    while (counter >= currClock){
        counter-=currClock;
       // counter=0;
        if(regTima==0xFF){
            InterruptManager::getInterruptManager()->requestInterrupt(2);
            regTima=regTma;
        } else {
            regTima++;
        }
    }
}

bool Timer::accepts(Word address) {
    return address>=0xFF04 && address<=0xFF07;
}

Byte Timer::getByte(Word address) {
    switch (address){
        case 0xFF04: return regDiv;
        case 0xFF05: return regTima;
        case 0xFF06: return regTma;
        case 0xFF07: return regTac;
        default:
            throw WrongAddressException("Timer[read]", address);
    }
}

void Timer::setByte(Word address, Byte value) {
    switch (address) {
        case 0xFF04: regDiv=0;break;
        case 0xFF05: regTima=value;break;
        case 0xFF06: regTma=value;break;
        case 0xFF07: regTac=(value & 7);break;
        default:
            throw WrongAddressException("Timer[write]", address);
    }

}

Timer *Timer::getTimer() {
    static Timer timer;
    return &timer;
}


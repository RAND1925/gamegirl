//
// Created by jjf09 on 2019/5/24.
//

#include "Timer.h"

Timer timer;
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
            interruptManager.requestInterrupt(2);
            regTima=regTma;
        } else {
            regTima++;
        }
    }
}
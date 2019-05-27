//
// Created by jjf09 on 2019/5/24.
//

#include "Timer.h"

Byte Timer::increase(Byte cycle) {
    counter+=cycle;
    if(counter>=4){
        mainClock++;
        counter-=4;
        divider++;
        if(divider==16){
            mmu.writeByte(regDiv,(mmu.readByte(regDiv)+1)&255);
            divider=0;
        }
    }
    if(check()){
        return getByte(regTma);
        //interrupt here
    }
    else{
        return NULL;
    }
}
bool Timer::check() {
    if(mmu.readByte(regTac)%4){
        switch (mmu.readByte(regTac)%3){
            case 0:threshold=64;break;
            case 1:threshold=4;break;
            case 2:threshold=4;break;
            case 3:threshold=16;break;
        }
        if(mainClock>threshold){
            if(step())
                return true;
        }
    }
    return false;
}

bool Timer::step() {
    mainClock=0;
    mmu.writeByte(regTima,mmu.readByte(regTima)+1);
    if(mmu.readByte(regTima)>255){
        mmu.writeByte(regTima,mmu.readByte(regTma));
        Byte add=mmu.readByte(0xFF0F);
        add |= (1<<2);
        mmu.writeByte(0xFF0F,add);
        return true;
    }
    return false;
}
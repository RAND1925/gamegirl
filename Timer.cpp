//
// Created by jjf09 on 2019/5/24.
//

#include "Timer.h"

Byte Timer::increase(Byte cycle) {
    subClock+=cycle;
    if(subClock>=4) {
        mainClock++;
        subClock -= 4;
        divider++;
        if (divider == 16) {
            regDiv++;
            divider = 0;
        }
    }
    if(check()){

        return regTma;
    }
    else{
        return 0;
    }
}
bool Timer::check() {
    if(regTac%4){
        switch (regTac%3){
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
    regTima++;
    if(regTima==0){//should be >255 here but it is Byte
        regTima=regTma;
        Byte oldIF=mmu.readByte(0xFF0F);
        setBit(oldIF, 2);
        mmu.writeByte(0xFF0F,oldIF);
        return true;
    }
    return false;
}
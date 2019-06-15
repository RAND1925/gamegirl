//
// Created by dell on 2019/6/5.
//

#ifndef GAMEGIRL_INTERRUPTMANAGER_H
#define GAMEGIRL_INTERRUPTMANAGER_H

#include "common.h"
#include "Exceptions.h"
#include "AddressSpace.h"

class InterruptManager:  public AddressSpace{
    bool iME = false;
    Byte iE = 0x00;//FFFF
    Byte iF = 0xE1;//FF0F
    bool stop = false;
    bool halt = false;
public:
    void requestInterrupt(Byte bit);

    bool hasInterrupt();
    Byte handleInterrupt();
    bool handleHalt();

    void setIF(Byte newIF);
    void setIME(bool newIME);
    bool getIME();
    void setStop(bool newStop);
    void setHalt(bool newHalt);


    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
};

extern InterruptManager interruptManager;

#endif //GAMEGIRL_INTERRUPTMANAGER_H

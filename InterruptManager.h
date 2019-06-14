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
    void requestInterrupt(Byte bit){
        setBit(iF, bit);
    }
    void setIF(Byte newIF){
        iF &= 0xE0;
        iF |= (newIF & 0x1F);
    };
    void setIME(bool newIME){
        iME = newIME;
    }
    bool getIME(){
        return iME;
    }
    void setStop(bool newStop){
        stop = newStop;
    }
    void setHalt(bool newHalt){
        halt = newHalt;
    }
    bool hasInterrupt(){
        Byte maskCode = static_cast<Byte>((iE & iF) & 0x1F);
        return iME && maskCode;
    }

    Byte handleInterrupt(){
        iME = false;
        halt = false;
        Byte maskCode = static_cast<Byte>((iE & iF) & 0x1F);
        for(Byte i = 0; i < 5; ++i){
            if (getBit(maskCode, i)){
                std::cout << "interrupt: " << i << std::endl;
                resetBit(iF, i);
                return i;
            }
        }
        throw InterruptException("", iE, iF);
    }

    bool handleHalt(){
        Byte iFUsed = iF & 0x1F;
        if (!iME && halt && iFUsed){
            halt = false;
            return false;
        }else if (!halt){
            return true;
        }
        return false;
    }
    bool accepts(Word address) override{
        return address == 0xFF0F || address == 0xFFFF;
    }
    Byte getByte(Word address) override{
        if (address == 0xFF0F) {
            return iF;
        } else if (address == 0xFFFF) {
            return iE;
        } else {
            throw WrongAddressException("Interrupt[read]", address);
        }
    }

    void setByte(Word address, Byte value) override{
        if (address == 0xFF0F) {
            setIF(value);
        } else if (address == 0xFFFF) {
            iE = value;
        } else {
            throw WrongAddressException("Interrupt[write]", address);
        }
    }
};

extern InterruptManager interruptManager;

#endif //GAMEGIRL_INTERRUPTMANAGER_H

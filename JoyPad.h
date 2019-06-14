//
// Created by dell on 2019/6/14.
//

#ifndef GAMEGIRL_JOYPAD_H
#define GAMEGIRL_JOYPAD_H

#include "AddressSpace.h"
#include "SDLManager.h"
#include "InterruptManager.h"

class Joypad: public AddressSpace {
public:
    bool accepts(Word address) override {
        return address == 0xFF00;
    }
    Byte getByte(Word address) override {
        return regJoypad;
    }
    void setByte(Word address, Byte value) override{
       regJoypad &= 0xCF;
       regJoypad |= (value & 0x30);
    }
    void update(){
        Byte newJoypad = sdlManager.getJoypad(regJoypad);
        if (newJoypad != regJoypad){
            interruptManager.requestInterrupt(4);
        }
    }
private:
    Byte regJoypad = 0x0F;
};
extern Joypad joypad;

#endif //GAMEGIRL_JOYPAD_H

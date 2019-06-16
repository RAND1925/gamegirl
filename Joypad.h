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
    void update();
    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
    static Joypad* getJoypad(){
        static Joypad joypad;
        return &joypad;
    }

protected:
    Joypad()= default;

private:
    Byte regJoypad = 0x0F;

};


#endif //GAMEGIRL_JOYPAD_H
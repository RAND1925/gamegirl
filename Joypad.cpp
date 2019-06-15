//
// Created by dell on 2019/6/14.
//

#include "Joypad.h"

Joypad joypad;

bool Joypad::accepts(Word address) {
    return address == 0xFF00;
}

Byte Joypad::getByte(Word address) {
    return regJoypad;
}
void Joypad::setByte(Word address, Byte value) {
    regJoypad &= 0xCF;
    regJoypad |= (value & 0x30);
}

void Joypad::update() {
    Byte newJoypad = sdlManager.getJoypad(regJoypad);
    if (newJoypad != regJoypad){
        interruptManager.requestInterrupt(4);
    }
    regJoypad = newJoypad;
}


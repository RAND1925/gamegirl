//
// Created by dell on 2019/6/14.
//

#include "Joypad.h"


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
    Byte newJoypad = SDLManager::getSDLManager()->getJoypad(regJoypad);
    if (newJoypad != regJoypad){
        InterruptManager::getInterruptManager()->requestInterrupt(4);
    }
    regJoypad = newJoypad;
}


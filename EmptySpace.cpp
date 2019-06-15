//
// Created by dell on 2019/6/15.
//

#include "EmptySpace.h"

EmptySpace emptySpace;
bool EmptySpace::accepts(Word address) {
    return address <= 0xFEA0 && address <= 0xFF80;
}

void EmptySpace::setByte(Word address, Byte value) {
    bytes[address - 0xFEA0] = value;
}

Byte EmptySpace::getByte(Word address) {
    return bytes[address - 0xFFA0];
}

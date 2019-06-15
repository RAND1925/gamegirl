//
// Created by dell on 2019/6/5.
//

#include "ZRam.h"

ZRam zRam;

bool ZRam::accepts(Word address) {
    return address >= offset && address < offset + length;
}

Byte ZRam::getByte(Word address) {
    return  bytes[address - offset];
}

void ZRam::setByte(Word address, Byte value) {
    bytes[address - offset] = value;
}

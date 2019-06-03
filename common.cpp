//
// Created by dell on 2019/5/29.
//
#include "common.h"
Byte getBit(const Byte b, const Byte p){
    return (b >> p) & 1;
}
void setBit(Byte & b, const Byte p){
    b |= (1 << p);
}
void resetBit(Byte & b, const Byte p){
    b &= ~(1 << p);
}
//
// Created by dell on 2019/6/15.
//

#include "PlainMemory.h"
template <Word len>
bool PlainMemory<len>::accepts(Word address) {
    return address >= offset && address < offset + length;
}
template <Word len>
Byte PlainMemory<len>::getByte(Word address) {
    return bytes[address - offset];
}

template <Word len>
void PlainMemory<len>::setByte(Word address, Byte value) {
    bytes[address - offset] = value;
}

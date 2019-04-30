//
// Created by dell on 2019/4/17.
//

#ifndef CPPGB_MMU_H
#define CPPGB_MMU_H

#include "common.h"
#include "Memory.h"
class MMU {
public:
    Memory rom;
    MMU(char * s): rom(0x00, 0x3FFF, s){};
    Byte readByte(Word address);
    Word readWord(Word address);
	SByte readSByte(Word address);
    void writeByte(Word address, Byte value);
    void writeWord(Word address, Word value);
};


#endif //CPPGB_MMU_H

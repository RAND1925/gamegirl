// memory management unit
// Created by dell on 2019/4/17.
//

#ifndef GAMEGIRL_MMU_H
#define GAMEGIRL_MMU_H

#include "common.h"
#include "AddressSpace.h"

#include <vector>
#include <array>

class MMU {
private:
    std::vector<AddressSpace *> spaces;
    std::array<Byte, 32_kb> unusedSpaces;
public:
	void addAddressSpace(AddressSpace * s){
		spaces.push_back(s);
	};

    Byte readByte(Word address);
    Word readWord(Word address);
	SByte readSByte(Word address);
    void writeByte(Word address, Byte value);
    void writeWord(Word address, Word value);
};


#endif //GAMEGIRL_MMU_H

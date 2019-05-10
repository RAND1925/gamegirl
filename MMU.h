// memory management unit
// Created by dell on 2019/4/17.
//

#ifndef CPPGB_MMU_H
#define CPPGB_MMU_H

#include "common.h"
#include "AddressSpace.h"

#include <vector>
#include <memory>

using std::vector;
class MMU {
private:
    vector<AddressSpace *> spaces;

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


#endif //CPPGB_MMU_H

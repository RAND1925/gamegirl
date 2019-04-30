//
// Created by dell on 2019/4/17.
//

#ifndef CPPGB_MEMORY_H
#define CPPGB_MEMORY_H

#include "common.h"
#include <vector>
class Memory {
public:
    std::vector<Byte> memory;
    Word begin;
    Word len;
    Memory(Word b, Word l, char* s):begin(b), len(l){
        memory.reserve(len);
        for (int i = 0; i < l; ++i){
            memory.push_back(s[i]);
        }
    }
};


#endif //CPPGB_MEMORY_H

// an abstract class, each part which require to access memory should derive it
// Created by dell on 2019/5/7.
//

#ifndef CPPGB_ADDRESSSPACE_H
#define CPPGB_ADDRESSSPACE_H

#include"common.h"
class AddressSpace {
public:
    virtual bool accepts(Word address) = 0;
    virtual Byte getByte(Word address) = 0;
    virtual void setByte(Word address, Byte value) = 0;
};


#endif //CPPGB_ADDRESSSPACE_H

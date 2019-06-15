//
// Created by dell on 2019/6/15.
//

#ifndef GAMEGIRL_PLAINMEMORY_H
#define GAMEGIRL_PLAINMEMORY_H

#include <array>
#include "AddressSpace.h"
template <Word len>
class PlainMemory: public AddressSpace {
public:
    virtual bool accepts(Word address) override;
    virtual Byte getByte(Word address) override;
    virtual void setByte(Word address, Byte value) override;

private:
    std::array<Byte, len> bytes{};
    const Word offset;
    const Word length = len;
};



#endif //GAMEGIRL_PLAINMEMORY_H

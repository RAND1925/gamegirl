// FE00 - FFFF will be advanced after other parts completed
// Created by dell on 2019/5/8.
//

#ifndef GAMEGIRL_ZRAM_H
#define GAMEGIRL_ZRAM_H

#include <array>
#include "common.h"
#include "AddressSpace.h"

class ZRam: public AddressSpace {
public:
    bool accepts(Word address) override;
    Byte getByte(Word address) override;
    void setByte(Word address, Byte value) override;
    static ZRam* getZRam();
protected:
    ZRam()= default;
private:
    const static Word offset = 0xFF80;
    const static Word length = 0x007F;
    std::array<Byte, length> bytes{};

};
#endif //GAMEGIRL_ZRAM_H

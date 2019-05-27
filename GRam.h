#ifndef CPPGB_GRAM_H
#define CPPGB_GRAM_H

#include "common.h"
#include "AddressSpace.h"
#include <array>

template <Word offset, Word length>
class GRam : public AddressSpace
{
    std::array<Byte, length> bytes;

    bool accepts(Word address) override
    {
        return address >= offset && address < offset + length;
    }
    Byte getByte(Word address) override
    {
        if (address == 0xFF00)
        {
            if (KeyColumn == 0x10)
                return joypad_C0;
            else if (KeyColumn == 0x20)
                return joypad_C1;
        }
        else
            return bytes[address - offset];
    }
    void setByte(Word address, Byte value) override
    {
        if (address == 0xFF00)
            KeyColumn = value &= 0x30; //get the high 4 bit of the value
        else
            bytes[address - offset] = value;
    }
    //two byte to store joypad information
    Byte joypad_C0 = 0x0F;
    Byte joypad_C1 = 0x0F;
    
    //judge if it's direction or select
    Byte KeyColumn = 0x00;
};
#endif //CPPGB_GRAM_H

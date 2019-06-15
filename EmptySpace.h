
/*
 * unused
 * FEA0-FF unused
 * FF01-03 serial
 * FF10-3F sound
 * FF4C-79 unused
 *
 */
#ifndef GAMEGIRL_EMPTYSPACE_H
#define GAMEGIRL_EMPTYSPACE_H

#include "AddressSpace.h"
class EmptySpace: public AddressSpace{
private:
    Byte bytes[0xFF80 - 0xFEA0]{};
public:
    bool accepts(Word address) override;
    void setByte(Word address, Byte value) override;
    Byte getByte(Word address) override;
};

extern EmptySpace emptySpace;
#endif // GAMEGIRL_EMPTYSPACE_H

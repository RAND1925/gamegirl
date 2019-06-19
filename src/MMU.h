// memory management unit, every should use it to read and write memory
// Created by dell on 2019/4/17.
//

/*
read and write form ROM
0000-3FFF 16KB ROM Bank 00 (CartridgeDriver, fixed at bank 00)
4000-7FFF 16KB ROM Bank 01..NN (CartridgeDriver, switchable bank number)
8000-9FFF 8KB Video RAM (GPU) (switchable bank 0-1 in CGB Mode)
A000-BFFF 8KB External RAM (CartridgeDriver, switchable bank, if any)
C000-CFFF 4KB Work RAM Bank 0 (WRam)
D000-DFFF 4KB Work RAM Bank 1 (WRam) (switchable bank 1-7 in CGB Mode)
E000-FDFF Same as C000-DDFF ECHO (WRam) (typically not used)
FE00-FE9F Sprite Attribute Table OAM (GPU)
FEA0-FEFF Not Usable (EmptySpace)
FF00-FF7F I/O Ports (EmptySpace & different parts)
FF80-FFFE High RAM (WRam)
FFFF Interrupt Enable Register
*/

#ifndef GAMEGIRL_MMU_H
#define GAMEGIRL_MMU_H



#include <vector>
#include "common.h"
#include "AddressSpace.h"
#include "Exceptions.h"

class MMU {
private:
    std::vector<AddressSpace *> spaces{};

public:
    Byte readByte(Word address);
    Word readWord(Word address);
    void writeByte(Word address, Byte value);
    void writeWord(Word address, Word value);
    AddressSpace* findAddressSpace(Word address);
    void addAddressSpace(AddressSpace * s);
    void removeAddressSpace(AddressSpace * s);
    void init();
    static MMU* getMMU();

protected:
    MMU()= default;

#ifndef NO_ADDRESS_ERROR
    Byte* unusedSpaces{nullptr};
#endif
};

#endif //GAMEGIRL_MMU_H

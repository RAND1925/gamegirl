//
// Created by dell on 2019/4/16.
//

#ifndef GAMEGIRL_CPU_H
#define GAMEGIRL_CPU_H

#include <functional>
#include <iostream>
#include <iomanip>
#include "common.h"
#include "Exceptions.h"
#include "MMU.h"
#include "InterruptManager.h"
#include <fstream>

class CPU {
private:

    struct Registers {
        Byte a, f, b, c, d, e, h, l;
        Word sp, pc;
    } registers, backup;

    using FlagSetter = std::function<void(void)>;
    using FlagGetter = std::function<Byte(void)>;
    FlagSetter setZ = [this]() { setBit(registers.f, 7); };
    FlagSetter setN = [this]() { setBit(registers.f, 6); };
    FlagSetter setH = [this]() { setBit(registers.f, 5); };
    FlagSetter setC = [this]() { setBit(registers.f, 4); };
    FlagSetter resetZ = [this]() { resetBit(registers.f, 7); };
    FlagSetter resetN = [this]() { resetBit(registers.f, 6); };
    FlagSetter resetH = [this]() { resetBit(registers.f, 5); };
    FlagSetter resetC = [this]() { resetBit(registers.f, 4); };
    FlagGetter getZ = [this]() -> Byte { return getBit(registers.f, 7); };
    FlagGetter getN = [this]() -> Byte { return getBit(registers.f, 6); };
    FlagGetter getH = [this]() -> Byte { return getBit(registers.f, 5); };
    FlagGetter getC = [this]() -> Byte { return getBit(registers.f, 4); };
    //tool functions to run opcodes
    Byte getImmediateValue8();
    Word getImmediateValue16();;
    SByte getSignedImmediateValue8();;
    void ld8(Byte &lhs, Byte rhs);
    void ld16(Word &lhs, Word rhs);
    Word getR16(Byte high, Byte low);
    void setR16(Byte &high, Byte &low, Word t);
    Word getAF();
    Word getHL();;
    Word getBC();;
    Word getDE();;
    void setAF(Word t);;
    void setHL(Word t);;
    void setBC(Word t);;
    void setDE(Word t);;
    void push16(Word val);
    Word pop16();
    Byte add(Byte a, Byte b);
    Byte adc(Byte a, Byte b);
    Word add(Word a, Word b);
    Word addSp(Word a, SByte b);
    Byte sub(Byte a, Byte b);
    Byte sbc(Byte a, Byte b);
    Byte andAL(Byte a, Byte b);
    Byte orAL(Byte a, Byte b);
    Byte xorAL(Byte a, Byte b);
    void cp(Byte a, Byte b);
    Byte inc(Byte a);
    Word inc(Word a);
    Byte dec(Byte a);
    Word dec(Word a);
    Byte swap(Byte a);
    void daa();
    Byte cpl(Byte a);
    void ccf();
    void scf();
    Byte rlc(Byte a);
    Byte rl(Byte a);
    Byte rrc(Byte a);
    Byte rr(Byte a);
    Byte sla(Byte a);
    Byte sra(Byte a);
    Byte srl(Byte a);
    void bit(Byte b, Byte a);
    Byte set(Byte b, Byte a);
    Byte res(Byte b, Byte a);
    void jump(Word addr);
    void jr(Byte r);
    void call(Word addr);

    void ret();

    void rsv();

    void rls();

    void restart(Word addr);

    std::function<Byte(void)> opMap[0x100];
    std::function<Byte(void)> opCBMap[0x100];
    Byte normalTable[0x100]{
            1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
            0, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
            2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
            2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            2, 2, 2, 2, 2, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4,
            2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4,
            3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
            3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4
    };
    Byte CBTable[0x100]{
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2
    };
    void display();
public:

    Byte step();

    inline void setPc(Word pc);
    inline Word getPc();
    void initRegisters();
    void initRegistersAfterBoot();
    void initMap();
};

extern CPU cpu;

#endif //GAMEGIRL_CPU_H

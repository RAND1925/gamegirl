// cpu
// Created by dell on 2019/4/16.
//

#ifndef GAMEGIRL_CPU_H
#define GAMEGIRL_CPU_H

#include <functional>
#include "common.h"

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
    inline Byte getImmediateValue8();
    inline Word getImmediateValue16();;
    inline SByte getSignedImmediateValue8();;
    inline void ld8(Byte &lhs, Byte rhs);
    inline void ld16(Word &lhs, Word rhs);
    inline Word getR16(Byte high, Byte low);
    inline void setR16(Byte &high, Byte &low, Word t);
    inline Word getAF();
    inline Word getHL();;
    inline Word getBC();;
    inline Word getDE();;
    inline void setAF(Word t);;
    inline void setHL(Word t);;
    inline void setBC(Word t);;
    inline void setDE(Word t);;
    inline void push16(Word val);
    inline Word pop16();
    inline Byte add(Byte a, Byte b);
    inline Byte adc(Byte a, Byte b);
    inline Word add(Word a, Word b);
    inline Word addSp(Word a, SByte b);
    inline Byte sub(Byte a, Byte b);
    inline Byte sbc(Byte a, Byte b);
    inline Byte andAL(Byte a, Byte b);
    inline Byte orAL(Byte a, Byte b);
    inline Byte xorAL(Byte a, Byte b);
    inline void cp(Byte a, Byte b);
    inline Byte inc(Byte a);
    inline Word inc(Word a);
    inline Byte dec(Byte a);
    inline Word dec(Word a);
    inline Byte swap(Byte a);
    inline void daa();
    inline Byte cpl(Byte a);
    inline void ccf();
    inline void scf();
    inline Byte rlc(Byte a);
    inline Byte rl(Byte a);
    inline Byte rrc(Byte a);
    inline Byte rr(Byte a);
    inline Byte sla(Byte a);
    inline Byte sra(Byte a);
    inline Byte srl(Byte a);
    inline void bit(Byte b, Byte a);
    inline Byte set(Byte b, Byte a);
    inline Byte res(Byte b, Byte a);
    inline void jump(Word addr);
    inline void jr(Byte r);
    inline void call(Word addr);
    inline void ret();
    inline void rsv();
    inline void rls();
    inline void restart(Word addr);

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

    inline void setPC(Word pc){ registers.pc = pc;};
    inline Word getPC(){return registers.pc;};
    void initRegisters();
    void initRegistersAfterBoot();
    void initMap();
};

extern CPU cpu;

#endif //GAMEGIRL_CPU_H

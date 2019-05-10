//
// Created by dell on 2019/4/16.
//

#ifndef CPPGB_CPU_H
#define CPPGB_CPU_H

#include <functional>
#include <iostream>
#include <iomanip>
#include "common.h"
#include "MMU.h"

class CPU {
private:
        using FlagSetter = std::function<void(void)>;
        using FlagGetter = std::function<Byte(void)>;
        FlagSetter setZ = [this]() { registers.f |= (1 << 7); };
        FlagSetter setN = [this]() { registers.f |= (1 << 6); };
        FlagSetter setH = [this]() { registers.f |= (1 << 5); };
        FlagSetter setC = [this]() { registers.f |= (1 << 4); };
        FlagSetter resetZ = [this]() { registers.f &= ~(1 << 7); };
        FlagSetter resetN = [this]() { registers.f &= ~(1 << 6); };
        FlagSetter resetH = [this]() { registers.f &= ~(1 << 5); };
        FlagSetter resetC = [this]() { registers.f &= ~(1 << 4); };
        FlagGetter getZ = [this]() -> Byte { return (registers.f & (1 << 7) ) >> 7; };
        FlagGetter getN = [this]() -> Byte { return (registers.f & (1 << 6) ) >> 6; };
        FlagGetter getH = [this]() -> Byte { return (registers.f & (1 << 5) ) >> 5; };
        FlagGetter getC = [this]() -> Byte { return (registers.f & (1 << 4) ) >> 4; };

    struct Registers {
        Byte a, f, b, c, d, e, h, l;
        Word sp, pc;
    }registers;
    MMU & mmu;
	Byte add(Byte a, Byte b) {
		if (((a & 0xF) + (b & 0xF)) > 0xf) {
			setH();
		}
		if (UINT8_MAX - a > b) {
			setC();
		}
		if (a + b == 0) {
			setZ();
		}
		resetN();
		return a + b;
	};
    Byte adc(Byte a, Byte b){

        return add(a, b + getC());
    };

    Word add(Word a, Word b){
		if (UINT8_MAX - a > b) {
			setH();
		}
		if (UINT16_MAX - a > b) {
			setC();
		}
		resetN();
        return a + b;
    };

	Word addSp(Word a, Word b) {
		if (UINT8_MAX - a > b) {
			setH();
		}
		if (UINT16_MAX - a > b) {
			setC();
		}
		//����ԭ�鿴������GBCPUman P91����ֱ�ӳ��������Ǹ�������Ҫ��
		resetZ();
		resetN();
		return a + b;
	}
	Byte sub(Byte a, Byte b) {
		setN();
		if (a == b) {
			setZ();
		}
		if ((a & 0xF) >= (b & 0xF)) {
			setH();
		}
		if ((a >= b)) {
			setC();
		}
		return  a - b;
	}
	
    Byte sbc(Byte a, Byte b){
        return sub(a, b + getC());
    }
    Byte andAL(Byte a, Byte b){
        if ((a & b) == 0){
            setZ();
        }
        resetN();
        setH();
        resetC();
        return a & b;
    }
    Byte orAL(Byte a, Byte b){
        if ((a | b) == 0){
            setZ();
        }
        resetN();
        resetH();
        resetC();
        return a | b;
    }
    Byte xorAL(Byte a, Byte b){
        if ((a ^ b) == 0){
            setZ();
        }
        resetN();
        resetH();
        resetC();
        return a ^ b;
    }
    void cp(Byte a, Byte b){
        sub(a, b);
    }
	Byte inc(Byte a) {
		if (a == 255) {
			setZ();
		}
		if (a & 0xF == 0xF) {
			setH();
		}
		resetN();
		return a + 1;
	}

	Word inc(Word a) {
		return a + 1;
	}

	Byte dec(Byte a) {
		if (a == 1) {
			setZ();
		}
		if (a & 0xF == 0xF) {
			setH();
		}
		setN();
		return a - 1;
	}

	Word dec(Word a) {
		return a - 1;
	}

	Byte swap(Byte a) {
		if (a == 0)
		{
			setZ();
		}
		resetN();
		resetC();
		resetH();
		return Byte(a << 4) + Byte(a >> 4);
	}
	/*
	Word swap(Word a) {
		if (a == 0)
		{
			setZ();
		}
		resetN();
		resetC();
		resetH();
		return Word(a << 8) + Word(a >> 8);
	}
	*/
	Byte cpl(Byte a) {
		setN();
		setH();
		return (a ^ 0xFF);
	}

	void ccf() {
		resetH();
		resetN();
		if (getC())
			resetC();
		else
			setC();
	}

	void scf() {
		resetH();
		resetN();
		setC();
	}

	Byte rlc(Byte a) {
		Byte bit_7 = a >> 7;
		Byte res = (a << 1) + getC();
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_7)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
	}

	Byte rl(Byte a) {
		Byte bit_7 = a >> 7;
		Byte res = (a << 1) + bit_7;
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_7)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
	}

	Byte rrc(Byte a) {
		Byte bit_0 = a << 7;
		Byte res = (a >> 1) + (getC() << 7);
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_0)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
	}

	Byte rr(Byte a) {
		Byte bit_0 = a << 7;
		Byte res = (a >> 1) + bit_0;
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_0)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
	}

	Byte sla(Byte a) {
		Byte bit_7 = a >> 7;
		Byte res = a << 1;
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_7)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
		// LSB of n set to 0(LSBָ�����������Ҳ�һλ)
	}

	Byte sra(Byte a) {
		Byte bit_7 = a & (1 << 7);
		Byte bit_0 = a << 7;
		Byte res = (a >> 1) + bit_7;
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_0)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
	}

	Byte srl(Byte a) {
		Byte bit_0 = a << 7;
		Byte res = a >> 1;
		if (res == 0)
		{
			setZ();
		}
		resetN();
		resetH();
		if (bit_0)
		{
			setC();
		}
		else
		{
			resetC();
		}
		return res;
		//MSB set to 0��MSBָ�����������λ��
	}

	void bit(Byte b,Byte a) {
		resetN();
		setH();
		Byte res = a & (1 << b);
		if (res == 0)
			setZ();
	}

	Byte set(Byte b, Byte a) {
		return a | (1 << b);
	}

	Byte res(Byte b, Byte a) {
		return a & !(1 << b);
	}


    void initMap();
    std::function<Byte(void)> opMap[0x100];
	std::function<Byte(void)> opCBMap[0x100];


public:
    void step(){
        Byte opNum = mmu.readByte(registers.pc);
		std::cout << "pc: 0x" << std::hex << registers.pc << " opNum: 0x" << std::hex<< (int)opNum << std::endl;

		int p = (opMap[opNum])();
        registers.pc++;
    }

    CPU(MMU & m):mmu(m){
        registers.a = registers.f = registers.b = registers.c = registers.d = registers.e = registers.h = registers.l = 0;
        registers.sp = registers.pc = 0;
        registers.pc = 0x150;
        initMap();
    };

};


#endif //CPPGB_CPU_H

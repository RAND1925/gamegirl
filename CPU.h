//
// Created by dell on 2019/4/16.
//

#ifndef CPPGB_CPU_H
#define CPPGB_CPU_H

#include <functional>
#include <iostream>
#include "common.h"
#include "Op.h"
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
        Register8 a, f, b, c, d, e, h, l;
        Register16 sp, pc;
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
		Word sum = a + b;
		Word half = (a & 0xFFF) + (b & 0xFFF);
		half > 0xFFF ? setH() : resetH();
		sum > 0xFFFF ? setN() : resetN();
		return sum & 0xFFFF;
    };

	Word addSp(Word a, Word b) {
		Word sum = a + b;
		(sum & 0xF) < (a & 0xF) ? setH() : resetH();
		(sum & 0xFF) < (a & 0xFF) > 0xFFFF ? setN() : resetN();
		//这里原书看不懂（GBCPUman P91），直接抄的上面那个，错了要改
		resetZ();
		resetN();
		return sum;
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

	void push16(Word val) {
		mmu.writeByte(registers.sp, val);
		registers.sp -= 2;
		//according to gb instructions25, I thought it should be +=2
	}
	Word pop16() {
		registers.sp += 2;
		//same as above
		Word val = mmu.readWord(registers.sp - 2);
		val |= mmu.readWord(registers.sp - 1);//according to gb instructions25, idk why
		return val;
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
		// LSB of n set to 0(LSB指二进制数最右侧一位)
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
		//MSB set to 0（MSB指二进制数最高位）
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

	void call(Word addr) {
		push16(registers.pc);
		registers.pc = addr;
	}


    void initMap();
    std::function<Byte(void)> opMap[0x100];
	std::function<Byte(void)> opCBMap[0x100];
public:
    void step(){
        Byte opNum = mmu.readByte(registers.pc);
        int p = (opMap[opNum])();
        std::cout << "w";
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

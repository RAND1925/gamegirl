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

class CPU{
private:
        using FlagSetter = std::function<void(void)>;
        using FlagGetter = std::function<Byte(void)>;
        FlagSetter setZ = [this]() {setBit(registers.f, 7); };
        FlagSetter setN = [this]() {setBit(registers.f, 6); };
        FlagSetter setH = [this]() {setBit(registers.f, 5); };
        FlagSetter setC = [this]() {setBit(registers.f, 4); };
        FlagSetter resetZ = [this]() { resetBit(registers.f, 7); };
        FlagSetter resetN = [this]() { resetBit(registers.f, 6); };
        FlagSetter resetH = [this]() { resetBit(registers.f, 5); };
        FlagSetter resetC = [this]() { resetBit(registers.f, 4); };
        FlagGetter getZ = [this]() -> Byte { return getBit(registers.f, 7); };
        FlagGetter getN = [this]() -> Byte { return getBit(registers.f, 6); };
        FlagGetter getH = [this]() -> Byte { return getBit(registers.f, 5); };
        FlagGetter getC = [this]() -> Byte { return getBit(registers.f, 4); };

    struct Registers {
        Byte a, f, b, c, d, e, h, l;
        Word sp, pc;
    }registers, backup;

	Byte add(Byte a, Byte b) {
        (((a & 0xF) + (b & 0xF)) > 0xF) ?setH():resetH();
        (UINT8_MAX - a < b) ?setC():resetC();
        ((a + b == 0)||(a + b == 256)) ?setZ():resetZ();
        resetN();
        return a + b;
	};
    Byte adc(Byte a, Byte b){
        return add(a, b + getC());
    };

    Word add(Word a, Word b){
        unsigned int sum = a + b;
        unsigned int half = (a & 0xFFF) + (b & 0xFFF);
        half > 0xFFF ? setH() : resetH();
        sum > 0xFFFF ? setC() : resetC();
        resetN();
        return sum & 0xFFFF;
    };

	Word addSp(Word a, SByte b) {
        Word sum = a + b;
        (sum & 0xF) < (a & 0xF) ? setH() : resetH();
        (sum & 0xFF) < (a & 0xFF) ? setC() : resetC();
        resetZ();
        resetN();
        return sum;
	}
	Byte sub(Byte a, Byte b) {
        setN();
        (a==b)?setZ():resetZ();
        (((a - b) & 0xF)>(a&0xF))?setH():resetH();
        (a<b)?setC():resetC();
        return  a - b;
	}
	
    Byte sbc(Byte a, Byte b){
        return sub(a, b + getC());
    }
    Byte andAL(Byte a, Byte b){
        ((a & b) == 0)?setZ():resetZ();
        resetN();
        setH();
        resetC();
        return a & b;
    }
    Byte orAL(Byte a, Byte b){
        ((a | b) == 0)?setZ():resetZ();
        resetN();
        resetH();
        resetC();
        return a | b;
    }
    Byte xorAL(Byte a, Byte b){
        ((a ^ b) == 0)?setZ():resetZ();
        resetN();
        resetH();
        resetC();
        return a ^ b;
    }
    void cp(Byte a, Byte b){
        sub(a, b);
    }
	Byte inc(Byte a) {
		(a == 255) ?setZ():resetZ();
		((a & 0xF) == 0xF) ?setH():resetH();
		resetN();
		return a + 1;
	}

	Word inc(Word a) {
		return a + 1;
	}

	Byte dec(Byte a) {
	    (a == 1) ?setZ():resetZ();
        ((a & 0xF) == 0x0) ?setH():resetH();
        setN();
        return a - 1;
	}

	Word dec(Word a) {
		return a - 1;
	}

	void push16(Word val) {
        registers.sp -= 2;
		mmu.writeWord(registers.sp, val);

		//according to gb instructions25, I thought it should be +=2
	}
	Word pop16() {
		Word val = mmu.readWord(registers.sp);
        registers.sp += 2;
		return val;
	}
	Byte swap(Byte a) {
        (a == 0) ?setZ():resetZ();
		resetN();
		resetC();
		resetH();
		return Byte(a << 4) + Byte(a >> 4);
	}

	void daa(){
	    Word ra=registers.a;
	    if(getN()) {
            if (getH()) {
                ra -= 0x06;
                ra &= 0xFF;
            }
            if(getC()){
                ra -= 0x06;
            }
        }
	    else{
	        if((ra & 0x0F)>0x09||getH()){
	            ra+=0x06;
	        }
	        if(ra > 0x9F||getC()){
	            ra+=0x60;
	        }
	    }
        (ra&0xFF)==0?setZ():resetZ();
	    resetH();
	    if(ra&0x100) setC();
	    registers.a=(Byte)(ra & 0xFF);
	}

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
        Byte res = (a << 1) | bit_7;
        (res == 0) ?setZ():resetZ();
        resetN();
        resetH();
        (bit_7)?setC():resetC();
        return res;
	}

	Byte rl(Byte a) {
        Byte bit_7 = a >> 7;
        Byte res = (a << 1) | getC();
        (res == 0) ?setZ():resetZ();
        resetN();
        resetH();
        (bit_7)?setC():resetC();
        return res;
	}

	Byte rrc(Byte a) {
        Byte bit_0 = a << 7;
        Byte res = (a >> 1) + bit_0;
        (res == 0) ?setZ():resetZ();
        resetN();
        resetH();
        (bit_0)?setC():resetC();
        return res;
	}

	Byte rr(Byte a) {
        Byte bit_0 = a << 7;
        Byte res = (a >> 1) + (getC() << 7);
        (res == 0) ?setZ():resetZ();
        resetN();
        resetH();
        (bit_0)?setC():resetC();
        return res;
	}

	Byte sla(Byte a) {
		Byte bit_7 = a >> 7;
		Byte res = a << 1;
        (res == 0) ?setZ():resetZ();
		resetN();
		resetH();
        (bit_7)?setC():resetC();
		return res;
		// LSB of n set to 0(LSB is the last bit)
	}
    //maybe some promblems
	Byte sra(Byte a) {
		Byte bit_7 = a & (Byte)(1 << 7);
		Byte bit_0 = a << 7;
		Byte res = (a >> 1) + bit_7;
        (res == 0) ?setZ():resetZ();
		resetN();
		resetH();
        (bit_0)?setC():resetC();
		return res;
		//MSB dosen't change
	}

	Byte srl(Byte a) {
		Byte bit_0 = a << 7;
		Byte res = a >> 1;
        (res == 0) ?setZ():resetZ();
		resetN();
		resetH();
        (bit_0)?setC():resetC();
		return res;
		//MSB set to 0   MSB is the first bit
	}

	void bit(Byte b,Byte a) {
		resetN();
		setH();
		Byte res = a & (Byte)(1 << b);
        (res == 0) ?setZ():resetZ();
	}

	Byte set(Byte b, Byte a) {
	    setBit(a, b);
        return a;
	}

	Byte res(Byte b, Byte a) {
	    resetBit(a, b);
        return a;
	}

	void jump(Word addr){
		registers.pc = addr;
	}
	void jr(Byte r){
		registers.pc += (SByte)r;
	}

	void call(Word addr) {
		push16(registers.pc);
		jump(addr);
	}

	void ret(Word addr){
	    jump(pop16());
	}
	void rsv(){
	    backup = registers;
	}
	void rls(){
	    registers = backup;
	}

	void restart(Word addr){
        rsv();
        call(addr);
	}

    std::function<Byte(void)> opMap[0x100];
	std::function<Byte(void)> opCBMap[0x100];
  public:
    void display();

	void initMap();
    Byte step();

    void initRegisters();;
    CPU(){
        initRegisters();
        initMap();
    }
};

extern CPU cpu;

#endif //GAMEGIRL_CPU_H

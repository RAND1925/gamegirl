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
    }registers, backup;
    struct States{
        bool interruptMasterEnabled;
        bool interruptEnabled;
        Byte interruptFlag;
        bool m;
        bool halt = false;
        bool stop = false;

    }states;


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

	void rsv(){
	    backup = registers;
	}
	void rls(){
	    registers = backup;
	}

	template<Byte address>
	Byte restart(){
        rsv();
        registers.sp -=2;
        mmu.writeWord(registers.sp, registers.pc);
        registers.pc = address;
        return 32;
	}
    void initMap();

	std::function<Byte(void)> opMap[0x100];
	std::function<Byte(void)> opCBMap[0x100];
    std::function<Byte(void)> rst40 = [this](){ return restart<0x40>(); };
    std::function<Byte(void)> rst48 = [this](){ return restart<0x48>(); };
    std::function<Byte(void)> rst50 = [this](){ return restart<0x50>(); };
    std::function<Byte(void)> rst58 = [this](){ return restart<0x58>(); };
    std::function<Byte(void)> rst60 = [this](){ return restart<0x60>(); };
  public:
    Byte step(){
    	Byte timing = 4;
        bool hasInterrupt = states.interruptEnabled & states.interruptFlag;
        if (hasInterrupt && states.interruptMasterEnabled){
            states.halt = false;
            states.interruptMasterEnabled = false;
			std::cout << "interrupt: " << states.interruptFlag << std::endl;
			if (getBit<0>(states.interruptFlag)){
				return rst40();
			}
			if (getBit<1>(states.interruptFlag)){
				return rst48();
			}
			if (getBit<2>(states.interruptFlag)){
				return rst50();
			}
			if (getBit<3>(states.interruptFlag)){
				return rst58();
			}
			if (getBit<4>(states.interruptFlag)){
				return rst60();
			}
        }
        else {
            if (!states.interruptMasterEnabled && states.interruptFlag && states.halt){
				std::cout << "halt" << std::endl;
				states.halt = false;
            }
            else {
				Byte opNum = mmu.readByte(registers.pc);
				registers.pc++;
				timing = opMap[opNum]();
				std::cout << "pc:" << std::hex << registers.pc << " opNum: " << std::hex << (int)opNum << std::endl;


            }
        }
        return timing;
    }

    CPU(MMU & m):mmu(m){
        initRegisters();
        initMap();
    };
    void cycle(){
    	while(1){
    		readStates();
			int timing = step();
			writeStates();//display

		}
      }

    void initRegisters(){
        registers.a = 0x01;
        registers.f = 0x80;
        registers.b = 0x00;
        registers.c = 0x13;
        registers.d = 0x00;
        registers.e = 0xD8;
        registers.h = 0x01;
        registers.l = 0x4D;
        registers.sp = 0xFFFE;
        registers.pc = 0x0100;

        //todo: init registers in zram;
    };
    void readStates(){
    	states.interruptFlag = mmu.readByte(0xFF0F);
    	states.interruptEnabled = mmu.readByte(0xFFFF);
    }
    void writeStates(){
    	mmu.writeByte(0xFF0F, states.interruptFlag);
 		mmu.writeByte(0xFFFF, states.interruptEnabled);
    }
};


#endif //CPPGB_CPU_H

//
// Created by dell on 2019/4/16.
//

#ifndef GAMEGIRL_CPU_H
#define GAMEGIRL_CPU_H

#include <functional>
#include <iostream>
#include <iomanip>
#include "common.h"
#include "MMU.h"

class CPU: public AddressSpace {
private:
        using FlagSetter = std::function<void(void)>;
        using FlagGetter = std::function<Byte(void)>;
        FlagSetter setZ = [this]() {setBit(registers.f, 7); };
        FlagSetter setN = [this]() {setBit(registers.f, 6); };
        FlagSetter setH = [this]() {setBit(registers.f, 5); };
        FlagSetter setC = [this]() {setBit(registers.f, 5); };
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
    struct States{
        bool interruptMasterEnabled = true;
        Byte interruptEnabled = 0;
        Byte interruptFlag = 0;
        bool halt = false;
        bool stop = false;

    }states;


    MMU & mmu;
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

	Word addSp(Word a, Word b) {
        Word sum = a + (int_fast8_t)b;
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
		mmu.writeByte(registers.sp-2, (Byte)val);
		mmu.writeByte(registers.sp-1, (Byte)(val>>8));
		registers.sp -= 2;
		//according to gb instructions25, I thought it should be +=2
	}
	Word pop16() {
		registers.sp += 2;
		Word val = mmu.readByte(registers.sp - 2);
		val |= (mmu.readByte(registers.sp - 1)<<8);//according to gb instructions25, idk why
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
	    registers.a=ra;
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
		Byte bit_7 = a & (1 << 7);
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
		Byte res = a & (1 << b);
        (res == 0) ?setZ():resetZ();
	}

	Byte set(Byte b, Byte a) {
		return a | (1 << b);
	}

	Byte res(Byte b, Byte a) {
        return a & ~(1 << b);
	}

	void call(Word addr) {
		push16(registers.pc);
		registers.pc = addr;
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

    void initMap();
    std::function<Byte(void)> opMap[0x100];
	std::function<Byte(void)> opCBMap[0x100];
  public:
    Byte step(){
    	Byte timing = 4;
        bool hasInterrupt = states.interruptEnabled & states.interruptFlag;
        if (hasInterrupt && states.interruptMasterEnabled){
            states.halt = false;
            states.interruptMasterEnabled = false;
			std::cout << "interrupt: " << states.interruptFlag << std::endl;
			for(Byte i = 0; i < 8; ++i){
				if (getBit(states.interruptFlag, i)){
					restart(0x40 + (i<<3));
					return 32;
				}
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
    Byte cycle(){
    		Byte timing = step();
			return timing;
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
    bool accepts(Word address) override{
		return address == 0xFF0F || address == 0xFFFF;
    }
    Byte getByte(Word address) override{
		if (address == 0xFF0F){
			return states.interruptEnabled;
		}
    	else{
			return states.interruptFlag;
    	}
    }

	void setByte(Word address, Byte value) override{
		if (address == 0xFF0F){
			states.interruptEnabled  = value;
		}
		else{
			states.interruptFlag = value;
		}
	}
};


#endif //GAMEGIRL_CPU_H

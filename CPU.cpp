// cpu...
// Created by dell on 2019/4/16.
//

#include "CPU.h"
#include <iostream>
void CPU::initMap() {

	auto ld8 = [](Byte &lhs, Byte rhs) { lhs = rhs; };
	auto ld16 = [](Word &lhs, Word rhs) { lhs = rhs; };

	auto getR16 = [](Byte high, Byte low)->Word { return high << 8 | low; };
	auto setR16 = [](Byte & high, Byte & low, Word t) { high = t >> 8; low = t & 0xFF; };
	auto getHL = [this, getR16]()->Word { return getR16(registers.h, registers.l); };
	auto getBC = [this, getR16]()->Word { return getR16(registers.b, registers.c); };
	auto getDE = [this, getR16]()->Word { return getR16(registers.d, registers.e); };
	auto setHL = [this, setR16](Word t) { setR16(registers.h, registers.l, t); };
	auto setBC = [this, setR16](Word t) { setR16(registers.b, registers.c, t); };
	auto setDE = [this, setR16](Word t) { setR16(registers.d, registers.e, t); };
	auto getImmediateValue8 = [this]()->Byte { return mmu.readByte(registers.pc++); };
	auto getImmediateValue16 = [this]()->Word {
		Word v = mmu.readWord(registers.pc);
		registers.pc += 2;
		return v;
	};
	auto getSignedImmediateValue8 = [this]()->SByte { return mmu.readSByte(registers.pc++); };

	auto nothing = [this]() { std::cout << "nothing" << std::endl; return 0; };
	for (int i = 0; i < 0x100; ++i) {
		opMap[i] = nothing;
	}
	// ld A into others;
	opMap[0x02] = [this, &getBC]() {mmu.writeByte(mmu.readByte(getBC()), registers.a); return 8; };
	opMap[0x12] = [this, &getDE]() {mmu.writeByte(mmu.readByte(getDE()), registers.a); return 8; };
	opMap[0xEA] = [this, getImmediateValue16]() {mmu.writeByte(getImmediateValue16(), registers.a); return 16; };

	opMap[0x0A] = [this, &ld8, &getBC]() {ld8(registers.a, mmu.readByte(getBC())); return 8; };
	opMap[0x1A] = [this, &ld8, &getDE]() {ld8(registers.a, mmu.readByte(getDE())); return 8; };
	opMap[0xFA] = [this, &ld8, &getImmediateValue16]() {
		ld8(registers.a, mmu.readWord(getImmediateValue16()));
		return 16;
	};

	// ld by immediateValue
	opMap[0x06] = [this, &ld8, &getImmediateValue8]() {ld8(registers.b, getImmediateValue8()); return 8; };
	opMap[0x0E] = [this, &ld8, &getImmediateValue8]() {ld8(registers.c, getImmediateValue8()); return 8; };
	opMap[0x16] = [this, &ld8, &getImmediateValue8]() {ld8(registers.d, getImmediateValue8()); return 8; };
	opMap[0x1E] = [this, &ld8, &getImmediateValue8]() {ld8(registers.e, getImmediateValue8()); return 8; };
	opMap[0x26] = [this, &ld8, &getImmediateValue8]() {ld8(registers.h, getImmediateValue8()); return 8; };
	opMap[0x2E] = [this, &ld8, &getImmediateValue8]() {ld8(registers.l, getImmediateValue8()); return 8; };
	opMap[0x3E] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, getImmediateValue8()); return 8; };
	opMap[0x36] = [this, &getHL, &getImmediateValue8]() {mmu.writeByte(getHL(), getImmediateValue8()); return 12; };


	//ld to b
	opMap[0x40] = [this, &ld8]() {ld8(registers.b, registers.b); return 4; };
	opMap[0x41] = [this, &ld8]() {ld8(registers.b, registers.c); return 4; };
	opMap[0x42] = [this, &ld8]() {ld8(registers.b, registers.d); return 4; };
	opMap[0x43] = [this, &ld8]() {ld8(registers.b, registers.e); return 4; };
	opMap[0x44] = [this, &ld8]() {ld8(registers.b, registers.h); return 4; };
	opMap[0x45] = [this, &ld8]() {ld8(registers.b, registers.l); return 4; };
	opMap[0x46] = [this, &ld8, getHL]() {ld8(registers.b, mmu.readByte(getHL())); return 8; };
	opMap[0x47] = [this, &ld8]() {ld8(registers.b, registers.a); return 4; };

	//ld to c
	opMap[0x48] = [this, &ld8]() {ld8(registers.c, registers.b); return 4; };
	opMap[0x49] = [this, &ld8]() {ld8(registers.c, registers.c); return 4; };
	opMap[0x4A] = [this, &ld8]() {ld8(registers.c, registers.d); return 4; };
	opMap[0x4B] = [this, &ld8]() {ld8(registers.c, registers.e); return 4; };
	opMap[0x4C] = [this, &ld8]() {ld8(registers.c, registers.h); return 4; };
	opMap[0x4D] = [this, &ld8]() {ld8(registers.c, registers.l); return 4; };
	opMap[0x4E] = [this, &ld8, getHL]() {ld8(registers.c, mmu.readByte(getHL())); return 8; };
	opMap[0x4F] = [this, &ld8]() {ld8(registers.c, registers.a); return 4; };

	//ld to d
	opMap[0x50] = [this, &ld8]() {ld8(registers.d, registers.b); return 4; };
	opMap[0x51] = [this, &ld8]() {ld8(registers.d, registers.c); return 4; };
	opMap[0x52] = [this, &ld8]() {ld8(registers.d, registers.d); return 4; };
	opMap[0x53] = [this, &ld8]() {ld8(registers.d, registers.e); return 4; };
	opMap[0x54] = [this, &ld8]() {ld8(registers.d, registers.h); return 4; };
	opMap[0x55] = [this, &ld8]() {ld8(registers.d, registers.l); return 4; };
	opMap[0x56] = [this, &ld8, getHL]() {ld8(registers.d, mmu.readByte(getHL())); return 8; };
	opMap[0x57] = [this, &ld8]() {ld8(registers.d, registers.a); return 4; };

	//ld to e
	opMap[0x58] = [this, &ld8]() {ld8(registers.e, registers.b); return 4; };
	opMap[0x59] = [this, &ld8]() {ld8(registers.e, registers.c); return 4; };
	opMap[0x5A] = [this, &ld8]() {ld8(registers.e, registers.d); return 4; };
	opMap[0x5B] = [this, &ld8]() {ld8(registers.e, registers.e); return 4; };
	opMap[0x5C] = [this, &ld8]() {ld8(registers.e, registers.h); return 4; };
	opMap[0x5D] = [this, &ld8]() {ld8(registers.e, registers.l); return 4; };
	opMap[0x5E] = [this, &ld8, getHL]() {ld8(registers.e, mmu.readByte(getHL())); return 8; };
	opMap[0x5F] = [this, &ld8]() {ld8(registers.e, registers.a); return 4; };

	//ld to h
	opMap[0x60] = [this, &ld8]() {ld8(registers.h, registers.b); return 4; };
	opMap[0x61] = [this, &ld8]() {ld8(registers.h, registers.c); return 4; };
	opMap[0x62] = [this, &ld8]() {ld8(registers.h, registers.d); return 4; };
	opMap[0x63] = [this, &ld8]() {ld8(registers.h, registers.e); return 4; };
	opMap[0x64] = [this, &ld8]() {ld8(registers.h, registers.h); return 4; };
	opMap[0x65] = [this, &ld8]() {ld8(registers.h, registers.l); return 4; };
	opMap[0x66] = [this, &ld8, getHL]() {ld8(registers.h, mmu.readByte(getHL())); return 8; };
	opMap[0x67] = [this, &ld8]() {ld8(registers.h, registers.a); return 4; };

	//ld to l
	opMap[0x68] = [this, &ld8]() {ld8(registers.l, registers.b); return 4; };
	opMap[0x69] = [this, &ld8]() {ld8(registers.l, registers.c); return 4; };
	opMap[0x6A] = [this, &ld8]() {ld8(registers.l, registers.d); return 4; };
	opMap[0x6B] = [this, &ld8]() {ld8(registers.l, registers.e); return 4; };
	opMap[0x6C] = [this, &ld8]() {ld8(registers.l, registers.h); return 4; };
	opMap[0x6D] = [this, &ld8]() {ld8(registers.l, registers.l); return 4; };
	opMap[0x6E] = [this, &ld8, getHL]() {ld8(registers.l, mmu.readByte(getHL())); return 8; };
	opMap[0x6F] = [this, &ld8]() {ld8(registers.l, registers.a); return 4; };

	//ld to (hl)
	opMap[0x70] = [this, &getHL]() {mmu.writeByte(getHL(), registers.b); return 4; };
	opMap[0x71] = [this, &getHL]() {mmu.writeByte(getHL(), registers.c); return 4; };
	opMap[0x72] = [this, &getHL]() {mmu.writeByte(getHL(), registers.d); return 4; };
	opMap[0x73] = [this, &getHL]() {mmu.writeByte(getHL(), registers.e); return 4; };
	opMap[0x74] = [this, &getHL]() {mmu.writeByte(getHL(), registers.h); return 4; };
	opMap[0x75] = [this, &getHL]() {mmu.writeByte(getHL(), registers.l); return 4; };


	opMap[0x77] = [this, &getHL]() {mmu.writeByte(mmu.readByte(getHL()), registers.a); return 8; };

	//ld to a
	opMap[0x78] = [this, &ld8]() {ld8(registers.a, registers.b); return 4; };
	opMap[0x79] = [this, &ld8]() {ld8(registers.a, registers.c); return 4; };
	opMap[0x7A] = [this, &ld8]() {ld8(registers.a, registers.d); return 4; };
	opMap[0x7B] = [this, &ld8]() {ld8(registers.a, registers.e); return 4; };
	opMap[0x7C] = [this, &ld8]() {ld8(registers.a, registers.h); return 4; };
	opMap[0x7D] = [this, &ld8]() {ld8(registers.a, registers.h); return 4; };
	opMap[0x7E] = [this, &ld8, &getHL]() {ld8(registers.a, mmu.readByte(getHL())); return 8; };
	opMap[0x7F] = [this, &ld8]() {ld8(registers.a, registers.a); return 4; };


	//FF00

	opMap[0xF2] = [this, &ld8]() {ld8(registers.a, mmu.readByte(registers.c + (Word)0xFF00)); return 8; };
	opMap[0xE2] = [this, &ld8]() {mmu.writeByte(registers.c + (Word)0xFF00, registers.a); return 8; };
	opMap[0xF0] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, mmu.readByte(getImmediateValue8() + (Word)0xFF00)); return 12; };
	opMap[0xE0] = [this, &ld8, &getImmediateValue8]() {mmu.writeByte(getImmediateValue8() + (Word)0xFF00, registers.a); return 12; };

	//increase & decement
	opMap[0x3A] = [this, &ld8, &getHL, &setHL]() {ld8(registers.a, mmu.readByte(getHL())); setHL((getHL() - 1)); return 8; };
	opMap[0x32] = [this, &ld8, &getHL, &setHL]() {mmu.writeByte(getHL(), registers.a); setHL((getHL() - 1)); return 8; };
	opMap[0x2A] = [this, &ld8, &getHL, &setHL]() {ld8(registers.a, mmu.readByte(getHL())); setHL((getHL() + 1)); return 8; };
	opMap[0x22] = [this, &ld8, &getHL, &setHL]() {mmu.writeByte(getHL(), registers.a); setHL((getHL() + 1)); return 8; };


	//16bits
	opMap[0x01] = [this, &setBC, &getImmediateValue16]() {setBC(getImmediateValue16()); return 12; };
	opMap[0x11] = [this, &setDE, &getImmediateValue16]() {setDE(getImmediateValue16()); return 12; };
	opMap[0x21] = [this, &setHL, &getImmediateValue16]() {setHL(getImmediateValue16()); return 12; };
	opMap[0x31] = [this, &ld16, &getImmediateValue16]() {ld16(registers.sp, getImmediateValue16()); return 12; };

	//sp
	opMap[0xF9] = [this, &ld16, &getHL]() {ld16(registers.sp, getHL()); return 8; };

	//ldhl sp
	opMap[0xF8] = [this, &setHL, &getImmediateValue16]() {setHL(add(registers.sp, getImmediateValue16())); return 12; };

	//ld(nn) sp
	opMap[0x08] = [this, &getImmediateValue16]() {mmu.writeWord(getImmediateValue16(), registers.sp); return 20; };

	//push
	opMap[0xC5] = [this]() {registers.sp--; mmu.writeByte(registers.sp, registers.b); registers.sp--; mmu.writeByte(registers.sp, registers.c); return 16; };
	opMap[0xD5] = [this]() {registers.sp--; mmu.writeByte(registers.sp, registers.d); registers.sp--; mmu.writeByte(registers.sp, registers.e); return 16; };
	opMap[0xE5] = [this]() {registers.sp--; mmu.writeByte(registers.sp, registers.h); registers.sp--; mmu.writeByte(registers.sp, registers.l); return 16; };
	opMap[0xF5] = [this]() {registers.sp--; mmu.writeByte(registers.sp, registers.a); registers.sp--; mmu.writeByte(registers.sp, registers.f); return 16; };

	//pop

	opMap[0xF1] = [this, &ld8]() {ld8(registers.f, mmu.readByte(registers.sp)); registers.sp++; ld8(registers.a, mmu.readByte(registers.sp)); registers.sp++; return 12; };
	opMap[0xC1] = [this, &ld8]() {ld8(registers.c, mmu.readByte(registers.sp)); registers.sp++; ld8(registers.b, mmu.readByte(registers.sp)); registers.sp++; return 12; };
	opMap[0xD1] = [this, &ld8]() {ld8(registers.e, mmu.readByte(registers.sp)); registers.sp++; ld8(registers.d, mmu.readByte(registers.sp)); registers.sp++; return 12; };
	opMap[0xE1] = [this, &ld8]() {ld8(registers.l, mmu.readByte(registers.sp)); registers.sp++; ld8(registers.h, mmu.readByte(registers.sp)); registers.sp++; return 12; };

	//add
	opMap[0x87] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.a)); return 4; };
	opMap[0x80] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.b)); return 4; };
	opMap[0x81] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.c)); return 4; };
	opMap[0x82] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.d)); return 4; };
	opMap[0x83] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.e)); return 4; };
	opMap[0x84] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.h)); return 4; };
	opMap[0x85] = [this, &ld8]() {ld8(registers.a, add(registers.a, registers.l)); return 4; };
	opMap[0x86] = [this, &ld8, &getHL]() {ld8(registers.a, add(registers.a, mmu.readByte(getHL()))); return 8; };
	opMap[0xC6] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, add(registers.a, getImmediateValue8())); return 8; };

	//adc
	opMap[0x8F] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.a)); return 4; };
	opMap[0x88] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.b)); return 4; };
	opMap[0x89] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.c)); return 4; };
	opMap[0x8A] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.d)); return 4; };
	opMap[0x8B] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.e)); return 4; };
	opMap[0x8C] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.h)); return 4; };
	opMap[0x8D] = [this, &ld8]() {ld8(registers.a, adc(registers.a, registers.l)); return 4; };
	opMap[0x8E] = [this, &ld8, &getHL]() {ld8(registers.a, adc(registers.a, mmu.readByte(getHL()))); return 8; };
	opMap[0xCE] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, adc(registers.a, getImmediateValue8())); return 8; };

	//sub
	opMap[0x97] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.a)); return 4; };
	opMap[0x90] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.b)); return 4; };
	opMap[0x91] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.c)); return 4; };
	opMap[0x92] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.d)); return 4; };
	opMap[0x93] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.e)); return 4; };
	opMap[0x94] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.h)); return 4; };
	opMap[0x95] = [this, &ld8]() {ld8(registers.a, sub(registers.a, registers.l)); return 4; };
	opMap[0x96] = [this, &ld8, &getHL]() {ld8(registers.a, sub(registers.a, mmu.readByte(getHL()))); return 8; };
	opMap[0xD6] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, sub(registers.a, getImmediateValue8())); return 8; };

	//sbc
	opMap[0x9F] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.a)); return 4; };
	opMap[0x98] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.b)); return 4; };
	opMap[0x99] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.c)); return 4; };
	opMap[0x9A] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.d)); return 4; };
	opMap[0x9B] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.e)); return 4; };
	opMap[0x9C] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.h)); return 4; };
	opMap[0x9D] = [this, &ld8]() {ld8(registers.a, sbc(registers.a, registers.l)); return 4; };
	opMap[0x9E] = [this, &ld8, &getHL]() {ld8(registers.a, sbc(registers.a, mmu.readByte(getHL()))); return 8; };
	//�����#�������opcode���ʺ�����ɾ����

	//and
	opMap[0xA7] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.a)); return 4; };
	opMap[0xA0] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.b)); return 4; };
	opMap[0xA1] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.c)); return 4; };
	opMap[0xA2] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.d)); return 4; };
	opMap[0xA3] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.e)); return 4; };
	opMap[0xA4] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.h)); return 4; };
	opMap[0xA5] = [this, &ld8]() {ld8(registers.a, andAL(registers.a, registers.l)); return 4; };
	opMap[0xA6] = [this, &ld8, &getHL]() {ld8(registers.a, andAL(registers.a, mmu.readByte(getHL()))); return 8; };
	opMap[0xE6] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, andAL(registers.a, getImmediateValue8())); return 8; };

	//or
	opMap[0xB7] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.a)); return 4; };
	opMap[0xB0] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.b)); return 4; };
	opMap[0xB1] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.c)); return 4; };
	opMap[0xB2] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.d)); return 4; };
	opMap[0xB3] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.e)); return 4; };
	opMap[0xB4] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.h)); return 4; };
	opMap[0xB5] = [this, &ld8]() {ld8(registers.a, orAL(registers.a, registers.l)); return 4; };
	opMap[0xB6] = [this, &ld8, &getHL]() {ld8(registers.a, orAL(registers.a, mmu.readByte(getHL()))); return 8; };
	opMap[0xF6] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, orAL(registers.a, getImmediateValue8())); return 8; };

	//xor
	opMap[0xAF] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.a)); return 4; };
	opMap[0xA8] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.b)); return 4; };
	opMap[0xA9] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.c)); return 4; };
	opMap[0xAA] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.d)); return 4; };
	opMap[0xAB] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.e)); return 4; };
	opMap[0xAC] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.h)); return 4; };
	opMap[0xAD] = [this, &ld8]() {ld8(registers.a, xorAL(registers.a, registers.l)); return 4; };
	opMap[0xAE] = [this, &ld8, &getHL]() {ld8(registers.a, xorAL(registers.a, mmu.readByte(getHL()))); return 8; };
	opMap[0xEE] = [this, &ld8, &getImmediateValue8]() {ld8(registers.a, xorAL(registers.a, getImmediateValue8())); return 8; };

	//cp
	opMap[0xBF] = [this]() {cp(registers.a, registers.a); return 4; };
	opMap[0xB8] = [this]() {cp(registers.a, registers.b); return 4; };
	opMap[0xB9] = [this]() {cp(registers.a, registers.c); return 4; };
	opMap[0xBA] = [this]() {cp(registers.a, registers.d); return 4; };
	opMap[0xBB] = [this]() {cp(registers.a, registers.e); return 4; };
	opMap[0xBC] = [this]() {cp(registers.a, registers.h); return 4; };
	opMap[0xBD] = [this]() {cp(registers.a, registers.l); return 4; };
	opMap[0xBE] = [this, &getHL]() {cp(registers.a, mmu.readByte(getHL())); return 8; };
	opMap[0xFE] = [this, &getImmediateValue8]() {cp(registers.a, getImmediateValue8()); return 8; };

	//inc
	opMap[0x3C] = [this, &ld8]() {ld8(registers.a, inc(registers.a)); return 4; };
	opMap[0x04] = [this, &ld8]() {ld8(registers.b, inc(registers.b)); return 4; };
	opMap[0x0C] = [this, &ld8]() {ld8(registers.c, inc(registers.c)); return 4; };
	opMap[0x14] = [this, &ld8]() {ld8(registers.d, inc(registers.d)); return 4; };
	opMap[0x1C] = [this, &ld8]() {ld8(registers.e, inc(registers.e)); return 4; };
	opMap[0x24] = [this, &ld8]() {ld8(registers.h, inc(registers.h)); return 4; };
	opMap[0x2C] = [this, &ld8]() {ld8(registers.l, inc(registers.l)); return 4; };
	opMap[0x34] = [this, &getHL, &setHL]() {mmu.writeByte(getHL(),inc(mmu.readByte(getHL()))); return 12; };

	//dec
	opMap[0x3D] = [this, &ld8]() {ld8(registers.a, dec(registers.a)); return 4; };
	opMap[0x05] = [this, &ld8]() {ld8(registers.b, dec(registers.b)); return 4; };
	opMap[0x0D] = [this, &ld8]() {ld8(registers.c, dec(registers.c)); return 4; };
	opMap[0x15] = [this, &ld8]() {ld8(registers.d, dec(registers.d)); return 4; };
	opMap[0x1D] = [this, &ld8]() {ld8(registers.e, dec(registers.e)); return 4; };
	opMap[0x25] = [this, &ld8]() {ld8(registers.h, dec(registers.h)); return 4; };
	opMap[0x2D] = [this, &ld8]() {ld8(registers.l, dec(registers.l)); return 4; };
	opMap[0x35] = [this, &getHL, &setHL]() {mmu.writeByte(getHL(),dec(mmu.readByte(getHL()))); return 12; };

	//16bit opcode

	//add
	opMap[0x09] = [this, &setHL, &getHL, &getBC]() {setHL(add(mmu.readWord(getHL()), mmu.readWord(getBC()))); return 8; };
	opMap[0x19] = [this, &setHL, &getHL, &getDE]() {setHL(add(mmu.readWord(getHL()), mmu.readWord(getDE()))); return 8; };
	opMap[0x29] = [this, &setHL, &getHL]() {setHL(add(mmu.readWord(getHL()), mmu.readWord(getHL()))); return 8; };
	opMap[0x39] = [this, &setHL, &getHL]() {setHL(add(mmu.readWord(getHL()), registers.sp)); return 8; };

	//add sp
	opMap[0xE8] = [this, &ld16, &getImmediateValue16]() {ld16(registers.sp,addSp(registers.sp,getImmediateValue16())); return 16; };

	//inc
	opMap[0x03] = [this, &setBC, &getBC]() {setBC(inc(mmu.readWord(getBC()))); return 8; };
	opMap[0x13] = [this, &setDE, &getDE]() {setDE(inc(mmu.readWord(getDE()))); return 8; };
	opMap[0x23] = [this, &setHL, &getHL]() {setHL(inc(mmu.readWord(getHL()))); return 8; };
	opMap[0x33] = [this, &ld16]() {ld16(registers.sp,inc(registers.sp)); return 8; };

	//dec
	opMap[0x0B] = [this, &setBC, &getBC]() {setBC(dec(mmu.readWord(getBC()))); return 8; };
	opMap[0x1B] = [this, &setDE, &getDE]() {setDE(dec(mmu.readWord(getDE()))); return 8; };
	opMap[0x2B] = [this, &setHL, &getHL]() {setHL(dec(mmu.readWord(getHL()))); return 8; };
	opMap[0x3B] = [this, &ld16]() {ld16(registers.sp, dec(registers.sp)); return 8; };

	opMap[0xCB] = [this, &getImmediateValue8]() {return opCBMap[getImmediateValue8()](); };
	//����
	//swap
	opCBMap[0x37] = [this, &ld8]() {ld8(registers.a, swap(registers.a)); return 8; };
	opCBMap[0x30] = [this, &ld8]() {ld8(registers.b, swap(registers.b)); return 8; };
	opCBMap[0x31] = [this, &ld8]() {ld8(registers.c, swap(registers.c)); return 8; };
	opCBMap[0x32] = [this, &ld8]() {ld8(registers.d, swap(registers.d)); return 8; };
	opCBMap[0x33] = [this, &ld8]() {ld8(registers.e, swap(registers.e)); return 8; };
	opCBMap[0x34] = [this, &ld8]() {ld8(registers.h, swap(registers.h)); return 8; };
	opCBMap[0x35] = [this, &ld8]() {ld8(registers.l, swap(registers.l)); return 8; };
	opCBMap[0x36] = [this, &getHL, &setHL]() {mmu.writeByte(getHL(),swap(mmu.readByte(getHL()))); return 16; };

	//todo: daa 0x27

	//cpl
	opMap[0x2F] = [this, &ld8]() {ld8(registers.a, cpl(registers.a)); return 4; };

	//ccf
	opMap[0x3F] = [this]() {ccf(); return 4; };

	//scf
	opMap[0x37] = [this]() {scf(); return 4; };

	//nop
	opMap[0x00] = [this]() {return 4; };
	//todo: halt 0x76 ok
	//todo: stop 1000
	//todo: EI DI 0xF3 0xF4

	//rlca
	opMap[0x07] = [this, &ld8]() { ld8(registers.a, rlc(registers.a)); return 4; };
	//rla 0x17
	opMap[0x17] = [this, &ld8]() {ld8(registers.a, rl(registers.a)); return 4; };
	//rrca
	opMap[0x0F] = [this, &ld8]() { ld8(registers.a, rrc(registers.a)); return 4; };
	//rra 0x1F
	opMap[0x1F] = [this, &ld8]() {ld8(registers.a, rr(registers.a)); return 4; };

	//rlc
	opCBMap[0x07] = [this, &ld8]() { ld8(registers.a, rlc(registers.a)); return 8; };
	opCBMap[0x00] = [this, &ld8]() { ld8(registers.b, rlc(registers.b)); return 8; };
	opCBMap[0x01] = [this, &ld8]() { ld8(registers.c, rlc(registers.c)); return 8; };
	opCBMap[0x02] = [this, &ld8]() { ld8(registers.d, rlc(registers.d)); return 8; };
	opCBMap[0x03] = [this, &ld8]() { ld8(registers.e, rlc(registers.e)); return 8; };
	opCBMap[0x04] = [this, &ld8]() { ld8(registers.h, rlc(registers.h)); return 8; };
	opCBMap[0x05] = [this, &ld8]() { ld8(registers.l, rlc(registers.l)); return 8; };
	opCBMap[0x06] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),rlc(mmu.readByte(getHL()))); return 16; };

	//rl
	opCBMap[0x17] = [this, &ld8]() { ld8(registers.a, rl(registers.a)); return 8; };
	opCBMap[0x10] = [this, &ld8]() { ld8(registers.b, rl(registers.b)); return 8; };
	opCBMap[0x11] = [this, &ld8]() { ld8(registers.c, rl(registers.c)); return 8; };
	opCBMap[0x12] = [this, &ld8]() { ld8(registers.d, rl(registers.d)); return 8; };
	opCBMap[0x13] = [this, &ld8]() { ld8(registers.e, rl(registers.e)); return 8; };
	opCBMap[0x14] = [this, &ld8]() { ld8(registers.h, rl(registers.h)); return 8; };
	opCBMap[0x15] = [this, &ld8]() { ld8(registers.l, rl(registers.l)); return 8; };
	opCBMap[0x16] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),rl(mmu.readByte(getHL()))); return 16; };
	//rrc
	opCBMap[0x0F] = [this, &ld8]() { ld8(registers.a, rrc(registers.a)); return 8; };
	opCBMap[0x08] = [this, &ld8]() { ld8(registers.b, rrc(registers.b)); return 8; };
	opCBMap[0x09] = [this, &ld8]() { ld8(registers.c, rrc(registers.c)); return 8; };
	opCBMap[0x0A] = [this, &ld8]() { ld8(registers.d, rrc(registers.d)); return 8; };
	opCBMap[0x0B] = [this, &ld8]() { ld8(registers.e, rrc(registers.e)); return 8; };
	opCBMap[0x0C] = [this, &ld8]() { ld8(registers.h, rrc(registers.h)); return 8; };
	opCBMap[0x0D] = [this, &ld8]() { ld8(registers.l, rrc(registers.l)); return 8; };
	opCBMap[0x0E] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),rrc(mmu.readByte(getHL()))); return 16; };
	//rr
	opCBMap[0x1F] = [this, &ld8]() { ld8(registers.a, rr(registers.a)); return 8; };
	opCBMap[0x18] = [this, &ld8]() { ld8(registers.b, rr(registers.b)); return 8; };
	opCBMap[0x19] = [this, &ld8]() { ld8(registers.c, rr(registers.c)); return 8; };
	opCBMap[0x1A] = [this, &ld8]() { ld8(registers.d, rr(registers.d)); return 8; };
	opCBMap[0x1B] = [this, &ld8]() { ld8(registers.e, rr(registers.e)); return 8; };
	opCBMap[0x1C] = [this, &ld8]() { ld8(registers.h, rr(registers.h)); return 8; };
	opCBMap[0x1D] = [this, &ld8]() { ld8(registers.l, rr(registers.l)); return 8; };
	opCBMap[0x1E] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),rr(mmu.readByte(getHL()))); return 16; };

	//sla
	opCBMap[0x27] = [this, &ld8]() { ld8(registers.a, sla(registers.a)); return 8; };
	opCBMap[0x20] = [this, &ld8]() { ld8(registers.b, sla(registers.b)); return 8; };
	opCBMap[0x21] = [this, &ld8]() { ld8(registers.c, sla(registers.c)); return 8; };
	opCBMap[0x22] = [this, &ld8]() { ld8(registers.d, sla(registers.d)); return 8; };
	opCBMap[0x23] = [this, &ld8]() { ld8(registers.e, sla(registers.e)); return 8; };
	opCBMap[0x24] = [this, &ld8]() { ld8(registers.h, sla(registers.h)); return 8; };
	opCBMap[0x25] = [this, &ld8]() { ld8(registers.l, sla(registers.l)); return 8; };
	opCBMap[0x26] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),sla(mmu.readByte(getHL()))); return 16; };

	//sra
	opCBMap[0x2F] = [this, &ld8]() { ld8(registers.a, sra(registers.a)); return 8; };
	opCBMap[0x28] = [this, &ld8]() { ld8(registers.b, sra(registers.b)); return 8; };
	opCBMap[0x29] = [this, &ld8]() { ld8(registers.c, sra(registers.c)); return 8; };
	opCBMap[0x2A] = [this, &ld8]() { ld8(registers.d, sra(registers.d)); return 8; };
	opCBMap[0x2B] = [this, &ld8]() { ld8(registers.e, sra(registers.e)); return 8; };
	opCBMap[0x2C] = [this, &ld8]() { ld8(registers.h, sra(registers.h)); return 8; };
	opCBMap[0x2D] = [this, &ld8]() { ld8(registers.l, sra(registers.l)); return 8; };
	opCBMap[0x2E] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),sra(mmu.readByte(getHL()))); return 16; };

	//srl
	opCBMap[0x3F] = [this, &ld8]() { ld8(registers.a, srl(registers.a)); return 8; };
	opCBMap[0x38] = [this, &ld8]() { ld8(registers.b, srl(registers.b)); return 8; };
	opCBMap[0x39] = [this, &ld8]() { ld8(registers.c, srl(registers.c)); return 8; };
	opCBMap[0x3A] = [this, &ld8]() { ld8(registers.d, srl(registers.d)); return 8; };
	opCBMap[0x3B] = [this, &ld8]() { ld8(registers.e, srl(registers.e)); return 8; };
	opCBMap[0x3C] = [this, &ld8]() { ld8(registers.h, srl(registers.h)); return 8; };
	opCBMap[0x3D] = [this, &ld8]() { ld8(registers.l, srl(registers.l)); return 8; };
	opCBMap[0x3E] = [this, &getHL, &setHL]() { mmu.writeByte(getHL(),srl(mmu.readByte(getHL()))); return 16; };

	//bit opcodes
	opCBMap[0x47] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.a); return 8; };
	opCBMap[0x40] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.b); return 8; };
	opCBMap[0x41] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.c); return 8; };
	opCBMap[0x42] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.d); return 8; };
	opCBMap[0x43] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.e); return 8; };
	opCBMap[0x44] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.h); return 8; };
	opCBMap[0x45] = [this, &getImmediateValue8]() { bit(getImmediateValue8(), registers.l); return 8; };
	opCBMap[0x46] = [this, &getHL, &getImmediateValue8]() {bit(getImmediateValue8(), mmu.readByte(getHL())); return 16; };

	//set
	opCBMap[0xC7] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.a); return 8; };
	opCBMap[0xC0] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.b); return 8; };
	opCBMap[0xC1] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.c); return 8; };
	opCBMap[0xC2] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.d); return 8; };
	opCBMap[0xC3] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.e); return 8; };
	opCBMap[0xC4] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.h); return 8; };
	opCBMap[0xC5] = [this, &getImmediateValue8]() { set(getImmediateValue8(), registers.l); return 8; };
	opCBMap[0xC6] = [this, &getHL, &getImmediateValue8]() {set(getImmediateValue8(), mmu.readByte(getHL())); return 16; };

	//jp
	opMap[0xC3] = [this, &ld16, &getImmediateValue16]() {ld16(registers.sp, getImmediateValue16()); return 12; };
	//jp cc
	opMap[0xC2] = [this, &ld16, &getImmediateValue16]() {if(!getZ()) ld16(registers.sp, getImmediateValue16()); return 12; };
	opMap[0xCA] = [this, &ld16, &getImmediateValue16]() {if(getZ()) ld16(registers.sp, getImmediateValue16()); return 12; };
	opMap[0xD2] = [this, &ld16, &getImmediateValue16]() {if(!getC()) ld16(registers.sp, getImmediateValue16()); return 12; };
	opMap[0xDA] = [this, &ld16, &getImmediateValue16]() {if(getC()) ld16(registers.sp, getImmediateValue16()); return 12; };
	//jp hl
	opMap[0xE9] = [this, &ld16, &getHL]() {ld16(registers.sp, mmu.readWord(getHL())); return 4; };
	//jr
	opMap[0x18] = [this, &ld16, &getSignedImmediateValue8]() {ld16(registers.sp, registers.sp + getSignedImmediateValue8()); return 8; };
	//jr cc
	opMap[0x20] = [this, &ld16, &getSignedImmediateValue8]() {if(!getZ()) ld16(registers.sp, registers.sp + getSignedImmediateValue8()); return 8; };
	opMap[0x28] = [this, &ld16, &getSignedImmediateValue8]() {if(getZ()) ld16(registers.sp, registers.sp + getSignedImmediateValue8()); return 8; };
	opMap[0x30] = [this, &ld16, &getSignedImmediateValue8]() {if(!getC()) ld16(registers.sp, registers.sp + getSignedImmediateValue8()); return 8; };
	opMap[0x38] = [this, &ld16, &getSignedImmediateValue8]() {if(getC()) ld16(registers.sp, registers.sp + getSignedImmediateValue8()); return 8; };
	//call
	opMap[0xCD] = [this, &ld16, &getImmediateValue16]() {mmu.writeByte(registers.sp, registers.a); ld16(registers.sp, getImmediateValue16()); return 12; };
	//call cc
	opMap[0xC4] = [this, &ld16, &getImmediateValue16]() {if (!getZ()) { mmu.writeByte(registers.sp, registers.a); ld16(registers.sp, getImmediateValue16()); } return 12; };
	opMap[0xCC] = [this, &ld16, &getImmediateValue16]() {if (getZ()) { mmu.writeByte(registers.sp, registers.a); ld16(registers.sp, getImmediateValue16()); } return 12; };
	opMap[0xD4] = [this, &ld16, &getImmediateValue16]() {if (!getC()) { mmu.writeByte(registers.sp, registers.a); ld16(registers.sp, getImmediateValue16()); }return 12; };
	opMap[0xDC] = [this, &ld16, &getImmediateValue16]() {if (getC()) { mmu.writeByte(registers.sp, registers.a); ld16(registers.sp, getImmediateValue16()); }return 12; };
	//ret
	opMap[0xC9] = [this, &ld16, &getImmediateValue16, &ld8]() {ld8(registers.a, mmu.readByte(registers.sp)); ld16(registers.sp, getImmediateValue16()); return 8; };
	//ret cc
	opMap[0xC0] = [this, &ld16, &getImmediateValue16, &ld8]() {if (!getZ()) { ld8(registers.a, mmu.readByte(registers.sp)); ld16(registers.sp, getImmediateValue16()); } return 8; };
	opMap[0xC8] = [this, &ld16, &getImmediateValue16, &ld8]() {if (getZ()) { ld8(registers.a, mmu.readByte(registers.sp)); ld16(registers.sp, getImmediateValue16()); }return 8; };
	opMap[0xD0] = [this, &ld16, &getImmediateValue16, &ld8]() {if (!getC()) { ld8(registers.a, mmu.readByte(registers.sp)); ld16(registers.sp, getImmediateValue16()); }return 8; };
	opMap[0xD8] = [this, &ld16, &getImmediateValue16, &ld8]() {if (getC()) { ld8(registers.a, mmu.readByte(registers.sp)); ld16(registers.sp, getImmediateValue16()); }return 8; };
	//todo: reti


	//interrupt
	opMap[0xF3] = [this](){states.interruptMasterEnabled = true; return 4; };
	opMap[0xFB] = [this](){states.interruptMasterEnabled = false; return 4; };
	opMap[0x76] = [this](){states.halt = true; return 4; };


	opMap[0xC7] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x00; return 32;};
	opMap[0xCF] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x08; return 32;};
	opMap[0xD7] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x10; return 32;};
	opMap[0xDF] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x18; return 32;};
	opMap[0xE7] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x20; return 32;};
	opMap[0xEF] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x28; return 32;};
	opMap[0xF7] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x30; return 32;};
	opMap[0xFF] = [this](){rsv(); registers.sp -=2; mmu.writeWord(registers.sp, registers.pc); registers.pc = 0x38; return 32;};

	//DI & EI

	opMap[0xF3] = [this](){states.interruptMasterEnabled = false; return 4; };
	opMap[0xFB] = [this](){states.interruptMasterEnabled = false; return 4; };

}
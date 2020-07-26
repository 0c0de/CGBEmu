#include "CPU.h"
#include "mmu.h"
#include "GUI.h"

MMU mmu;
GameboyRegisters reg;
GameboyFlags flags;


void CPU::init() {
	pc = 0x100;
	reg.AF = 0x01B0;
	reg.A = 0x01;
	reg.F = 0xB0;
	reg.BC = 0x0013;
	reg.B = 0x00;
	reg.C = 0x13;
	reg.DE = 0x00D8;
	reg.D = 0x00;
	reg.E = 0xD8;
	reg.HL = 0x014D;
	reg.H = 0x01;
	reg.L = 0x4D;

	mmu.sp = 0xE000;

	mmu.write8(0xFF05, 0x00);
	mmu.write8(0xFF06, 0x00);
	mmu.write8(0xFF07, 0x00);
	mmu.write8(0xFF10, 0x80);
	mmu.write8(0xFF11, 0xBF);
	mmu.write8(0xFF12, 0xF3);
	mmu.write8(0xFF14, 0xBF);
	mmu.write8(0xFF16, 0x3F);
	mmu.write8(0xFF17, 0x00);
	mmu.write8(0xFF19, 0xBF);
	mmu.write8(0xFF1A, 0x7F);
	mmu.write8(0xFF1B, 0xFF);
	mmu.write8(0xFF1C, 0x9F);
	mmu.write8(0xFF1E, 0xBF);
	mmu.write8(0xFF20, 0xFF);
	mmu.write8(0xFF21, 0x00);
	mmu.write8(0xFF22, 0x00);
	mmu.write8(0xFF23, 0xBF);
	mmu.write8(0xFF24, 0x77);
	mmu.write8(0xFF25, 0xF3);
	mmu.write8(0xFF26, 0xF1);
	mmu.write8(0xFF40, 0x91);
	mmu.write8(0xFF42, 0x00);
	mmu.write8(0xFF43, 0x00);
	mmu.write8(0xFF45, 0x00);
	mmu.write8(0xFF47, 0xFC);
	mmu.write8(0xFF48, 0xFF);
	mmu.write8(0xFF49, 0xFF);
	mmu.write8(0xFF4A, 0x00);
	mmu.write8(0xFF4B, 0x00);
	mmu.write8(0xFFFF, 0x00);
}

void CPU::addCycles(int ciclesToAdd) {
	cicles += ciclesToAdd;
}

void CPU::clearCycles() {
	cicles = 0;
}

MMU *CPU::getMMUValues() {
	return &mmu;
}

void CPU::runLife() {
	uint8_t opcode = mmu.read8(pc);
	//cout << "PC: " << hex << static_cast<unsigned>(pc) << "  |   Opcode: " << hex << static_cast<unsigned>(opcode) << endl;
	switch (opcode) {
		case 0x00:
			NOP();
			break;
		case 0x7F: case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: 
		case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x48: 
		case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x50: case 0x51: 
		case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x58: case 0x59: case 0x36:
		case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x60: case 0x61: case 0x62:
		case 0x63: case 0x64: case 0x65: case 0x66: case 0x68: case 0x69: case 0x6A: case 0x6B:
		case 0x6C: case 0x6D: case 0x6E: case 0x70: case 0x71: case 0x72: case 0x73: case 0x74:
		case 0x75:  
			LD_r1_r2(opcode);
			break;
		case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E:
			LD_NN_N(opcode);
			break;
		case 0x0A: case 0x1A: case 0x3E: case 0xFA:
			LD_A_N(opcode);
			break;
		case 0x47: case 0x4F: case 0x57: case 0x5F: case 0x67: case 0x6F: case 0x02: case 0x12:
		case 0x77: case 0xEA:
			LD_N_A(opcode);
			break;
		case 0xC3:
			JP_NN(opcode);
			break;
		case 0xF3:
			DI();
			break;
		case 0xFB:
			EI();
			break;
		case 0xCD:
			CALL_NN(opcode);
			break;
		case 0xFE: case 0xBF: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE:
			CP_N(opcode);
			break;
		case 0x20: case 0x28: case 0x30: case 0x38:
			JR_CC_N(opcode);
			break;
		case 0x01: case 0x11: case 0x21: case 0x31:
			LD_N_NN(opcode);
			break;
		case 0x2a:
			LDI_A_regHL();
			break;
		case 0x22:
			LDI_regHL_A();
			break;
		case 0x3c: case 0x34: case 0x04: case 0x0c: case 0x14: case 0x1c: case 0x24: case 0x2c:
			INC_N(opcode);
			break;
		case 0x3d: case 0x35: case 0x05: case 0x0d: case 0x15: case 0x1d: case 0x25: case 0x2d:
			DEC_N(opcode);
			break;
		case 0xc9:
			RET();
			break;
		case 0xF0:
			LDH_A_N();
			break;
		case 0xE0:
			LDH_N_A();
			break;
		default:
			cout << "Opcode: " << hex << static_cast<unsigned>(opcode) << " not implemented" << endl;
			break;
	}
}

bool CPU::isHalfCarry(uint8_t a, uint8_t b, std::string type) {
	if (type == "ADD") {
		return (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
	}
	else {
		return ((a & 0xF) - (b & 0xF)) < 0;
	}
}

void CPU::RET() {
	uint16_t addrPoped;
	mmu.pop(&addrPoped);
	std::cout << "RET, Address poped: " << hex << addrPoped << std::endl;
	addCycles(8);
	pc = addrPoped;
}

void CPU::LD_NN_N(uint16_t opcode) {
	uint8_t n = mmu.read8(pc+1);

	switch (opcode)
	{

	case 0x06:
		//reg.B = n;
		mmu.setRegisters8Bit(&reg, "B", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x0E:
		//reg.C = n;
		mmu.setRegisters8Bit(&reg, "C", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x16:
		//reg.D = n;
		mmu.setRegisters8Bit(&reg, "D", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x1E:
		//reg.E = n;
		mmu.setRegisters8Bit(&reg, "E", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x26:
		//reg.H = n;
		mmu.setRegisters8Bit(&reg, "H", n);
		addCycles(8);
		pc += 2;
		break;

	case 0x2E:
		//reg.L = n;
		mmu.setRegisters8Bit(&reg, "L", n);
		addCycles(8);
		pc+=2;
		break;

	default:
		pc += 2;
		break;
	}
}

void CPU::LD_r1_r2(uint16_t opcode) {
	switch (opcode)
	{
	case 0x7F:
		//reg.A = reg.A;
		mmu.setRegisters8Bit(&reg, "A", reg.A);
		addCycles(4);
		pc += 1;
		break;
	case 0x78:
		//reg.A = reg.B;
		mmu.setRegisters8Bit(&reg, "A", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x79:
		//reg.A = reg.C;
		mmu.setRegisters8Bit(&reg, "A", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x7a:
		//reg.A = reg.D;
		mmu.setRegisters8Bit(&reg, "A", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x7b:
		//reg.A = reg.E;
		mmu.setRegisters8Bit(&reg, "A", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x7c:
		//reg.A = reg.H;
		mmu.setRegisters8Bit(&reg, "A", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x7d:
		//reg.A = reg.L;
		mmu.setRegisters8Bit(&reg, "A", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x7e:
		//reg.A = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x40:
		//reg.B = reg.B;
		mmu.setRegisters8Bit(&reg, "B", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x41:
		//reg.B = reg.C;
		mmu.setRegisters8Bit(&reg, "B", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x42:
		//reg.B = reg.D;
		mmu.setRegisters8Bit(&reg, "B", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x43:
		//reg.B = reg.E;
		mmu.setRegisters8Bit(&reg, "B", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x44:
		//reg.B = reg.H;
		mmu.setRegisters8Bit(&reg, "B", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x45:
		//reg.B = reg.L;
		mmu.setRegisters8Bit(&reg, "B", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x46:
		//reg.B = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "B", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x48:
		//reg.C = reg.B;
		mmu.setRegisters8Bit(&reg, "C", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x49:
		//reg.C = reg.C;
		mmu.setRegisters8Bit(&reg, "C", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x4A:
		//reg.C = reg.D;
		mmu.setRegisters8Bit(&reg, "C", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x4B:
		//reg.C = reg.E;
		mmu.setRegisters8Bit(&reg, "C", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x4C:
		//reg.C = reg.H;
		mmu.setRegisters8Bit(&reg, "C", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x4D:
		//reg.C = reg.L;
		mmu.setRegisters8Bit(&reg, "C", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x4E:
		//reg.C = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "C", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x50:
		//reg.D = reg.B;
		mmu.setRegisters8Bit(&reg, "D", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x51:
		//reg.D = reg.C;
		mmu.setRegisters8Bit(&reg, "D", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x52:
		//reg.D = reg.D;
		mmu.setRegisters8Bit(&reg, "D", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x53:
		//reg.D = reg.E;
		mmu.setRegisters8Bit(&reg, "D", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x54:
		//reg.D = reg.H;
		mmu.setRegisters8Bit(&reg, "D", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x55:
		//reg.D = reg.L;
		mmu.setRegisters8Bit(&reg, "D", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x56:
		//reg.D = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "D", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x58:
		//reg.E = reg.B;
		mmu.setRegisters8Bit(&reg, "E", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x59:
		//reg.E = reg.C;
		mmu.setRegisters8Bit(&reg, "E", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x5A:
		//reg.E = reg.D;
		mmu.setRegisters8Bit(&reg, "E", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x5B:
		//reg.E = reg.E;
		mmu.setRegisters8Bit(&reg, "E", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x5C:
		//reg.E = reg.H;
		mmu.setRegisters8Bit(&reg, "E", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x5D:
		//reg.E = reg.L;
		mmu.setRegisters8Bit(&reg, "E", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x5E:
		//reg.E = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "E", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x60:
		//reg.H = reg.B;
		mmu.setRegisters8Bit(&reg, "H", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x61:
		//reg.H = reg.C;
		mmu.setRegisters8Bit(&reg, "H", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x62:
		//reg.H = reg.D;
		mmu.setRegisters8Bit(&reg, "H", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x63:
		//reg.H = reg.E;
		mmu.setRegisters8Bit(&reg, "H", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x64:
		//reg.H = reg.H;
		mmu.setRegisters8Bit(&reg, "H", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x65:
		//reg.H = reg.L;
		mmu.setRegisters8Bit(&reg, "H", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x66:
		//reg.H = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "H", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x68:
		//reg.L = reg.B;
		mmu.setRegisters8Bit(&reg, "L", reg.B);
		addCycles(4);
		pc += 1;
		break;
	case 0x69:
		//reg.L = reg.C;
		mmu.setRegisters8Bit(&reg, "L", reg.C);
		addCycles(4);
		pc += 1;
		break;
	case 0x6A:
		//reg.L = reg.D;
		mmu.setRegisters8Bit(&reg, "L", reg.D);
		addCycles(4);
		pc += 1;
		break;
	case 0x6B:
		//reg.L = reg.E;
		mmu.setRegisters8Bit(&reg, "L", reg.E);
		addCycles(4);
		pc += 1;
		break;
	case 0x6C:
		//reg.L = reg.H;
		mmu.setRegisters8Bit(&reg, "L", reg.H);
		addCycles(4);
		pc += 1;
		break;
	case 0x6D:
		//reg.L = reg.L;
		mmu.setRegisters8Bit(&reg, "L", reg.L);
		addCycles(4);
		pc += 1;
		break;
	case 0x6E:
		//reg.L = mmu.read8(reg.HL);
		mmu.setRegisters8Bit(&reg, "L", mmu.read8(reg.HL));
		addCycles(8);
		pc += 1;
		break;
	case 0x70:
		mmu.write8(mmu.read8(reg.HL), reg.B);
		addCycles(8);
		pc += 1;
		break;
	case 0x71:
		mmu.write8(mmu.read8(reg.HL), reg.C);
		addCycles(8);
		pc += 1;
		break;
	case 0x72:
		mmu.write8(mmu.read8(reg.HL), reg.D);
		addCycles(8);
		pc += 1;
		break;
	case 0x73:
		mmu.write8(mmu.read8(reg.HL), reg.E);
		addCycles(8);
		pc += 1;
		break;
	case 0x74:
		mmu.write8(mmu.read8(reg.HL), reg.H);
		addCycles(8);
		pc += 1;
		break;
	case 0x75:
		mmu.write8(mmu.read8(reg.HL), reg.L);
		addCycles(8);
		pc += 1;
		break;
	case 0x36:
		mmu.write8(mmu.read8(reg.HL), reg.B);
		addCycles(12);
		pc += 1;
		break;
	default:
		pc += 1;
		break;
	}
}

void CPU::LD_A_N(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1);
	uint8_t n = mmu.read8(pc + 1);
	switch (opcode)
	{
	
	case 0x0A:
		std::cout << "LD A, (BC)" << endl;
		//reg.A = mmu.read8(reg.BC);
		mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.BC));
		pc+=1;
		addCycles(8);
		break;
	case 0x1A:
		std::cout << "LD A, (DE)" << endl;
		//reg.A = mmu.read8(reg.DE);
		mmu.setRegisters8Bit(&reg, "A", mmu.read8(reg.DE));
		pc+=1;
		addCycles(8);
		break;
	case 0xFA:
		std::cout << "LD A, (" << hex << static_cast<unsigned>(nn) << "h)" << endl;
		//reg.A = mmu.read8(nn);
		mmu.setRegisters8Bit(&reg, "A", mmu.read(nn));
		pc += 3;
		addCycles(16);
		break;
	case 0x3E:
		std::cout << "LD A, " << hex << static_cast<unsigned>(n) << "h)" << endl;
		//reg.A = mmu.read8(n);
		mmu.setRegisters8Bit(&reg, "A", mmu.read(n));
		pc+=2;
		addCycles(8);
		break;
	default:
		break;
	}
}

void CPU::LD_N_A(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1);
	switch (opcode)
	{
	case 0x47:
		cout << "LD B, A" << endl;
		//reg.B = reg.A;
		mmu.setRegisters8Bit(&reg, "B", reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x4F:
		cout << "LD C, A" << endl;
		//reg.C = reg.A;
		mmu.setRegisters8Bit(&reg, "C", reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x57:
		cout << "LD D, A" << endl;
		mmu.setRegisters8Bit(&reg, "D", reg.A);
		//reg.D = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x5F:
		cout << "LD E, A" << endl;
		mmu.setRegisters8Bit(&reg, "E", reg.A);
		//reg.E = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x67:
		cout << "LD H, A" << endl;
		mmu.setRegisters8Bit(&reg, "H", reg.A);
		//reg.H = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x6F:
		cout << "LD L, A" << endl;
		mmu.setRegisters8Bit(&reg, "L", reg.A);
		//reg.L = reg.A;
		pc += 1;
		addCycles(4);
		break;
	case 0x02:
		cout << "LD (BC), A" << endl;
		mmu.write8(mmu.read(reg.BC), reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x12:
		cout << "LD (DE), A" << endl;
		mmu.write8(mmu.read(reg.DE), reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0x77:
		cout << "LD (HL), A" << endl;
		mmu.write8(mmu.read(reg.HL), reg.A);
		pc += 1;
		addCycles(4);
		break;
	case 0xEA:
		cout << "LD (" << hex << static_cast<unsigned>(nn) << "h), A" << endl;
		//reg.B = reg.A;
		mmu.write8(mmu.read(nn), reg.A);
		pc += 3; 
		addCycles(4);
		break;
	default:
		pc += 1;
		break;
	}
}

void CPU::LD_N_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc + 1);
	switch (opcode)
	{
	case 0x01:
		//reg.BC = nn;
		cout << "LD BC, " << hex << nn << endl;
		mmu.setRegisters16Bit(&reg, "BC", nn);
		pc += 3;
		addCycles(12);
		break;
	case 0x11:
		//reg.DE = nn;
		cout << "LD DE, " << hex << nn << endl;
		mmu.setRegisters16Bit(&reg, "DE", nn);
		pc += 3;
		addCycles(12);
		break;
	case 0x21:
		//reg.HL = nn;
		cout << "LD HL, " << hex << nn << endl;
		mmu.setRegisters16Bit(&reg, "HL", nn);
		pc += 3;
		addCycles(12);
		break;
	case 0x31:
		mmu.sp = nn;
		cout << "LD SP, " << hex << nn << endl;
		pc += 3;
		addCycles(12);
		break;
	}
}

void CPU::NOP() {
	pc++;
	cout << "NOP" << endl;
	addCycles(4);
}

void CPU::JP_NN(uint16_t opcode) {
	pc = mmu.read(pc+1);
	cout << "JP, " << hex << pc << "h" << endl;
	addCycles(12);
}

void CPU::DI() {
	IME = false;
	cout << "DI" << endl;
	addCycles(4);
	pc++;
}

void CPU::EI() {
	IME = true;
	cout << "EI" << endl;
	addCycles(4);
	pc++;
}

void CPU::CALL_NN(uint16_t opcode) {
	uint16_t nn = mmu.read(pc+1);
	cout << "CALL, " << static_cast<unsigned>(nn) << "h" << endl;
	mmu.push(pc+3);
	pc = nn;
	addCycles(12);
}

void CPU::LDH_N_A() {
	uint8_t n = mmu.read8(pc + 1);
	uint16_t memoryAdress = 0xFF0 + n;
	mmu.write8(memoryAdress, reg.A);
	addCycles(12);
	pc += 2;
}

void CPU::LDH_A_N() {
	uint8_t n = mmu.read8(pc + 1);
	uint16_t memoryAdress = 0xFF0 + n;
	mmu.setRegisters8Bit(&reg, "A", mmu.read8(memoryAdress));
	addCycles(12);
	pc += 2;
}

void CPU::CP_N(uint16_t opcode) {
	uint8_t n;
	uint16_t nn;
	
	switch (opcode) {
	case 0xFE:
		n = mmu.read8(pc+1);
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}

		pc += 2;
		addCycles(8);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBF:
		n = reg.A;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xB8:
		n = reg.B;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xB9:
		n = reg.C;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBA:
		n = reg.D;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBB:
		n = reg.E;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBC:
		n = reg.H;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBD:
		n = reg.L;
		if (reg.A == n) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > n) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, n, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(4);
		cout << "CP, A " << hex << static_cast<unsigned>(n) << "h" << endl;
		break;
	case 0xBE:
		nn = mmu.read(reg.HL);
		if (reg.A == nn) {
			flags.Z = true;
		}
		else {
			flags.Z = false;
		}

		flags.N = true;

		if (reg.A > nn) {
			flags.C = true;
		}
		else {
			flags.C = false;
		}

		if (isHalfCarry(reg.A, nn, "SUB")) {
			flags.H = true;
		}
		else {
			flags.H = false;
		}
		pc += 1;
		addCycles(8);
		cout << "CP, A " << hex << static_cast<unsigned>(nn) << "h" << endl;
		break;
	default:
		pc+=2;
	}

}

void CPU::JR_CC_N(uint16_t opcode) {
	uint8_t n = mmu.read8(pc+1);
	switch (opcode) {
		//JR NZ, N
	case 0x20: {
			if (!flags.Z) {
				pc = (pc - (0xff - n)) - 1;
			}
			pc += 2;
			cout << "JR NZ, " << hex << pc << endl;
			addCycles(8);
		}
		break;
		//JR Z, N
	case 0x28: {
			if (flags.Z) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			addCycles(8);
			cout << "JR Z, " << hex << pc << endl;
		}
		break;
		//JR NC, N
	case 0x30: {
			if (!flags.C) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			addCycles(8);
			cout << "JR NC, " << hex << pc << endl;
		}
		break;
		//JR C, N
	case 0x38: {
			if (flags.C) {
				pc = pc - (0xff - n) - 1;
			}
			pc += 2;
			addCycles(8);
			cout << "JR C, " << hex << pc << endl;
		}
		break;

	}
}

void CPU::INC_N(uint16_t opcode) {
	switch (opcode)
	{
		case 0x3C:
			std::cout << "INC A" << endl;
			reg.A++;
			if (reg.A == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.A, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x04:
			std::cout << "INC B" << endl;
			reg.B++;
			if (reg.B == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.B, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x0C:
			std::cout << "INC C" << endl;
			reg.C++; 
			if (reg.C == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.C, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x14:
			std::cout << "INC D" << endl;
			reg.D++;
			if (reg.D == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.D, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x1C:
			std::cout << "INC E" << endl;
			reg.E++;
			if (reg.E == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.E, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x24:
			std::cout << "INC H" << endl;
			reg.H++;
			if (reg.H == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.H, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x2C:
			std::cout << "INC L" << endl;
			reg.L++;
			if (reg.L == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(reg.L, 1, "ADD")) {
				flags.H = true;
			}
			addCycles(4);
			pc++;
			break;
		case 0x34:
			std::cout << "INC (HL)" << endl;
			mmu.write8(reg.HL, mmu.read8(reg.HL) + 1);
			if (mmu.read8(reg.HL) == 0) {
				flags.Z = true;
			}

			if (isHalfCarry(mmu.read8(reg.HL), 1, "ADD")) {
				flags.H = true;
			}
			addCycles(12);
			pc++;
			break;
		default:
			break;
	}
	flags.N = false;
}

void CPU::DEC_N(uint16_t opcode) {
	switch (opcode)
	{
	case 0x3D:
		std::cout << "DEC A" << endl;
		reg.A--;
		if (reg.A == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.A, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x05:
		std::cout << "DEC B" << endl;
		reg.B--;
		if (reg.B == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.B, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x0D:
		std::cout << "DEC C" << endl;
		reg.C--;
		if (reg.C == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.C, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x15:
		std::cout << "DEC D" << endl;
		reg.D--;
		if (reg.D == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.D, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x1D:
		std::cout << "DEC E" << endl;
		reg.E--;
		if (reg.E == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.E, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x25:
		std::cout << "DEC H" << endl;
		reg.H--;
		if (reg.H == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.H, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x2D:
		std::cout << "DEC L" << endl;
		reg.L--;
		if (reg.L == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(reg.L, 1, "SUB")) {
			flags.H = true;
		}
		addCycles(4);
		pc++;
		break;
	case 0x35:
		std::cout << "DEC (HL)" << endl;
		mmu.write8(reg.HL, mmu.read8(reg.HL) - 1);
		if (mmu.read8(reg.HL) == 0) {
			flags.Z = true;
		}

		if (isHalfCarry(mmu.read8(reg.HL), 1, "SUB")) {
			flags.H = true;
		}
		addCycles(12);
		pc++;
		break;
	default:
		break;
	}
	flags.N = false;
}

void CPU::LDI_regHL_A() {
	std::cout << "LDI (HL), A" << endl;
	mmu.write8(mmu.read(reg.HL), reg.A);
	reg.HL++;
	addCycles(8);
	pc++;
}

void CPU::LDI_A_regHL() {
	std::cout << "LDI A, (HL)" << endl;
	reg.A = mmu.read8(reg.HL);
	reg.HL++;
	addCycles(8);
	pc++;
}

GameboyFlags *CPU::getFlagState() {
	return &flags;
}

GameboyRegisters *CPU::getGameboyRegisters() {
	return &reg;
}

//Load the game into memory
void CPU::loadGame(const char* path) {
	//Holds the game
	ifstream game;
	game.open(path, ifstream::binary);
	
	//Set pos at the end
	game.seekg(0, ifstream::end);
	
	//Holds the size of the game
	int gamesize = game.tellg();
	cout << "Size is: " << gamesize << endl;
	
	//Set pos at the beginning
	game.seekg(0, ifstream::beg);
	char* tempGame = new char[gamesize];

	//Dump content into the buffer we created
	game.read(tempGame, gamesize);
	
	//Now dump the content in memory space desired
	for (int x = 0; x < gamesize; x++) {
		mmu.rom[x] = tempGame[x];
		//std::cout << "Address is " << x << ", Value of that is: " << hex << static_cast<unsigned>(mmu.rom[x]) << endl;
	}
	std::cout << "Game loaded into memory" << endl;
	game.close();
}

/*void CPU::loadBIOS() {
	const uint8_t bios[0x100] = {
	0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
	0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
	0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
	0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
	0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
	0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
	0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
	0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
	0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
	0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
	0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
	0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
	0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
	};

	//Dump into the 256 byte rom memory of gameboy
	for (int x = 0; x < 0x100; x++) {
		mmu.bios[x] = bios[x];
	}
}*/
#pragma once
#include <iostream>
#include <stdio.h>
#include "mmu.h"
#include <fstream>
using namespace std;
//CPU is similar to Z80 but with modified instructions
class CPU {

public:
	//IME(Interrupt Master Enable) disabled by DI, enabled by EI
	bool IME;

	//Count cicles for keep games at the same speed
	uint16_t cicles;

	//Program counter of gameboy
	uint16_t pc;
	
	//Screen bidimensional array
	uint16_t gfx[160][144];

	void loadGame(const char* path);

	void runLife();

	void loadBIOS();

	void init();

	void addCycles(int ciclesToAdd);
	
	void clearCycles();

private:
	//8-Bit(1 Byte) Loads
	//LD nn,n
	void LD_NN_N(uint16_t opcode);
	
	//LD r1,r2
	void LD_r1_r2(uint16_t opcode);
	
	//LD A,n
	void LD_A_N(uint16_t opcode);
	
	//LD n,A
	void LD_N_A(uint16_t opcode);
	
	//LD A, (C)
	void LD_A_regC(uint16_t opcode);
	
	//LD (C),A
	void LD_regC_A(uint16_t opcode);
	
	//LDD A, (HL)
	void LDD_A_regHL(uint16_t opcode);

	//LDD (HL), A
	void LDD_regHL_A(uint16_t opcode);

	//LDI A, (HL)
	void LDI_A_regHL(uint16_t opcode);

	//LDI (HL), A
	void LDI_regHL_A(uint16_t opcode);

	//LDH (n), A
	//Put A into memory address $FF00+n.
	void LDH_N_A(uint16_t opcode);

	//LDH A, (n) 
	//Put memory address $FF00 + n into A.
	void LDH_A_N(uint16_t opcode);

	//16-Bit(2 Bytes) Loads
	//LD n, nn
	void LD_N_NN(uint16_t opcode);

	//LD SP, HL
	void LD_SP_HL(uint16_t opcode);

	//LDHL SP, n
	//Put SP + n effective address into HL.
	void LDHL_SP_N(uint16_t opcode);

	//LD (nn), SP
	//Put Stack Pointer (SP) at address n.
	void LD_NN_SP(uint16_t opcode);

	//PUSH NN
	/*Push register pair nn onto stack.
	Decrement Stack Pointer(SP) twice.*/
	void PUSH_NN(uint16_t opcode);

	//POP NN
	/*Pop two bytes off stack into register pair nn.
	Increment Stack Pointer (SP) twice.*/
	void POP_NN(uint16_t opcode);

	//8-Bit ALU (Operations)
	//ADD A,N
	//Adds n to a
	void ADD_A_N(uint16_t opcode);

	//ADC A,n
	//Add n + carry flag to A
	void ADC_A_N(uint16_t opcode);

	//SUB n
	//Substract n from A
	void SUB_N(uint16_t opcode );

	//SUBC A, n
	//Substract n + carry flag from A
	void SUBC_A_N(uint16_t opcode );

	//AND n
	//Use AND with N and the value in register A and saves it in A
	void AND_N(uint16_t opcode );
	
	//OR n
	//Use OR with N and the value in register A and saves it in A
	void OR_N(uint16_t opcode );

	//XOR n
	//Use XOR with N and the value in register A and saves it in A
	void XOR_N(uint16_t opcode );

	//CP n
	/*Compare A with n. This is basically an A - n
	subtraction instruction but the results are thrown away.*/
	void CP_N(uint16_t opcode );

	//INC n
	//Increment register n
	void INC_N(uint16_t opcode );

	//DEC n
	//Decrement register n
	void DEC_N(uint16_t opcode );

	//16 Bit ALU
	//ADD HL, n
	//Adds n to HL
	void ADD_HL_N(uint16_t opcode );

	//ADD SP, n
	//Adds n to stack pointer(SP)
	void ADD_SP_N(uint16_t opcode );

	//INC nn
	//Increment register nn
	void INC_NN(uint16_t opcode);

	//DEC nn
	//Decrement register nn
	void DEC_NN(uint16_t opcode);

	//Other UGLY Instructions
	//SWAP n
	//Swap upper and lower nibbles of n
	void SWAP_N(uint16_t opcode );

	//DAA
	/*Decimal adjust register A.
	This instruction adjusts register A so that the
	correct representation of Binary Coded Decimal(BCD)	is obtained.*/
	void DAA();

	//CPL
	/*Complement A register. (Flip all bits.)*/
	void CPL( );

	//CCF
	/*Complement carry flag*/
	void CCF( );

	//SCF
	/*Set carry flag*/
	void SCF( );

	//NOP just pass to the next instruction
	void NOP();
	
	//HALT
	/*Power down CPU, for improve battery life*/
	void HALT();

	//STOP
	/*Halt CPU and LCD Screen until a button is pressed*/
	void STOP();

	//Disable Interruptions
	void DI();
	
	//Enable Interruptions
	void EI();

	//Rotates and Shifts operations
	//RLCA
	/*Rotate A left. Old bit 7 to Carry flag.*/
	void RLCA( );

	//RLA
	/*Rotate A left through Carry flag.*/
	void RLA( );
	
	//RRCA
	/*Rotate A right Old bit 0 to Carry flag.*/
	void RRCA( );

	//RRA
	/*Rotate A right through Carry flag.*/
	void RRA( );

	//RLC n
	/*Rotate n left. Old bit 7 to Carry flag*/
	void RLC_N( );

	//RL n
	/*Rotate n left. through Carry flag*/
	void RL_N( );

	//RRC n
	/*Rotate n right. Old bit 7 to Carry flag*/
	void RRC_N();

	//RR n
	/*Rotate n right. through Carry flag*/
	void RR_N();

	//SLA n
	//Shift n left into Carry. LSB of n set to 0.
	void SLA_N();

	//SRA n
	//Shift n right into Carry. MSB doesn't change
	void SRA_N();

	//SRL n
	//Shift n right into Carry. MSB set to 0.
	void SRL_N();

	//Bit codes
	//BIT b,r
	//Test bit b in register r.
	void BIT_B_R(uint16_t opcode );

	//SET b,r
	//Set bit b in register r
	void SET_B_R(uint16_t opcode);

	//RES b,r
	//Reset bit b in register r
	void RES_B_R(uint16_t opcode);


	//Jumps
	//JP NN
	//Jump to address NN
	void JP_NN(uint16_t opcode);

	//JP CC, NN
	//Jump if condition is true
	void JR_CC_NN(uint16_t opcode);

	//JP (HL)
	//Jump to HL stored address
	void JP_regHL();

	//JR n
	//Makes a relative jump, adds n to the PC and jumps to that address
	void JR_N();

	//JR CC, N
	//If condition is true then adds n to PC and jumps to that address
	void JR_CC_N(uint16_t opcode);

	//Calls
	//Push address of next instruction to stack and then jump to address NN
	void CALL_NN(uint16_t opcode);

	//Same of CALL NN but if condition is true
	void CALL_CC_N(uint16_t opcode);
	
	//Restarts
	//RST n
	//Push actual address into stack and jump to 0x0000 + n
	void RST_N(uint16_t opcode);

	//Returns
	//RET
	//Pop two bytes of the stack and jump to that address
	void RET();

	//RET CC
	//Same as RET but if condition is true
	void RET_CC(uint16_t opcode);

	//RETI
	/*Pop two bytes from stack and jump to that address, then enables
	Interrups*/
	void RETI();
};
#include "CPU.h"
#include "Bus.h"
#include "HELPER.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace HELPER;

const array<string, 0xFF + 1> CPU::opcodes =
{
	"NOP              ",
	"LD PC, u16       ",
	"LD (BC), A       ",
	"INC BC           ",
	"INC B            ",
	"DEC B            ",
	"LD B, u8         ",
	"RLCA             ",
	"LD (u16), SP     ",
	"ADD HL, BC       ",
	"LD A, (BC)       ",
	"DEC BC           ",
	"INC C            ",
	"DEC C            ",
	"LD C, u8         ",
	"RRCA             ",
	"STOP             ",
	"LD DE, u16       ",
	"LD (DE), A       ",
	"INC DE           ",
	"INC D            ",
	"DEC D            ",
	"LD D, u8         ",
	"RLA              ",
	"JR i8            ",
	"ADD HL, DE       ",
	"LD A, (DE)       ",
	"DEC DE           ",
	"INC E            ",
	"DEC E            ",
	"LD E,u8          ",
	"RRA              ",
	"JR NZ, i8        ",
	"LD HL, u16       ",
	"LD (HL+), A      ",
	"INC HL           ",
	"INC H            ",
	"DEC H            ",
	"LD H, u8         ",
	"DAA              ",
	"JR Z, i8         ",
	"ADD HL, HL       ",
	"LD A, (HL+)      ",
	"DEC HL           ",
	"INC L            ",
	"DEC L            ",
	"LD L, u8         ",
	"CPL              ",
	"JR NC, i8        ",
	"LD SP, u16       ",
	"LD (HL-), A      ",
	"INC SP           ",
	"INC (HL)         ",
	"DEC (HL)         ",
	"LD (HL), u8      ",
	"SCF              ",
	"JR C, i8         ",
	"ADD HL, SP       ",
	"LD A, (HL-)      ",
	"DEC SP           ",
	"INC A            ",
	"DEC A            ",
	"LD A, u8         ",
	"CCF              ",
	"LD B, B          ",
	"LD B, C          ",
	"LD B, D          ",
	"LD B, E          ",
	"LD B, H          ",
	"LD B, L          ",
	"LD B, (HL)       ",
	"LD B, A          ",
	"LD C, B          ",
	"LD C, C          ",
	"LD C, D          ",
	"LD C, E          ",
	"LD C, H          ",
	"LD C, L          ",
	"LD C, (HL)       ",
	"LD C, A          ",
	"LD D, B          ",
	"LD D, C          ",
	"LD D, D          ",
	"LD D, E          ",
	"LD D, H          ",
	"LD D, L          ",
	"LD D, (HL)       ",
	"LD D, A          ",
	"LD E, B          ",
	"LD E, C          ",
	"LD E, D          ",
	"LD E, E          ",
	"LD E, H          ",
	"LD E, L          ",
	"LD E, (HL)       ",
	"LD E, A          ",
	"LD H, B          ",
	"LD H, C          ",
	"LD H, D          ",
	"LD H, E          ",
	"LD H, H          ",
	"LD H, L          ",
	"LD H, (HL)       ",
	"LD H, A          ",
	"LD L, B          ",
	"LD L, C          ",
	"LD L, D          ",
	"LD L, E          ",
	"LD L, H          ",
	"LD L, L          ",
	"LD L, (HL)       ",
	"LD L, A          ",
	"LD (HL), B       ",
	"LD (HL), C       ",
	"LD (HL), D       ",
	"LD (HL), E       ",
	"LD (HL), H       ",
	"LD (HL), L       ",
	"HALT             ",
	"LD (HL), A       ",
	"LD A, B          ",
	"LD A, C          ",
	"LD A, D          ",
	"LD A, E          ",
	"LD A, H          ",
	"LD A, L          ",
	"LD A, (HL)       ",
	"LD A, A          ",
	"ADD A, B         ",
	"ADD A, C         ",
	"ADD A, D         ",
	"ADD A, E         ",
	"ADD A, H         ",
	"ADD A, L         ",
	"ADD A, (HL)      ",
	"ADD A, A         ",
	"ADC A, B         ",
	"ADC A, C         ",
	"ADC A, D         ",
	"ADC A, E         ",
	"ADC A, H         ",
	"ADC A, L         ",
	"ADC A, (HL)      ",
	"ADC A, A         ",
	"SUB A, B         ",
	"SUB A, C         ",
	"SUB A, D         ",
	"SUB A, E         ",
	"SUB A, H         ",
	"SUB A, L         ",
	"SUB A, (HL)      ",
	"SUB A, A         ",
	"SBC A, B         ",
	"SBC A, C         ",
	"SBC A, D         ",
	"SBC A, E         ",
	"SBC A, H         ",
	"SBC A, L         ",
	"SBC A, (HL)      ",
	"SBC A, A         ",
	"AND A, B         ",
	"AND A, C         ",
	"AND A, D         ",
	"AND A, E         ",
	"AND A, H         ",
	"AND A, L         ",
	"AND A, (HL)      ",
	"AND A, A         ",
	"XOR A, B         ",
	"XOR A, C         ",
	"XOR A, D         ",
	"XOR A, E         ",
	"XOR A, H         ",
	"XOR A, L         ",
	"XOR A, (HL)      ",
	"XOR A, A         ",
	"OR A, B          ",
	"OR A, C          ",
	"OR A, D          ",
	"OR A, E          ",
	"OR A, H          ",
	"OR A, L          ",
	"OR A, (HL)       ",
	"OR A, A          ",
	"CP A, B          ",
	"CP A, C          ",
	"CP A, D          ",
	"CP A, E          ",
	"CP A, H          ",
	"CP A, L          ",
	"CP A, (HL)       ",
	"CP A, A          ",
	"RET NZ           ",
	"POP BC           ",
	"JP NZ, u16       ",
	"JP u16           ",
	"CALL NZ, u16     ",
	"PUSH BC          ",
	"ADD A, u8        ",
	"RST 00h          ",
	"RET Z            ",
	"RET              ",
	"JP Z, u16        ",
	"PREFIX CB        ",
	"CALL Z, u16      ",
	"CALL u16         ",
	"ADC A, u8        ",
	"RST 08h          ",
	"RET NC           ",
	"POP DE           ",
	"JP NC, u16       ",
	"XXX              ",
	"CALL NC, u16     ",
	"PUSH DE          ",
	"SUB A, u8        ",
	"RST 10h          ",
	"RET C            ",
	"RETI             ",
	"JP C, u16        ",
	"XXX              ",
	"CALL C, u16      ",
	"XXX              ",
	"SBC A, u8        ",
	"RST 18h          ",
	"LD (FF00 + u8), A",
	"POP HL           ",
	"LD (FF00 + C), A ",
	"XXX              ",
	"XXX              ",
	"PUSH HL          ",
	"AND A, u8        ",
	"RST 20h          ",
	"ADD SP, i8       ",
	"JP HL            ",
	"LD (u16), A      ",
	"XXX              ",
	"XXX              ",
	"XXX              ",
	"XOR A, u8        ",
	"RST 28h          ",
	"LD A, (FF00 + u8)",
	"POP AF           ",
	"LD A, (FF00 + C) ",
	"DI               ",
	"XXX              ",
	"PUSH AD          ",
	"OR A, u8         ",
	"RST 30h          ",
	"LD HL, SP + i8   ",
	"LD A, (u16)      ",
	"EI               ",
	"XXX              ",
	"XXX              ",
	"CP A, u8         ",
	"RST 38h          ",
};

void CPU::reset()
{
	regA = 0x01; regB = 0x00; regC = 0x13; regD = 0x00; regE = 0xD8; regF = 0xB0; regH = 0x01; regL = 0x4D;
	SP = 0xFFFE; PC = 0x0101;
	opcode = 0x00;
	cyclesLeft = 4;
	totalCycles = 0;
	IME = 1;
	timerCounter = 0; constantTimerCounter = 0;

	Write(IE, (uint8_t)0xFF);
	Write(IF, (uint8_t)0x00);
}

stringstream logString;
void CPU::log()
{
	logString.str(std::string());
	uint8_t tac = ReadByte(TAC);
	logString <<
		"A: " << setfill('0') << setw(2) << uppercase << hex << +regA <<
		" F: " << setfill('0') << setw(2) << uppercase << hex << +regF <<
		" B: " << setfill('0') << setw(2) << uppercase << hex << +regB <<
		" C: " << setfill('0') << setw(2) << uppercase << hex << +regC <<
		" D: " << setfill('0') << setw(2) << uppercase << hex << +regD <<
		" E: " << setfill('0') << setw(2) << uppercase << hex << +regE <<
		" H: " << setfill('0') << setw(2) << uppercase << hex << +regH <<
		" L: " << setfill('0') << setw(2) << uppercase << hex << +regL <<
		" SP: " << setfill('0') << setw(4) << uppercase << hex << +SP <<
		" PC: 00:" << setfill('0') << setw(4) << uppercase << hex << +PC <<
		" (" << setfill('0') << setw(2) << uppercase << hex << +ReadByte(PC) <<
		" " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(PC + 1) <<
		" " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(PC + 2) <<
		" " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(PC + 3) <<
		") " <<
		"IF: " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(IF) <<
		" IE: " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(IE) <<
		" IME: " << setfill('0') << setw(1) << uppercase << hex << +IME <<
		" TIMA: " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(TIMA) <<
		" TMA: " << setfill('0') << setw(2) << uppercase << hex << +ReadByte(TMA) <<
		" TAC: " << +clamp(tac & Bit(2), 0, 1) << +clamp(tac & Bit(1), 0, 1) << +(tac & 1) <<
		" CYC: " << dec << +totalCycles << endl;
	Log(logString.str());
}

bool scheduleIME = 0;
void CPU::cycle()
{
	cyclesLeft--;
	totalCycles++;

	// clock handling
	timerCounter++;
	uint16_t timerMax = TimerTable(ReadByte(TAC));
	if (timerMax != 0)
	{
		if (timerCounter >= timerMax)
		{
			timerCounter = 0;
			WriteOffset(TIMA);
			if (ReadByte(TIMA) == 0) // overflow
			{
				Write(TIMA, ReadByte(TMA));
				RequestInterrupt(2);
			}
		}
	}
	constantTimerCounter++;
	if (constantTimerCounter == 0)
	{
		constantTimerCounter = 0;
		WriteOffset(DIV);
	}

	//if (cyclesLeft < 0)
	//{
	//	WriteLog();
	//	throw std::exception();
	//}
	if (cyclesLeft > 0)
		return;
	else if (cyclesLeft < 0)
	{
		//log();
		handleInterrupts();
		return;
	}

	log();

	opcode = ReadByte();
	//cout << logString.str() << endl;

	/*
	* opcodes can be represented as follows: (https://gb-archive.github.io/salvage/decoding_gbz80_opcodes/Decoding%20Gamboy%20Z80%20Opcodes.html)
	* (7 6)(5  4  3)(2  1  0)
	* \   /\+-p+  q/\       / (p is bit 5 and 4)
	*  \x/  \  y  /  \  z  /
	*/
	uint8_t x = Bits(opcode, 6, 2);
	uint8_t y = Bits(opcode, 3, 3);
		uint8_t p = Bits(y, 1, 2);
		uint8_t q = Bit(y, 0);
	uint8_t z = Bits(opcode, 0, 3);

	if (scheduleIME == 1)
	{
		IME = true;
		scheduleIME = 0;
	}

	// laid out using (https://cdn.discordapp.com/attachments/465586075830845475/742438340078469150/SM83_decoding.pdf)
	// note that the program counter is assumed to be pointing to the byte next to current opcode
	switch (x)
	{
	case 0b00:
		switch (z)
		{
		case 0b000:
			if ((y & 0b100) > 0) // JR condition/cc i8
			{
				int16_t rel = (int8_t)ReadByte();
				uint8_t condition = Bits(y, 0, 2);
				if (ConditionTable(condition))
				{
					// jump relative (move program counter by next byte's size)
					PC += rel; // cast to int8 because that uses two's complement to get negative values
					cyclesLeft = 12;
				}
				else
					cyclesLeft = 8;
			}
			else
				switch (y)
				{
				case 0b000: // NOP
					cyclesLeft = 4;
					break;
				case 0b001: // LD (u16), SP
				{
					// fetch pointer
					uint16_t pointer = ReadWord();

					Write(pointer, SP);

					cyclesLeft = 20;
					break;
				}
				case 0b010: // STOP
					// skips the next byte
					PC++;
					cyclesLeft = 4;
					break;
				case 0b011: // JR i8
				{
					// jump relative (move program counter by next byte's size)
					int16_t rel = (int8_t)ReadByte();
					PC += rel; // cast to int8 because that uses two's complement to get negative values
					cyclesLeft = 12;
					break;
				}
				default:
					break;
				}
			break;
		case 0b001:
			// inverted to get case 0 before case 1
			if (!q) // LD r16, u16
			{
				uint16_t result = ReadWord();
				R16Table1Ref(p, result);
				cyclesLeft = 12;
			}
			else // ADD HL, r16
			{
				uint16_t HL = BytesToWord(regH, regL);
				uint16_t add = R16Table1Val(p);
				bool carryCheck = (((uint32_t)HL + (uint32_t)add) & 0x10000) == 0x10000;
				bool halfCarryCheck = (((HL & 0xFFF) + (add & 0xFFF)) & 0x1000) == 0x1000;
				HL += add;
				R16Table1Ref(0b10, HL);

				SetFlag(FlagN, 0);
				SetFlag(FlagC, carryCheck); // set the carry flag if HL before the addition is bigger than it is now (meaning overflow)
				SetFlag(FlagH, halfCarryCheck); // set the half carry flag if the lower byte of HL before the addition is smaller than it is now (meaning overflow)
				cyclesLeft = 8;
			}
			break;
		case 0b010:
			if (!q) // LD (r16), A
			{
				// get pointer register from r16 table 2
				uint16_t regValue = R16Table2Val(p);
				// that register acts like an address in memory, and this instruction loads regA into that address
				bus->cpuWrite(regValue, regA);
			}
			else // LD A, (r16)
			{
				// effectively the reverse of the operation before it
				// get pointer register from r16 table 2
				uint16_t regValue = R16Table2Val(p);
				// that register, and this instruction loads that address's data into regA
				uint8_t data = ReadByte(regValue);
				
				regA = data;
			}
			cyclesLeft = 8;
			break;
		case 0b011:
			if (!q) // INC r16
				R16Table1Ref(p, R16Table1Val(p) + 1);
			else // DEC r16
				R16Table1Ref(p, R16Table1Val(p) - 1);
			cyclesLeft = 8;
			break;
		case 0b100: // INC r8
		{
			uint8_t prevVal = R8TableVal(y);
			R8TableRef(y, prevVal + 1);
			SetFlag(FlagZ, R8TableVal(y) == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, (((prevVal & 0x0F) + (1 & 0x0F)) & 0x10) == 0x10);
			if (y == 0b110) // HL pointer takes more time
				cyclesLeft = 12;
			else
				cyclesLeft = 4;
			break;
		}
		case 0b101: // DEC r8
		{
			uint8_t prevVal = R8TableVal(y);
			R8TableRef(y, prevVal - 1);
			SetFlag(FlagZ, R8TableVal(y) == 0);
			SetFlag(FlagN, 1);
			SetFlag(FlagH, (prevVal & 0x0F) < (1 & 0x0F));
			if (y == 0b110) // HL pointer takes more time
				cyclesLeft = 12;
			else
				cyclesLeft = 4;
			break;
		}
		case 0b110: // LD r8, u8
		{
			uint8_t result = ReadByte();
			R8TableRef(y, result);
			if (y == 0b110) // HL pointer takes more time
				cyclesLeft = 12;
			else
				cyclesLeft = 8;
			break;
		}
		case 0b111:
			uint8_t carry;
			switch (y)
			{
			case 0b000: // RLCA
				carry = Bit(regA, 7); // msb of reg A
				regA = (regA << 1) | carry; // "rotate" reg A to the left

				SetFlag(FlagZ, 0);
				SetFlag(FlagN, 0);
				SetFlag(FlagH, 0);
				SetFlag(FlagC, carry);
				cyclesLeft = 4;
				break;
			case 0b001: // RRCA
				carry = Bit(regA, 0); // lsb of reg A
				regA = (regA >> 1) | (carry << 7); // "rotate" reg A to the left

				SetFlag(FlagZ, 0);
				SetFlag(FlagN, 0);
				SetFlag(FlagH, 0);
				SetFlag(FlagC, carry);
				cyclesLeft = 4;
				break;
			case 0b010: // RLA
				carry = Bit(regA, 7); // msb of reg A
				regA = (regA << 1) | (int)GetFlag(FlagC);

				SetFlag(FlagZ, 0);
				SetFlag(FlagN, 0);
				SetFlag(FlagH, 0);
				SetFlag(FlagC, carry);
				cyclesLeft = 4;
				break;
			case 0b011: // RRA
				carry = Bit(regA, 0); // lsb of reg A
				regA = (regA >> 1) | ((int)GetFlag(FlagC) << 7);

				SetFlag(FlagZ, 0);
				SetFlag(FlagN, 0);
				SetFlag(FlagH, 0);
				SetFlag(FlagC, carry);
				cyclesLeft = 4;
				break;
			case 0b100: // DAA
				// note: assumes a is a uint8_t and wraps from 0xff to 0
				if (!GetFlag(FlagN))// after an addition, adjust if (half-)carry occurred or if result is out of bounds
				{  
					if (GetFlag(FlagC) || regA > 0x99) { regA += 0x60; SetFlag(FlagC, true); }
					if (GetFlag(FlagH) || (regA & 0x0F) > 0x09) { regA += 0x06; }
				}
				else // after a subtraction, only adjust if (half-)carry occurred
				{
					if (GetFlag(FlagC)) { regA -= 0x60; }
					if (GetFlag(FlagH)) { regA -= 0x06; }
				}
				// these flags are always updated
				SetFlag(FlagZ, regA == 0); // the usual z flag
				SetFlag(FlagH, 0); // h flag is always cleared
				cyclesLeft = 4;
				break;
			case 0b101: // CPL
				regA = ~regA;
				SetFlag(FlagN, 1);
				SetFlag(FlagH, 1);
				cyclesLeft = 4;
				break;
			case 0b110: // SCF
				SetFlag(FlagC, 1);
				SetFlag(FlagN, 0);
				SetFlag(FlagH, 0);
				cyclesLeft = 4;
				break;
			case 0b111: // CCF
				SetFlag(FlagC, !GetFlag(FlagC));
				SetFlag(FlagN, 0);
				SetFlag(FlagH, 0);
				cyclesLeft = 4;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case 0b01:
		if (opcode == 0b01110110) // HALT
		{
			cyclesLeft = -1;
			break;
		}
		/*
		* the reason why checking for HALT first is important is
		* because this instruction can potentially override HALT
		*/
		R8TableRef(y, R8TableVal(z)); // LD r8, r8
		cyclesLeft = 8;
		break;
	case 0b10: // ALU A, r8
	{
		uint8_t r8 = R8TableVal(z);
		ALU(y, r8);
		if (y == 0b110) // HL pointer takes more time
			cyclesLeft = 8;
		else
			cyclesLeft = 4;
		break;
	}
	case 0b11:
	{
		switch (z)
		{
		case 0b000:
			if (((y >> 2) & 1) == 0) // RET condition/cc
			{
				uint8_t condition = Bits(y, 0, 2);
				if (ConditionTable(condition))
				{
					PC = PopFromStack(true);
					cyclesLeft = 20;
				}
				else
					cyclesLeft = 8;
			}
			else
			{
				switch (y)
				{
				case 0b100: // LD (FF00 + u8), A
				{
					uint8_t offset = ReadByte();
					uint16_t pointer = 0xFF00 + offset;
					Write(pointer, regA);
					cyclesLeft = 12;
					break;
				}
				case 0b101: // ADD SP, i8
				{
					int16_t offset = (int8_t)ReadByte();
					uint16_t prevSP = SP;
					SP += offset;
					SetFlag(FlagZ, 0);
					SetFlag(FlagN, 0);
					SetFlag(FlagC, ((prevSP ^ offset ^ SP) & 0x100) == 0x100);
					SetFlag(FlagH, ((prevSP ^ offset ^ SP) & 0x10) == 0x10);
					cyclesLeft = 16;
					break;
				}
				case 0b110: // LD A, (FF00 + u8)
				{
					uint8_t offset = ReadByte();
					uint16_t pointer = 0xFF00 | offset;
					regA = ReadByte(pointer);
					cyclesLeft = 12;
					break;
				}
				case 0b111: // LD HL, SP + i8
				{
					int8_t offset = (int8_t)ReadByte();
					uint16_t result = SP + offset;
					uint16_t prevVal = R16Table1Val(0b10);
					R16Table1Ref(0b10, result);
					SetFlag(FlagZ, 0);
					SetFlag(FlagN, 0);
					SetFlag(FlagC, ((SP ^ offset ^ result) & 0x100) == 0x100);
					SetFlag(FlagH, ((SP ^ offset ^ result) & 0x10) == 0x10);
					cyclesLeft = 12;
					break;
				}
				default:
					break;
				}
			}
			break;
		case 0b001:
			if ((y & 1) == 0) // POP r16
			{
				R16Table3Ref(p, PopFromStack(true));
				cyclesLeft = 12;
			}
			else
			{
				uint16_t result;
				switch (p)
				{
				case 0b00: // RET
					result = PopFromStack(true);

					PC = result;
					cyclesLeft = 16;
					break;
				case 0b01: // RETI
					result = PopFromStack(true);

					PC = result;
					scheduleIME = 1;
					cyclesLeft = 16;
					break;
				case 0b10: // JP HL
					PC = R16Table1Val(0b10);
					cyclesLeft = 4;
					break;
				case 0b11: // LD SP, HL
					SP = R16Table1Val(0b10);
					cyclesLeft = 8;
					break;
				default:
					break;
				}
			}
			break;
		case 0b010:
			if (Bit(y, 2) == 0) // JP condition/cc
			{
				uint16_t result = ReadWord();
				uint8_t condition = Bits(y, 0, 2);
				if (ConditionTable(condition))
				{
					PC = result;
					cyclesLeft = 16;
				}
				else
					cyclesLeft = 12;
			}
			else
			{
				uint16_t pointer;
				switch (y)
				{
				case 0b100: // LD (FF00 + C), A
					pointer = 0xFF00 | regC;
					Write(pointer, regA);

					cyclesLeft = 8;
					break;
				case 0b101: // LD (u16), A
					pointer = ReadWord();

					Write(pointer, regA);

					cyclesLeft = 16;
					break;
				case 0b110: // LD A, (FF00 + C)
					pointer = 0xFF00 | regC;
					regA = ReadByte(pointer);

					cyclesLeft = 8;
					break;
				case 0b111: // LD A, (u16)
					pointer = ReadWord();

					regA = ReadByte(pointer);

					cyclesLeft = 16;
					break;
				default:
					break;
				}
			}
			break;
		case 0b011:
			switch (y)
			{
			case 0b000: // JP u16
			{
				PC = ReadWord();
				cyclesLeft = 16;
				break;
			}
			case 0b001: // switch to CB opcode table
				CB();
				break;
			case 0b110: // DI
				IME = false;
				cyclesLeft = 4;
				break;
			case 0b111: // EI
				// wait until next instruction to enable IME
				scheduleIME = 1;
				cyclesLeft = 4;
				break;
			default:
				break;
			}
			break;
		case 0b100: // CALL condition/cc u16
		{
			uint16_t result = ReadWord();
			uint8_t condition = Bits(y, 0, 2);
			if (ConditionTable(condition))
			{
				PushToStack(PC);
				PC = result;
				cyclesLeft = 24;
			}
			else
				cyclesLeft = 12;
			break;
		}
		case 0b101:
			if ((y & 1) == 0) // PUSH r16
			{
				uint16_t result = R16Table3Val(p);
				PushToStack(result);
				cyclesLeft = 16;
			}
			else // CALL u16
			{
				uint16_t result = ReadWord();

				PushToStack(PC);

				PC = result;
				cyclesLeft = 24;
			}
			break;
		case 0b110: // ALU A, u8
		{
			uint8_t u8 = ReadByte();
			ALU(y, u8);
			cyclesLeft = 8;
			break;
		}
		case 0b111: // RST
		{
			PushToStack(PC);

			uint16_t call = (uint8_t)y << 3;
			PC = call;
			cyclesLeft = 16;
			break;
		}
		default:
			break;
		}
	}
	default:
		break;
	}

	handleInterrupts();
}

// interrupt handling (https://gbdev.io/pandocs/Interrupts.html)
void CPU::handleInterrupts()
{
	if (IME)
	{
		if (PendingInterrupts())
		{
			uint8_t IEValue = ReadByte(IE);
			uint8_t IFValue = ReadByte(IF);

			uint8_t filtered = IEValue & IFValue & 0b11111;
			uint8_t interruptLocations[] = { 0x40, 0x48, 0x50, 0x58, 0x60 };
			for (uint8_t i = 0; i < 5; i++)
			{
				if (!(filtered & Bit(i)))
					continue;

				// interrupt happened
				WriteAnd(IF, (uint8_t)~Bit(i));
				IME = 0;
				PushToStack(PC);

				PC = interruptLocations[i];
				cyclesLeft = 20;
				break;
			}
		}
	}
	else if (cyclesLeft < 0 && PendingInterrupts())
		cyclesLeft = 1;
}

uint8_t CPU::R8TableVal(uint8_t x)
{
	x &= 0b00000111;
	switch (x)
	{
	case 0b000:
		return regB;
	case 0b001:
		return regC;
	case 0b010:
		return regD;
	case 0b011:
		return regE;
	case 0b100:
		return regH;
	case 0b101:
		return regL;
	case 0b110: // HL points to address that contains data
		return bus->cpuRead(BytesToWord(regH, regL));
	case 0b111:
		return regA;
	default:
		return 0;
	}
}
uint16_t CPU::R16Table1Val(uint8_t x)
{
	x &= 0b00000011;
	switch (x)
	{
	case 0b00:
		return BytesToWord(regB, regC);
	case 0b01:
		return BytesToWord(regD, regE);
	case 0b10:
		return BytesToWord(regH, regL);
	case 0b11:
		return SP;
	default:
		return 0;
	}
}
uint16_t CPU::R16Table2Val(uint8_t x)
{
	x &= 0b00000011;
	int result;
	switch (x)
	{
	case 0b00:
		result = BytesToWord(regB, regC);
		break;
	case 0b01:
		result = BytesToWord(regD, regE);
		break;
	case 0b10:
		result = BytesToWord(regH, regL);
		result++;
		regH = result >> 8;
		regL = result & 0x00FF; // low of byte
		result--;
		break;
	case 0b11:
		result = BytesToWord(regH, regL);
		result--;
		regH = result >> 8;
		regL = result & 0x00FF; // low of byte
		result++;
		break;
	default:
		result = 0;
		break;
	}
	return result;
}
uint16_t CPU::R16Table3Val(uint8_t x)
{
	x &= 0b00000011;
	switch (x)
	{
	case 0b00:
		return BytesToWord(regB, regC);
	case 0b01:
		return BytesToWord(regD, regE);
	case 0b10:
		return BytesToWord(regH, regL);
	case 0b11:
		return BytesToWord(regA, regF);
	default:
		return 0;
	}
}
bool CPU::ConditionTable(uint8_t x)
{
	x &= 0b11; // low of byte
	switch (x)
	{
	case 0b00:
		return !GetFlag(FlagZ);
	case 0b01:
		return  GetFlag(FlagZ);
	case 0b10:
		return !GetFlag(FlagC);
	case 0b11:
		return  GetFlag(FlagC);
	default:
		return false;
	}
}

// returns cycles per increment for mode x
// return value of 0 means timer is disabled
uint16_t CPU::TimerTable(uint8_t x)
{
	if (!(x & 0b100))
		return 0;
	switch (x & 0b11)
	{
	case 0b00:
		return 1024;
	case 0b01:
		return 16;
	case 0b10:
		return 64;
	case 0b11:
		return 256;
	default:
		return 1;
	}
}

void CPU::R8TableRef(uint8_t x, uint8_t val)
{
	x &= 0b00000111;
	switch (x)
	{
	case 0b000:
		regB = val;
		break;
	case 0b001:
		regC = val;
		break;
	case 0b010:
		regD = val;
		break;
	case 0b011:
		regE = val;
		break;
	case 0b100:
		regH = val;
		break;
	case 0b101:
		regL = val;
		break;
	case 0b110: // HL points to address that contains data
		bus->cpuWrite(BytesToWord(regH, regL), val);
		break;
	case 0b111:
		regA = val;
		break;
	default:
		break;
	}
}
void CPU::R16Table1Ref(uint8_t x, uint16_t val)
{
	x &= 0b00000011;
	switch (x)
	{
	case 0b00:
		regB = val >> 8;
		regC = val & 0x00FF; // low of byte
		break;
	case 0b01:
		regD = val >> 8;
		regE = val & 0x00FF;
		break;
	case 0b10:
		regH = val >> 8;
		regL = val & 0x00FF;
		break;
	case 0b11:
		SP = val;
		break;
	default:
		break;
	}
}
void CPU::R16Table2Ref(uint8_t x, uint16_t val)
{
	x &= 0b00000011;
	switch (x)
	{
	case 0b00:
		regB = val >> 8;
		regC = val & 0x00FF; // low of byte
		break;
	case 0b01:
		regD = val >> 8;
		regE = val & 0x00FF;
		break;
	case 0b10:
		val++;
		regH = val >> 8;
		regL = val & 0x00FF; // low of byte
		break;
	case 0b11:
		val--;
		regH = val >> 8;
		regL = val & 0x00FF; // low of byte
		break;
	default:
		break;
	}
}
void CPU::R16Table3Ref(uint8_t x, uint16_t val)
{
	x &= 0b00000011;
	switch (x)
	{
	case 0b00:
		regB = val >> 8;
		regC = val & 0x00FF; // low of byte
		break;
	case 0b01:
		regD = val >> 8;
		regE = val & 0x00FF;
		break;
	case 0b10:
		regH = val >> 8;
		regL = val & 0x00FF;
		break;
	case 0b11:
		regA = val >> 8;
		regF = val & 0x00F0; // remove the lower nibble because only the top is used
		break;
	default:
		break;
	}
}

bool CPU::GetFlag(Flags flag)
{
	return regF & flag;
}
void CPU::SetFlag(Flags flag, bool value)
{
	if (value)
		regF |=  flag;
	else
		regF &= ~flag;
}

void CPU::RequestInterrupt(int bit)
{
	bit = clamp(bit, 0, 4);
	WriteOr(IF, Bit(bit));
}

uint8_t CPU::PendingInterrupts()
{
	return ReadByte(IE) & ReadByte(IF);
}

uint8_t CPU::ReadByte()
{
	return ReadByte(PC++);
}
uint8_t CPU::ReadByte(uint16_t address)
{
	return bus->cpuRead(address);
}
uint16_t CPU::ReadWord()
{
	uint16_t result = ReadWord(PC);
	PC += 2;
	return result;
}
uint16_t CPU::ReadWord(uint16_t address)
{
	uint8_t lo = ReadByte(address++);
	uint8_t hi = ReadByte(address++);
	uint16_t result = BytesToWord(hi, lo);
	return result;
}

void CPU::Write(uint8_t data)
{
	Write(PC, data);
	PC++;
}
void CPU::Write(uint16_t address, uint8_t data)
{
	if (address == TAC && data != ReadByte(TAC))
		timerCounter = ReadByte(TMA);
	bus->cpuWrite(address, data);
}
void CPU::Write(uint16_t data)
{
	Write(PC, data);
	PC += 2;
}
void CPU::Write(uint16_t address, uint16_t data)
{
	uint8_t lo = data & 0x00FF;
	uint8_t hi = data >> 8;
	bus->cpuWrite(address++, lo);
	bus->cpuWrite(address++, hi);
}

void CPU::WriteOffset(uint16_t address, int offset)
{
	Write(address, (uint8_t)(this->ReadByte(address) + offset));
}

void CPU::WriteAnd(uint16_t address, uint8_t andData)
{
	Write(address, (uint8_t)(ReadByte(address) & andData));
}

void CPU::WriteOr(uint16_t address, uint8_t orData)
{
	Write(address, (uint8_t)(ReadByte(address) | orData));
}

void CPU::WriteXor(uint16_t address, uint8_t xorData)
{
	Write(address, (uint8_t)(ReadByte(address) ^ xorData));
}

void CPU::PushToStack(uint8_t value)
{
	Write(--SP, value);
}
void CPU::PushToStack(uint16_t value)
{
	Write(--SP, (uint8_t)(value >> 8));
	Write(--SP, (uint8_t)(value & 0x00FF));
}
uint16_t CPU::PopFromStack(bool twoValues)
{
	if (twoValues)
	{
		/*uint8_t lo = bus->cpuRead(SP++);
		uint8_t hi = bus->cpuRead(SP++);*/
		uint16_t result = ReadWord(SP);
		SP += 2;
		return result;
	}
	else
		return ReadByte(SP++);
}

void CPU::ALU(uint8_t y, uint8_t value)
{
	uint8_t prevCarry = GetFlag(FlagC);
	switch (y)
	{
	case 0b000: // ADD A, u8
		SetFlag(FlagH, (((regA & 0x0F) + (value & 0x0F)) & 0x10) == 0x10);
		SetFlag(FlagC, (uint16_t)regA + (uint16_t)value > 0xFF);
		regA += value;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 0);
		break;
	case 0b001: // ADC A, u8
		SetFlag(FlagH, ((((regA) & 0x0F) + (value & 0x0F) + prevCarry) & 0x10) == 0x10);
		SetFlag(FlagC, (uint16_t)regA + (uint16_t)value + (uint16_t)prevCarry > 0xFF);
		regA += value + prevCarry;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 0);
		break;
	case 0b010: // SUB A, u8
		SetFlag(FlagH, (regA & 0x0F) < (value & 0x0F));
		SetFlag(FlagC, regA < value);
		regA -= value;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 1);
		break;
	case 0b011: // SBC A, u8
		SetFlag(FlagH, (regA & 0x0F) < ((value & 0x0F) + prevCarry));
		SetFlag(FlagC, regA < value + prevCarry);
		regA -= value + prevCarry;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 1);
		break;
	case 0b100: // AND A, u8
		regA &= value;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 0);
		SetFlag(FlagH, 1);
		SetFlag(FlagC, 0);
		break;
	case 0b101: // XOR A, u8
		regA ^= value;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 0);
		SetFlag(FlagH, 0);
		SetFlag(FlagC, 0);
		break;
	case 0b110: // OR A, u8
		regA |= value;
		SetFlag(FlagZ, regA == 0);
		SetFlag(FlagN, 0);
		SetFlag(FlagH, 0);
		SetFlag(FlagC, 0);
		break;
	case 0b111: // CP A, u8
		SetFlag(FlagH, (regA & 0x0F) < (value & 0x0F));
		SetFlag(FlagC, regA < value);
		SetFlag(FlagZ, regA - value == 0);
		SetFlag(FlagN, 1);
		break;
	default:
		break;
	}
}

void CPU::CB()
{
	opcode = ReadByte(); // CB is a prefix for the actual opcode, so read the actual opcode now
	uint8_t x = Bits(opcode, 6, 2);
		uint8_t y = Bits(opcode, 3, 3);
		uint8_t p = Bits(y, 1, 2);
		uint8_t q = Bit(y, 0);
	uint8_t z = Bits(opcode, 0, 3);

	uint8_t prevValue = R8TableVal(z);
	switch (x)
	{
	case 0b00:
		uint8_t carry;
		switch (y)
		{
		case 0b000: // RLC r8
			carry = Bit(prevValue, 7); // msb
			R8TableRef(z, (prevValue << 1) | carry); // "rotate" to the left

			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			SetFlag(FlagZ, R8TableVal(z) == 0);
			break;
		case 0b001: // RRC r8
			carry = Bit(prevValue, 0); // lsb
			R8TableRef(z, (prevValue >> 1) | (carry << 7)); // "rotate" to the right

			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			SetFlag(FlagZ, R8TableVal(z) == 0);
			break;
		case 0b010: // RL r8
			carry = Bit(prevValue, 7); // msb
			R8TableRef(z, (prevValue << 1) | GetFlag(FlagC));

			SetFlag(FlagZ, R8TableVal(z) == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			break;
		case 0b011: // RR r8
			carry = Bit(prevValue, 0); // lsb
			R8TableRef(z, (prevValue >> 1) | (GetFlag(FlagC) << 7));

			SetFlag(FlagZ, R8TableVal(z) == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			break;
		case 0b100: // SLA r8
			carry = prevValue & Bit(7); // msb
			R8TableRef(z, prevValue << 1);

			SetFlag(FlagZ, R8TableVal(z) == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			break;
		case 0b101: // SRA r8
			carry = Bit(prevValue, 0); // lsb
			R8TableRef(z, (prevValue >> 1) | (prevValue & Bit(7)));

			SetFlag(FlagZ, R8TableVal(z) == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			break;
		case 0b110: // SWAP r8
		{
			uint8_t lo = prevValue & 0x0F;
			uint8_t hi = prevValue >> 4;

			R8TableRef(z, (lo << 4) | hi);
			SetFlag(FlagZ, prevValue == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, 0);
			break;
		}
		case 0b111: // SRL r8
			carry = Bit(prevValue, 0); // lsb
			R8TableRef(z, prevValue >> 1);

			SetFlag(FlagZ, R8TableVal(z) == 0);
			SetFlag(FlagN, 0);
			SetFlag(FlagH, 0);
			SetFlag(FlagC, carry);
			break;
		default:
			break;
		}
		if (z == 0b110)
			cyclesLeft = 16;
		else
			cyclesLeft = 8;
		break;
	case 0b01: // BIT u3, r8
		SetFlag(FlagZ, Bit(R8TableVal(z), y) == 0);
		SetFlag(FlagN, 0);
		SetFlag(FlagH, 1);
		if (z == 0b110)
			cyclesLeft = 12;
		else
			cyclesLeft = 8;
		break;
	case 0b10: // RES u3, r8
		R8TableRef(z, prevValue & ~Bit(y));
		if (z == 0b110)
			cyclesLeft = 12;
		else
			cyclesLeft = 8;
		break;
	case 0b11: // SET u3, r8
		R8TableRef(z, prevValue | Bit(y));
		if (z == 0b110)
			cyclesLeft = 16;
		else
			cyclesLeft = 8;
		break;
	default:
		break;
	}
}
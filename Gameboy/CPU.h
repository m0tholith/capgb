#include <string>
#include <cstdint>
#include <list>
#include <array>
#ifndef CPU_H
#define CPU_H
class Bus;

class CPU
{
public:
	void reset();
	// registers (https://gbdev.io/pandocs/CPU_Registers_and_Flags.html#registers)
	uint8_t regA, regB, regC, regD, regE, regF, regH, regL; // 8-bit
	//      ||||                          ||||
	// accumulator                        ||||
	// flags <----------------------------++++

	uint16_t   SP, PC; // 16-bit
	//         ||  ||
	// stack <-++  ||
	// pointer    \||/
	//             \/
	// program counter
	uint8_t opcode;

	Bus* bus;

	// flags (https://gbdev.io/pandocs/CPU_Registers_and_Flags.html#the-flags-register-lower-8-bits-of-af-register)
	enum Flags
	{
		FlagZ = 1 << 7, // zero (set if result of operation is 0)
		FlagN = 1 << 6, // subtraction (set if last operation was subtraction)
		FlagH = 1 << 5, // half carry (set if carry lower 4 bits of last add/sub result was needed)
		FlagC = 1 << 4, // carry (set if carry for last add/sub result was needed)
	};

	void log();
	void cycle();
	void handleInterrupts();
	intmax_t cyclesLeft = 4;
	int totalCycles;

	// value tables
	uint8_t R8TableVal(uint8_t x);
	uint16_t R16Table1Val(uint8_t x);
	uint16_t R16Table2Val(uint8_t x);
	uint16_t R16Table3Val(uint8_t x);
	bool ConditionTable(uint8_t x);
	uint16_t TimerTable(uint8_t x);

	// reference tables (how to use: call table function and pass value)
	void R8TableRef(uint8_t x, uint8_t val);
	void R16Table1Ref(uint8_t x, uint16_t val);
	void R16Table2Ref(uint8_t x, uint16_t val);
	void R16Table3Ref(uint8_t x, uint16_t val);

	bool GetFlag(Flags flag);
	void SetFlag(Flags flag, bool value);

	uint8_t PendingInterrupts();
	void RequestInterrupt(int bit);

	uint8_t ReadByte();
	uint8_t ReadByte(uint16_t address);
	uint16_t ReadWord();
	uint16_t ReadWord(uint16_t address);
	void Write(uint8_t data);
	void Write(uint16_t address, uint8_t data);
	void Write(uint16_t data);
	void Write(uint16_t address, uint16_t data);

	void WriteOffset(uint16_t address, int offset = 1); // adds the offset to the byte in address
	void WriteAnd(uint16_t address, uint8_t andData); // ANDs the byte with data
	void WriteOr (uint16_t address, uint8_t orData);  //  ORs the byte with data
	void WriteXor(uint16_t address, uint8_t xorData); // XORs the byte with data

	void PushToStack(uint8_t value);
	void PushToStack(uint16_t value);
	uint16_t PopFromStack(bool twoValues = false);

	void ALU(uint8_t y, uint8_t value); // arithmetic
	void CB(); // moves to CB opcode table

	const static std::array<std::string, 0xFF + 1> opcodes;

	/*
	* interrupt enable lets the cpu know if it wants to check for interrupts
	* interrupt flags lets the cpu know if there was an interrupt
	* https://gbdev.io/pandocs/Interrupts.html#interrupts
	*/
	bool IME = 0; // enable/disable all interrupts
	const uint16_t IF = 0xFF0F; // interrupt flags address
	const uint16_t IE = 0xFFFF; // interrupt enable address

	/*
	* http://www.codeslinger.co.uk/pages/projects/gameboy/timers.html
	*/
	const uint16_t DIV  = 0xFF04; // constant timer address
	const uint16_t TIMA = 0xFF05; // timer address
	const uint16_t TMA  = 0xFF06; // timer reset value address
	const uint16_t TAC  = 0xFF07; // timer length address
	int timerCounter, constantTimerCounter;
};	
#endif // !CPU_H
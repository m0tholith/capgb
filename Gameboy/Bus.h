#include <cstdint>
#include <array>
#ifndef BUS_H
#define BUS_H
class CPU;

class Bus
{
public:
	CPU* cpu;

	uint8_t cpuRead(uint16_t address);
	void cpuWrite(uint16_t address, uint8_t data);

	void reset();

	// https://gbdev.io/pandocs/Memory_Map.html -> memory map
	std::array<uint8_t, 0x10000> memory;
};
#endif // !BUS_H
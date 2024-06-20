#include "Bus.h"
#include "CPU.h"

uint8_t Bus::cpuRead(uint16_t address)
{
    // no need to check if the address is in range (for now) because all addresses are accessible
    if (address == 0xFF44)
        return 0x90;
    return memory[address];
}

void Bus::cpuWrite(uint16_t address, uint8_t data)
{
    memory[address] = data;
}

void Bus::reset()
{
    for (int i = 0; i < memory.size(); i++)
        memory[i] = 0;
}

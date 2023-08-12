#include "Gameboy/Renderer/Renderer.h"
#include "Gameboy/Bus.h"
#include "Gameboy/CPU.h"
#include "Gameboy/HELPER.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <cstdio>
#undef main

Renderer renderer = Renderer();
Bus bus = Bus();
CPU cpu = CPU();
int x = 0, y = 0;
// time passed in system ticks
uint64_t frameStart;
uint64_t frameEnd;

std::string logs;

std::vector<uint8_t> readFile(const char* filename)
{
	// open the file:
	std::ifstream file(filename, std::ios::binary);

	// Stop eating new lines in binary mode!!!
	file.unsetf(std::ios::skipws);

	// get its size:
	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// reserve capacity
	std::vector<uint8_t> vec;
	vec.reserve(fileSize);

	// read the data:
	vec.insert(vec.begin(),
		std::istream_iterator<uint8_t>(file),
		std::istream_iterator<uint8_t>());

	return vec;
}

bool cycle()
{
	//renderer.ChangeColor(x, y, (x + renderer.totalTicks / 100000) % 256, y % 240, 0, 255);
	//x++;
	//if (x % 4)
		cpu.cycle();
	//if (x >= 256)
	//{
		//x = 0;
		//y++;
		//if (y >= 240)
		//{
			//y = 0;
			//frameEnd = SDL_GetPerformanceCounter();
			//float elapsedMS = (frameEnd - frameStart) / (float)SDL_GetPerformanceFrequency() * 1000;
			//if (elapsedMS < 1000 / 59.927)
				//SDL_Delay((int)floor(1000 / 59.927 - elapsedMS) * 2);

			//frameStart = frameEnd;
			//renderer.Render();
		//}
	//}

	// checking game link port for any writes (test rom)
		if (bus.memory[0xFF02] == 0x81)
		{
			printf("%c", bus.memory[0xFF01]);
			bus.memory[0xFF02] = 0;
			logs.push_back(bus.memory[0xFF01]);
		}
		return true;
}
std::vector<const char*> romFiles =
{
	"01-special.gb",
	"02-interrupts.gb",
	"03-op sp,hl.gb",
	"04-op r,imm.gb",
	"05-op rp.gb",
	"06-ld r,r.gb",
	"07-jr,jp,call,ret,rst.gb",
	"08-misc instrs.gb",
	"09-op r,r.gb",
	"10-bit ops.gb",
	"11-op a,(hl).gb",
	"instr_timing.gb",
	"cpu_instrs.gb",
};
bool loadRom(int i = 0)
{
	if (i >= romFiles.size())
		return false;
	std::vector<uint8_t> rom = readFile(std::string("Roms\\").append(romFiles[i]).c_str());
	for (int i = 0x0000; i < rom.size(); i++)
		bus.memory[i] = rom[i];
	return true;
}
int main()
{
	//renderer.Start();
	bus.cpu = &cpu;
	cpu.bus = &bus;

	cpu.reset();
	bus.reset();

	int i = 12;
	loadRom(i++);

	// delete log files
	std::remove("log.txt");
	std::remove("results.txt");

	//frameStart = SDL_GetPerformanceCounter();
	while (renderer.running)
	{
		if (!cycle())
		{
			//cpu.reset();
			//bus.reset();
			//system("cls");
			//if (!loadRom(i++))
			//	break;
			break;
		}
	}
	//SDL_Delay(3000);
	//renderer.Shutdown(&renderer.pWindow, &renderer.pRenderer, &renderer.pTexture);
	return 0;
}
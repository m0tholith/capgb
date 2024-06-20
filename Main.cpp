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
uint8_t x = 0, y = 0;
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
uint32_t colors[4] =
{
	// gameboy
	Renderer::ARGB(51, 44, 80, 255),
	Renderer::ARGB(70, 135, 143, 255),
	Renderer::ARGB(148, 227, 68, 255),
	Renderer::ARGB(226, 243, 228, 255),
	// greyscale
	//Renderer::ARGB(0, 0, 0, 255),
	//Renderer::ARGB(85, 85, 85, 255),
	//Renderer::ARGB(170, 170, 170, 255),
	//Renderer::ARGB(255, 255, 255, 255),
};

void renderVRAM()
{
	uint16_t address = 0x8000;
	uint8_t x = 0, y = 0; // position of tiles, not pixels
	for (uint8_t i = 0; i < 36; i++) // render deez tiles
	{
		for (uint8_t j = 0; j < 8; j++) // render deez rows
		{
			uint8_t row1 = bus.cpuRead(address + i * 16 + j * 2 + 0);
			uint8_t row2 = bus.cpuRead(address + i * 16 + j * 2 + 1);
			for (uint8_t k = 0; k < 8; k++) // render deez pixels
			{
				bool pixel1 = (row1 & HELPER::Bit(7 - k)) > 0;
				bool pixel2 = (row2 & HELPER::Bit(7 - k)) > 0;
				renderer.ChangeColor(x * 8 + k, y * 8 + j, colors[((int)pixel2 << 1) | (int)pixel1]);
			}
		}

		x++;
		if (x >= 4)
		{
			x = 0;
			y++;
		}
	}
}

bool cycle()
{
	renderer.ChangeColor(160 - 1, 144 - 1, colors[3], 1);
	renderer.ChangeColor(160 - 2, 144 - 1, colors[2], 1);
	renderer.ChangeColor(160 - 3, 144 - 1, colors[1], 1);
	renderer.ChangeColor(160 - 4, 144 - 1, colors[0], 1);
	// rendering blocks 0-2 in VRAM
	//if (HELPER::range(x, 0, 8, 0b10))
	//{
	//	uint16_t address = 0x8000 + y * 2 + 16 * floor(y / 8.0f);
	//	bool pixel1 = (bus.cpuRead(address + 0) & HELPER::Bit(7 - x)) > 0;
	//	bool pixel2 = (bus.cpuRead(address + 1) & HELPER::Bit(7 - x)) > 0;
	//	renderer.ChangeColor(x, y, colors[((int)pixel2 << 1) | (int)pixel1]);
	//}
	//if (HELPER::range(x, 8, 16, 0b10))
	//{
	//	uint16_t address = 0x8010 + y * 2 + 16 * floor(y / 8.0f);
	//	bool pixel1 = (bus.cpuRead(address + 0) & HELPER::Bit(7 - x)) > 0;
	//	bool pixel2 = (bus.cpuRead(address + 1) & HELPER::Bit(7 - x)) > 0;
	//	renderer.ChangeColor(x, y, colors[((int)pixel2 << 1) | (int)pixel1]);
	//}
	//if (HELPER::range(x, 16, 24, 0b10))
	//{
	//	uint16_t address = 0x8020 + y * 2 + 16 * floor(y / 8.0f);
	//	bool pixel1 = (bus.cpuRead(address + 0) & HELPER::Bit(7 - x)) > 0;
	//	bool pixel2 = (bus.cpuRead(address + 1) & HELPER::Bit(7 - x)) > 0;
	//	renderer.ChangeColor(x, y, colors[((int)pixel2 << 1) | (int)pixel1]);
	//}
	//if (HELPER::range(x, 24, 32, 0b10))
	//{
	//	uint16_t address = 0x8030 + y * 2 + 16 * floor(y / 8.0f);
	//	bool pixel1 = (bus.cpuRead(address + 0) & HELPER::Bit(7 - x)) > 0;
	//	bool pixel2 = (bus.cpuRead(address + 1) & HELPER::Bit(7 - x)) > 0;
	//	renderer.ChangeColor(x, y, colors[((int)pixel2 << 1) | (int)pixel1]);
	//}

	cpu.cycle();
	x++;
	if (x >= 160)
	{
		x = 0;
		y++;
		if (y >= 144)
		{
			y = 0;
			renderVRAM();

			frameEnd = SDL_GetPerformanceCounter();
			float elapsedMS = (frameEnd - frameStart) / (float)SDL_GetPerformanceFrequency() * 1000;
			if (elapsedMS < 1000 / 59.927)
				SDL_Delay((int)floor(1000 / 59.927 - elapsedMS) * 2);

			frameStart = frameEnd;
			renderer.Render();
		}
	}

	// checking game link port for any writes (test rom)
		//if (bus.memory[0xFF02] == 0x81)
		//{
		//	printf("%c", bus.memory[0xFF01]);
		//	bus.memory[0xFF02] = 0;
		//	logs.push_back(bus.memory[0xFF01]);
		//}
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
	"Super Mario Land (JUE) (V1.1) [!].gb",
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
	renderer.Start();
	bus.cpu = &cpu;
	cpu.bus = &bus;

	cpu.reset();
	bus.reset();

	int i = 13;
	loadRom(i++);

	// delete log files
	std::remove("log.txt");
	std::remove("results.txt");

	frameStart = SDL_GetPerformanceCounter();
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
	renderer.Shutdown(&renderer.pWindow, &renderer.pRenderer, &renderer.pTexture);
	return 0;
}
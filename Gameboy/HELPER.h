#ifndef HELPER_H
#define HELPER_H
#include <cstdint>
#include <fstream>
#include <exception>

namespace HELPER
{
	static bool range(size_t x, size_t a, size_t b, uint8_t control = 0b11) // what side of range is included in the range
	{
		control &= 0b11;
		switch (control)
		{
		case 0b00:
			return   a < x && x < b;
		case 0b01:
			return  a < x && x <= b;
		case 0b10:
			return  a <= x && x < b;
		case 0b11:
			return a <= x && x <= b;
		default: // impossible but clears errors
			return false;
		}
	}

	static size_t clamp(size_t x, size_t a, size_t b, uint8_t control = 0b11)
	{
		control &= 0b11;
		switch (control)
		{
		case 0b00:
			if (x < a)
				x = a;
			else if (x > b)
				x = b;
		case 0b01:
			if (x < a)
				x = a;
			else if (x >= b)
				x = b;
		case 0b10:
			if (x <= a)
				x = a;
			else if (x > b)
				x = b;
		case 0b11:
			if (x <= a)
				x = a;
			else if (x >= b)
				x = b;
		default: // impossible but clears errors
			return x;
		}
		return x;
	}

	static std::string fileContents;
	static uint64_t count;
	//static uint64_t saves;
	static void WriteLog()
	{
		std::ofstream ofs("log.txt", std::ios_base::out | std::ios_base::app);
		ofs << HELPER::fileContents;
		ofs.close();
		fileContents = std::string();
		count = 0;
		//saves++;
		//if (saves == 41)
		//	throw std::exception();
	}
	static void Log(std::string log)
	{
		fileContents.append(log);
		count++;
		if (count >= 100000)
			HELPER::WriteLog();
	}

	static int Bit(int k) // shift 1 by k
	{
		return 1 << k;
	}
	static bool Bit(int x, int k) // get the k'th bit of x
	{
		return x & Bit(k);
	}
	static int Bits(uintmax_t x, int k, int bitCount) // get the bits of x from k to k + bitCount
	{
		uintmax_t result = 0;
		for (int i = 0; i < bitCount; i++)
			result |= x & Bit(k + i);
		return result >> k;
	}
	static int BitToggle(uintmax_t x, int k)
	{
		return x & ~Bit(k);
	}

	static uint16_t BytesToWord(uint8_t hi, uint8_t lo)
	{
		return (((uint16_t)hi) << 8) | lo;
	}
};

#endif // !HELPER_H
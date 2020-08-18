#include "h264.h"

#include <iostream>
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
	std::cout << "h264 analysis" << std::endl;

	std::cout << "Usage : " << "h264 h264file." << std::endl;

	if (argc < 2)
	{
		std::cerr << "please see the usage message." << std::endl;
		return -1;
	}
	std::ifstream in(argv[1], std::ios::binary);
	if (in.fail())
	{
		std::cerr << "can not open file " << argv[1] << std::endl;
		return -1;
	}

	unsigned char c = 0;
	int32_t datalen = 0;
	int step = 0;//记录0x00的个数
	while (in.read(reinterpret_cast<char*>(&c), 1))
	{
		if (c == 0)
		{
			++step;
		}
		else if (c == 1 && step >= 2)
		{
			auto naluflagsize = step > 2 ? 3 : 4;
			datalen += (step > 3 ? step - 3 : 0);
			if (datalen != 0)
			{
				std::cout << "nalu size : " << datalen-1 << std::endl;
				in.seekg(-naluflagsize - datalen, std::ios::cur);

				if (!in.read(reinterpret_cast<char*>(&c), 1))
					break;
				std::cout << std::string(50, '*').c_str() << std::endl;
				NALHEADER* pnalheader = reinterpret_cast<NALHEADER*>(&c);
				std::cout << *pnalheader << std::endl;

				in.seekg(-1, std::ios::cur);

				std::ios::fmtflags f(std::cout.flags());
				for (int i = 0; i < datalen; ++i)
				{
					in.read(reinterpret_cast<char*>(&c), 1);
					std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(c) << " ";
				}
				std::cout << std::endl;
				std::cout.flags(f);
				in.seekg(naluflagsize, std::ios::cur);
			}

			datalen = 1;
			step = 0;
		}
		else
		{
			datalen += step + 1;
			step = 0;
		}
	}

	in.close();
	return 0;
}

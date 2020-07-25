#include <iostream>
#include <fstream>

#include "h264.h"

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

	NALHEADER nalheader = { 0 };
	unsigned char c = 0;
	uint32_t datalen = 0;
	int step = 0;//记录0x00的个数
	while (in.read(reinterpret_cast<char*>(&c), 1))
	{
		if (c == 0)
		{
			++step;
		}
		else if (c == 1 && step >= 2)
		{
			datalen += (step > 3 ? step - 3 : 0);
			if (datalen != 0)
			{
				std::cout << "nal size " << datalen << std::endl;
			}

			in.read(reinterpret_cast<char*>(&c), 1);
			memcpy(&nalheader, &c, 1);
			std::cout << std::string(50, '*') << std::endl;
			std::cout << nalheader << std::endl;

			datalen = 1;
			step = 0;
		}
		else
		{
			datalen += step + 1;
			step = 0;
		}
	}

	datalen += step;
	if (datalen != 0)
	{
		std::cout << "nal size " << datalen << std::endl;
	}

	in.close();
	return 0;
}

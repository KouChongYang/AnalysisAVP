/*
 * @Author: gongluck
 * @Date: 2020-08-24 20:40:16
 * @Last Modified by:   gongluck
 * @Last Modified time: 2020-08-24 20:40:16
 */

#include "aac.h"

#include <iostream>
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
	std::cout << "aac analysis" << std::endl;

	std::cout << "Usage : " << "aac aacfile." << std::endl;

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

	ADTS adts = { 0 };
	uint16_t framelength = 0;
	while (in.read(reinterpret_cast<char*>(&adts), sizeof(adts)))
	{
		framelength = get_aac_frame_length(adts);
		std::cout << framelength << std::endl;
		in.seekg(framelength - sizeof(adts), std::ios::cur);
	}

	in.close();
	return 0;
}

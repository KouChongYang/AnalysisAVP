/*
 * @Author: gongluck
 * @Date: 2020-10-03 15:36:42
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-10-03 15:37:43
 */

#include <iostream>
#include <fstream>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")
#endif

#include "../../../analysis/flv/flv.h"

extern "C"
{
#include "rtmp.h"
}

const int presentime = 5000;

int main(int argc, char* argv[])
{
	std::cout << "librtmp example" << std::endl;

	std::cout << "Usage : " << "thisfile flvfile pushurl." << std::endl;

	if (argc < 3)
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

#ifdef _WIN32
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(1, 1);
	WSAStartup(version, &wsaData);
#endif

	RTMP* rtmp = RTMP_Alloc();
	RTMP_Init(rtmp);
	auto rtmpres = RTMP_SetupURL(rtmp, argv[2]);
	RTMP_EnableWrite(rtmp);//推流要设置写
	rtmpres = RTMP_Connect(rtmp, nullptr);
	rtmpres = RTMP_ConnectStream(rtmp, 0);

	FLVHEADER flvheader = { 0 };
	in.read(reinterpret_cast<char*>(&flvheader), sizeof(flvheader));
	std::cout << flvheader << std::endl;

	FLVINT32 presize = { 0 };
	in.read(reinterpret_cast<char*>(&presize), 4);

	auto begintime = RTMP_GetTime();
	uint32_t timestamp = 0, now = 0;
	while (true)
	{
		FLVTAGHEADER tagheader = { 0 };
		if (!in.read(reinterpret_cast<char*>(&tagheader), sizeof(tagheader)))
			break;

		auto datalen = FLVINT24TOINT(tagheader.datalen);
		auto data = new char[sizeof(FLVTAGHEADER) + datalen + 4];
		memcpy(data, &tagheader, sizeof(FLVTAGHEADER));
		if (!in.read(data + sizeof(FLVTAGHEADER), datalen + 4))
			break;

		timestamp = FLVINT32TOINT(tagheader.timestamp);
	CALCTIME:
		now = RTMP_GetTime() - begintime;
		if (now < timestamp + presentime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((timestamp + presentime - now) / 2));
			goto CALCTIME;
		}
		rtmpres = RTMP_Write(rtmp, data, sizeof(FLVTAGHEADER) + datalen + 4);//tagheader + data + presize
		if (rtmpres < sizeof(FLVTAGHEADER) + datalen + 4)
			break;
		//std::cout << "timestamp " << timestamp << "ms" << std::endl;
		delete[]data;
	}

	RTMP_Close(rtmp);
	RTMP_Free(rtmp);
	in.close();

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}

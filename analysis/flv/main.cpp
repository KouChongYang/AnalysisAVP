#include "flv.h"

#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
	std::cout << "flv analysis" << std::endl;

	std::cout << "Usage : " << "flv flvfile." << std::endl;

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

	FLVHEADER flvheader = { 0 };
	in.read(reinterpret_cast<char*>(&flvheader), sizeof(flvheader));
	std::cout << flvheader << std::endl;

	// 保存264数据
	std::ofstream out("out.264", std::ios::binary);
	char nalutag[] = { 0x00, 0x00, 0x00, 0x01 };

	while (true)
	{
		GINT32 presize = { 0 };
		if (!in.read(reinterpret_cast<char*>(&presize), 4))
			break;
		std::cout << "presize : " << FINT32TOINT(presize) << std::endl;

		std::cout << std::string(50, '*').c_str() << std::endl;
		TAGHEADER tagheader = { 0 };
		if (!in.read(reinterpret_cast<char*>(&tagheader), sizeof(tagheader)))
			break;
		std::cout << tagheader << std::endl;

		auto datalen = FINT24TOINT(tagheader.datalen);
		auto data = new char[datalen];
		if (!in.read(data, datalen))
			break;

		if (tagheader.type == FLV_TAG_TYPE_VIDIO)
		{
			std::cout << std::string(50, '-').c_str() << std::endl;
			VIDEOTAG* pvideotag = reinterpret_cast<VIDEOTAG*>(data);
			std::cout << *pvideotag << std::endl;
			std::cout << std::string(50, '-').c_str() << std::endl;
			if (pvideotag->videopacket.avcvideopacket.type == AVC_PACKET_HEADER)
			{
				AVCDecoderConfigurationRecordHeader* configheader =
					reinterpret_cast<AVCDecoderConfigurationRecordHeader*>(pvideotag->videopacket.avcvideopacket.data);
				std::cout << *configheader << std::endl;

				SequenceParameterSet* sps =
					reinterpret_cast<SequenceParameterSet*>(configheader->data);
				int datasize = FINT16TOINT((sps->sequenceParameterSetLength));
				out.write(nalutag, 4);
				out.write(reinterpret_cast<char*>(sps->sequenceParameterSetNALUnit), datasize);
				std::cout << *sps << std::endl;

				PictureParameterSet* pps =
					reinterpret_cast<PictureParameterSet*>(sps->sequenceParameterSetNALUnit + FINT16TOINT(sps->sequenceParameterSetLength));
				datasize = FINT16TOINT((pps->pictureParameterSetLength));
				out.write(nalutag, 4);
				out.write(reinterpret_cast<char*>(pps->pictureParameterSetNALUnit), datasize);
				std::cout << *pps << std::endl;
			}
			else if (pvideotag->videopacket.avcvideopacket.type == AVC_PACKET_NALU)
			{
				std::cout << "nalu : ";
				std::ios::fmtflags f(std::cout.flags());
				for (int i = 0; i < FINT24TOINT(tagheader.datalen) - offsetof(VIDEOTAG, videopacket.avcvideopacket.data); ++i)
				{
					std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(pvideotag->videopacket.avcvideopacket.data[i]) << " ";
				}
				std::cout.flags(f);

				GINT32* nalsize = reinterpret_cast<GINT32*>(&pvideotag->videopacket.avcvideopacket.data[0]);
				int datasize = FINT32TOINT((*nalsize));
				out.write(nalutag, 4);
				out.write(reinterpret_cast<char*>(&pvideotag->videopacket.avcvideopacket.data[0 + 4]), datasize);
			}
			else if (pvideotag->videopacket.avcvideopacket.type == AVC_PACKET_END)
			{
				out.close();
				std::cout << "AVC end of sequence" << std::endl;
			}
		}

		delete[]data;
	}

	in.close();
	return 0;
}

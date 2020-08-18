#include "flv.h"

#include <iomanip>

const char* flv_tag_parse_type(uint8_t type)
{
	switch (type)
	{
	case FLV_TAG_TYPE_AUDIO:
		return "audio tag";
	case FLV_TAG_TYPE_VIDIO:
		return "video tag";
	case FLV_TAG_TYPE_SCRIPT:
		return "script tag";
	default:
		return "UNKNOWN";
	}
}

const char* flv_video_parse_type(uint8_t type)
{
	switch (type)
	{
	case FLV_VIDEO_FRAME_KEYFRAME:
		return "keyframe(for AVC, a seekable frame) —— 即H.264的IDR帧";
	case FLV_VIDEO_FRAME_IFRAME:
		return "inter frame(for AVC, a non - seekable frame) —— H.264的普通I帧";
	case FLV_VIDEO_FRAME_DISPOSABLE:
		return "disposable inter frame(H.263 only)";
	case FLV_VIDEO_FRAME_GENERATED:
		return "generated keyframe(reserved for server use only)";
	case FLV_VIDEO_FRAME_COMMAND:
		return "video info / command frame";
	default:
		return "UNKNOWN";
	}
}

const char* flv_video_parse_codecid(uint8_t codecid)
{
	switch (codecid)
	{
	case FLV_VIDEO_CODECID_JPEG:
		return "JPEG (currently unused)";
	case FLV_VIDEO_CODECID_H263:
		return "Sorenson H.263";
	case FLV_VIDEO_CODECID_SCREEN:
		return "Screen video";
	case FLV_VIDEO_CODECID_ON2VP6:
		return "On2 VP6";
	case FLV_VIDEO_CODECID_ON2VP6A:
		return "On2 VP6 with alpha channel";
	case FLV_VIDEO_CODECID_SCREEN2:
		return "Screen video version 2";
	case FLV_VIDEO_CODECID_AVC:
		return "AVC";
	default:
		return "UNKNOWN";
	}
}

const char* avc_packet_parse_type(uint8_t type)
{
	switch (type)
	{
	case AVC_PACKET_HEADER:
		return "AVC sequence header";
	case AVC_PACKET_NALU:
		return "AVC NALU";
	case AVC_PACKET_END:
		return "AVC end of sequence";
	default:
		return "UNKNOWN";
	}
}

std::ostream& operator<<(std::ostream& os, const FLVHEADER& flvheader)
{
	os << "f : " << static_cast<char>(flvheader.f)
		<< "\nl : " << static_cast<char>(flvheader.l)
		<< "\nv : " << static_cast<char>(flvheader.v)
		<< "\ntype : " << static_cast<int>(flvheader.type)
		<< "\nvideo : " << static_cast<int>(flvheader.video)
		<< "\naudio : " << static_cast<int>(flvheader.audio);
	return os;
}

std::ostream& operator<<(std::ostream& os, const VIDEOTAG& videotag)
{
	os << "type : " << flv_video_parse_type(videotag.type)
		<< "\ncodecid : " << flv_video_parse_codecid(videotag.codecid)
		<< "\navcpacket type : " << avc_packet_parse_type(videotag.videopacket.avcvideopacket.type)
		<< "\navcpacket compositiontime : " << FINT24TOINT(videotag.videopacket.avcvideopacket.compositiontime);
	return os;
}

std::ostream& operator<<(std::ostream& os, const TAGHEADER& tagheader)
{
	os << "type : " << flv_tag_parse_type(tagheader.type)
		<< "\ndatalen : " << FINT24TOINT(tagheader.datalen)
		<< "\ntimestamp : " << FINT32TOINT(tagheader.timestamp)
		<< "\nstreamsid : " << FINT24TOINT(tagheader.streamsid);
	return os;
}

std::ostream& operator<<(std::ostream& os, const SequenceParameterSet& sps)
{
	os << "numOfSequenceParameterSets : " << static_cast<unsigned int>(sps.numOfSequenceParameterSets)
		<< "\nsequenceParameterSetLength : " << FINT16TOINT(sps.sequenceParameterSetLength)
		<< "\nsps : ";
	std::ios::fmtflags f(std::cout.flags());
	for (int i = 0; i < FINT16TOINT(sps.sequenceParameterSetLength); ++i)
	{
		os << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(sps.sequenceParameterSetNALUnit[i]) << " ";
	}
	std::cout.flags(f);
	return os;
}

std::ostream& operator<<(std::ostream& os, const PictureParameterSet& pps)
{
	os << "numOfPictureParameterSets : " << static_cast<unsigned int>(pps.numOfPictureParameterSets)
		<< "\npictureParameterSetLength : " << FINT16TOINT(pps.pictureParameterSetLength)
		<< "\npps : ";
	std::ios::fmtflags f(std::cout.flags());
	for (int i = 0; i < FINT16TOINT(pps.pictureParameterSetLength); ++i)
	{
		os << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(pps.pictureParameterSetNALUnit[i]) << " ";
	}
	std::cout.flags(f);
	return os;
}

std::ostream& operator<<(std::ostream& os, const AVCDecoderConfigurationRecordHeader& configureHeader)
{
	os << "configurationVersion : " << static_cast<unsigned int>(configureHeader.configurationVersion)
		<< "\nAVCProfileIndication : " << static_cast<unsigned int>(configureHeader.AVCProfileIndication)
		<< "\nprofile_compatibility : " << static_cast<unsigned int>(configureHeader.profile_compatibility)
		<< "\nAVCLevelIndication : " << static_cast<unsigned int>(configureHeader.AVCLevelIndication)
		<< "\nlengthSizeMinusOne : " << static_cast<unsigned int>(configureHeader.lengthSizeMinusOne);
	return os;
}

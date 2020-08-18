#pragma once

#include <stdint.h>
#include <iostream>

//FLV tag type
#define FLV_TAG_TYPE_AUDIO    0x08
#define FLV_TAG_TYPE_VIDIO    0x09
#define FLV_TAG_TYPE_SCRIPT   0x12

//flv video tag frame type
#define FLV_VIDEO_FRAME_KEYFRAME	1	//keyframe(for AVC, a seekable frame) —— 即H.264的IDR帧；
#define FLV_VIDEO_FRAME_IFRAME		2	//inter frame(for AVC, a non - seekable frame) —— H.264的普通I帧； 
#define FLV_VIDEO_FRAME_DISPOSABLE	3	//disposable inter frame(H.263 only) 
#define FLV_VIDEO_FRAME_GENERATED	4	//generated keyframe(reserved for server use only) 
#define FLV_VIDEO_FRAME_COMMAND		5	//video info / command frame

//flv video tag frame codecid
#define FLV_VIDEO_CODECID_JPEG		1	//JPEG (currently unused)
#define FLV_VIDEO_CODECID_H263		2	//Sorenson H.263
#define FLV_VIDEO_CODECID_SCREEN	3	//Screen video
#define FLV_VIDEO_CODECID_ON2VP6	4	//On2 VP6
#define FLV_VIDEO_CODECID_ON2VP6A	5	//On2 VP6 with alpha channel
#define FLV_VIDEO_CODECID_SCREEN2	6	//Screen video version 2
#define FLV_VIDEO_CODECID_AVC		7	//AVC

//AVC packet type
#define AVC_PACKET_HEADER	0	//AVC sequence header
#define AVC_PACKET_NALU		1	//AVC NALU
#define AVC_PACKET_END		2	//AVC end of sequence

#pragma pack(1)
typedef struct __GINT16
{
	uint8_t data1;
	uint8_t data2;
}GINT16;
typedef struct __GINT24
{
	uint8_t data1;
	uint8_t data2;
	uint8_t data3;
}GINT24;
typedef struct __GINT32
{
	uint8_t data1;
	uint8_t data2;
	uint8_t data3;
	uint8_t data4;
}GINT32;
typedef struct __GTIMESTAMP
{
	uint8_t data2;
	uint8_t data3;
	uint8_t data4;
	uint8_t data1;
}GTIMESTAMP;

/*FLV文件头*/
typedef struct __FLVHEADER
{
	uint8_t f;			//0x46
	uint8_t l;			//0x4C
	uint8_t v;			//0x56
	uint8_t type;		//版本
	uint8_t video : 1;	//是否有视频
	uint8_t : 1;			//全为0
	uint8_t audio : 1;	//是否有音频
	uint8_t : 5;			//全为0
	GINT32 len;			//文件头长度9
}FLVHEADER;
/*****************************/
/*前一个tag的长度(4字节)*/
/*****************************/
/*tag头*/
typedef struct __TAGHEADER
{
	uint8_t type;			//0x08--音频 0x09--视频 0x12--脚本
	GINT24 datalen;			//数据区的长度
	GTIMESTAMP timestamp;	//时间戳
	GINT24 streamsid;		//流信息
}TAGHEADER;
/*****************************/
/*tag数据区*/
typedef struct __VIDEOTAG
{
	uint8_t codecid : 4;		//编解码器
	uint8_t type : 4;		//视频帧类型
	union
	{
		//codecid == VIDEO_TAG_FRAME_CODECID_AVC
		struct AVCVIDEOPACKE {
			uint8_t type;
			//如果AVCPacketType=1，则为时间cts偏移量；否则，为0。当B帧的存在时，视频解码呈现过程中，dts、pts可能不同，cts的计算公式为 pts - dts/90，单位为毫秒；如果B帧不存在，则cts固定为0。
			GINT24 compositiontime;
			//AVCPacketType=0，则为AVCDecoderConfigurationRecord，H.264 视频解码所需要的参数集（SPS、PPS）
			//AVCPacketType=1，则为NALU（一个或多个）
			//如果AVCPacketType=2，则为空
			unsigned char data[];
		}avcvideopacket;
	}videopacket;
}VIDEOTAG;
//AVCDecoderConfigurationRecord = AVCDecoderConfigurationRecordHeader + SequenceParameterSet + PictureParameterSet
typedef struct __AVCDecoderConfigurationRecordHeader
{
	uint8_t configurationVersion;
	uint8_t AVCProfileIndication;
	uint8_t profile_compatibility;
	uint8_t AVCLevelIndication;
	uint8_t lengthSizeMinusOne : 2;
	uint8_t : 6;
	uint8_t data[];//后续数据
}AVCDecoderConfigurationRecordHeader;
typedef struct __SequenceParameterSet
{
	uint8_t numOfSequenceParameterSets : 5;
	uint8_t : 3;
	GINT16 sequenceParameterSetLength;
	uint8_t sequenceParameterSetNALUnit[];
}SequenceParameterSet;
typedef struct __PictureParameterSet
{
	uint8_t numOfPictureParameterSets;
	GINT16 pictureParameterSetLength;
	uint8_t pictureParameterSetNALUnit[];
}PictureParameterSet;
/*****************************/
/*...........................*/
/*前一个tag的长度(4字节)*/
/*EOF*/

#pragma pack()

#define FINT16TOINT(x) ((x.data1<<8 & 0xff00) | (x.data2 & 0xff))
#define FINT24TOINT(x) ((x.data1<<16 & 0xff0000) | (x.data2<<8 & 0xff00) | (x.data3 & 0xff))
#define FINT32TOINT(x) ((x.data1<<24 & 0xff000000) | (x.data2<<16 & 0xff0000) | (x.data3<<8 & 0xff00) | (x.data4 & 0xff))

const char* flv_tag_parse_type(uint8_t type);
const char* flv_video_parse_type(uint8_t type);
const char* flv_video_parse_codecid(uint8_t codecid);
const char* avc_packet_parse_type(uint8_t type);

std::ostream& operator<<(std::ostream& os, const FLVHEADER& flvheader);
std::ostream& operator<<(std::ostream& os, const TAGHEADER& tagheader);
std::ostream& operator<<(std::ostream& os, const VIDEOTAG& videotag);
std::ostream& operator<<(std::ostream& os, const AVCDecoderConfigurationRecordHeader& configureHeader);
std::ostream& operator<<(std::ostream& os, const SequenceParameterSet& sps);
std::ostream& operator<<(std::ostream& os, const PictureParameterSet& sps);

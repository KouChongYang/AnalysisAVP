/*
 * @Author: gongluck
 * @Date: 2020-09-02 23:40:40
 * @Last Modified by:   gongluck
 * @Last Modified time: 2020-09-02 23:40:40
 */

#include <iostream>
#include <iomanip>
#include <fstream>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

const int bufsize = 1024000;

int read_packet(void* opaque, uint8_t* buf, int buf_size)
{
	std::ifstream* in = static_cast<std::ifstream*>(opaque);
	in->read(reinterpret_cast<char*>(buf), buf_size);
	return in ? in->gcount() : EOF;
}

int write_packet(void* opaque, uint8_t* buf, int buf_size)
{
	std::ofstream* outfile = static_cast<std::ofstream*>(opaque);
	outfile->write((char*)buf, buf_size);
	//std::cout << "write " << buf_size << std::endl;
	return buf_size;
}
int64_t seek_packet(void* opaque, int64_t offset, int whence)
{
	//std::ofstream* outfile = static_cast<std::ofstream*>(opaque);
	//outfile->seekp(offset, whence);
	//std::cout << "seek " << offset << std::endl;
	return 0;
}

int main(int argc, char* argv[])
{
	av_log_set_level(AV_LOG_VERBOSE);

	std::cout << "ffmpeg demo" << std::endl;

	std::cout << "Usage : " << "thisfilename h264file aacfile" << std::endl;

	if (argc < 3)
	{
		std::cerr << "please see the usage message." << std::endl;
		return -1;
	}
	std::ifstream vin(argv[1], std::ios::binary);
	if (vin.fail())
	{
		std::cerr << "can not open file " << argv[1] << std::endl;
		return -1;
	}
	std::ifstream ain(argv[2], std::ios::binary);
	if (ain.fail())
	{
		std::cerr << "can not open file " << argv[2] << std::endl;
		return -1;
	}

	std::ofstream outfile("out.mp4", std::ios::binary);
	AVFormatContext* outctx = nullptr;
	auto ret = avformat_alloc_output_context2(&outctx, nullptr, "mp4", nullptr);
	auto iobuffer = static_cast<unsigned char*>(av_malloc(bufsize));
	auto avio = avio_alloc_context(iobuffer, bufsize, 1, &outfile, nullptr, write_packet, seek_packet);
	outctx->pb = avio;
	outctx->flags = AVFMT_FLAG_CUSTOM_IO;
	AVDictionary* dict = nullptr;
	ret = av_dict_set(&dict, "movflags", "faststart+delay_moov", 0);

	auto vfmt = avformat_alloc_context();
	auto viobuffer = static_cast<unsigned char*>(av_malloc(bufsize));
	auto vavio = avio_alloc_context(viobuffer, bufsize, 0, &vin, read_packet, nullptr, nullptr);
	vfmt->pb = vavio;
	vfmt->flags = AVFMT_FLAG_CUSTOM_IO;
	ret = avformat_open_input(&vfmt, nullptr, nullptr, nullptr);
	ret = avformat_find_stream_info(vfmt, nullptr);

	auto vstream = avformat_new_stream(outctx, nullptr);
	ret = avcodec_copy_context(outctx->streams[vstream->index]->codec, vfmt->streams[0]->codec);
	ret = avcodec_parameters_copy(outctx->streams[vstream->index]->codecpar, vfmt->streams[0]->codecpar);
	if (outctx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		outctx->streams[vstream->index]->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}

	auto afmt = avformat_alloc_context();
	auto aiobuffer = static_cast<unsigned char*>(av_malloc(bufsize));
	auto aavio = avio_alloc_context(aiobuffer, bufsize, 0, &ain, read_packet, nullptr, nullptr);
	afmt->pb = aavio;
	afmt->flags = AVFMT_FLAG_CUSTOM_IO;
	ret = avformat_open_input(&afmt, nullptr, nullptr, nullptr);
	ret = avformat_find_stream_info(afmt, nullptr);

	auto astream = avformat_new_stream(outctx, nullptr);
	ret = avcodec_copy_context(outctx->streams[astream->index]->codec, afmt->streams[0]->codec);
	ret = avcodec_parameters_copy(outctx->streams[astream->index]->codecpar, afmt->streams[0]->codecpar);
	if (outctx->oformat->flags & AVFMT_GLOBALHEADER)
	{
		outctx->streams[astream->index]->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}

	ret = avformat_write_header(outctx, &dict);

	auto pkt = av_packet_alloc();
	av_init_packet(pkt);
	while (av_read_frame(vfmt, pkt) == 0)
	{
		pkt->stream_index = vstream->index;
		if (pkt->pts == AV_NOPTS_VALUE)
		{
			static int64_t pts = 0;
			pkt->pts = pkt->dts = av_rescale_q(pts++, { 4, 95 }, outctx->streams[vstream->index]->time_base);
		}
		pkt->pos = -1;
		ret = av_interleaved_write_frame(outctx, pkt);
		av_packet_unref(pkt);
	}

	auto aacbsfc = av_bitstream_filter_init("aac_adtstoasc");
	while (av_read_frame(afmt, pkt) == 0)
	{
		ret = av_bitstream_filter_filter(aacbsfc, afmt->streams[0]->codec, nullptr, &pkt->data, &pkt->size, pkt->data, pkt->size, 0);
		pkt->stream_index = astream->index;
		static int64_t pts = 0;
		pkt->pts = pkt->dts = av_rescale_q(pts, afmt->streams[0]->time_base, outctx->streams[astream->index]->time_base);
		pts += pkt->duration;
		pkt->duration = 0;//next_pts - this_pts
		pkt->pos = -1;
		ret = av_interleaved_write_frame(outctx, pkt);
		av_packet_unref(pkt);
	}
	av_bitstream_filter_close(aacbsfc);

	av_packet_free(&pkt);

	ret = av_write_trailer(outctx);

	if (vfmt->pb != nullptr)
	{
		avio_context_free(&vfmt->pb);
	}
	avformat_close_input(&vfmt);
	if (afmt->pb != nullptr)
	{
		avio_context_free(&afmt->pb);
	}
	avformat_close_input(&afmt);
	if (outctx->pb != nullptr)
	{
		avio_context_free(&outctx->pb);
	}
	avformat_free_context(outctx);

	return 0;
}

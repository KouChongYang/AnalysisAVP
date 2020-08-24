/*
 * @Author: gongluck
 * @Date: 2020-08-24 20:38:45
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-08-24 20:40:20
 */
#pragma once

 //Advanced Audio Coding(⾼级⾳频编码)

#include <stdint.h>
#include <iostream>

typedef struct __ADTS
{
	struct __ADTS_FIXED_HEADER//固定头信息
	{
		uint8_t syncword : 12;					//同步头 总是0xFFF, all bits must be 1，代表着⼀个ADTS帧的开始
		uint8_t ID : 1;							//MPEG标识符，0标识MPEG-4，1标识MPEG-2
		uint8_t layer : 2;						//always: '00'
		uint8_t protection_absent : 1;			//表示是否误码校验。Warning, set to 1 if there is no CRC and 0 if there is CRC
		uint8_t profile : 2;					//profile：表示使⽤哪个级别的AAC，如01 Low Complexity(LC)--- AAC LC。有些芯⽚只⽀持AAC LC。
		uint8_t sampling_frequency_index : 4;	//表示使⽤的采样率下标，通过这个下标在Sampling Frequencies[]数组中查找得知采样率的值
		uint8_t channel_configuration : 3;		//表示声道数，⽐如2表示⽴体声双声道
		uint8_t original_copy : 1;
		uint8_t home : 1;
	}adts_fixed_header;
	struct __ADTS_VARIABLE_HEADER//可变头信息
	{
		uint8_t copyright_identification_bit : 1;
		uint8_t copyright_identification_start : 1;
		uint8_t aac_frame_length : 13;	//⼀个ADTS帧的⻓度包括ADTS头和AAC原始流.
										//frame length, this value must include 7 or 9 bytes of header length :
										//aac_frame_length = (protection_absent == 1 ? 7 : 9) + size(AACFrame)
										//protection_absent = 0时, header length = 9bytes
										//protection_absent = 1时, header length = 7bytes
		uint8_t adts_buffer_fullness : 11;	//0x7FF 说明是码率可变的码流。
		uint8_t number_of_raw_data_blocks : 2;	//表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧。
												//所以说number_of_raw_data_blocks_in_frame == 0 表示说ADTS帧中有⼀个AAC数据块。
	}adts_variable_header;
};

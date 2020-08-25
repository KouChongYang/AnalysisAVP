/*
 * @Author: gongluck
 * @Date: 2020-08-24 20:40:30
 * @Last Modified by:   gongluck
 * @Last Modified time: 2020-08-24 20:40:30
 */

#include "aac.h"

uint16_t get_syncword(const ADTS& adts)
{
	return adts.syncwordH << 8 | adts.syncwordL;
}
void set_syncword(ADTS& adts, uint16_t syncword)
{
	adts.syncwordH = syncword >> 8 & 0x0f;
	adts.syncwordL = syncword & 0xff;
}

uint8_t get_channel_configuration(const ADTS& adts)
{
	return adts.channel_configurationH << 2 | adts.channel_configurationL;
}
void set_channel_configuration(ADTS& adts, uint8_t channel_configuration)
{
	adts.channel_configurationH = channel_configuration >> 2 & 0x01;
	adts.channel_configurationL = channel_configuration & 0x03;
}

uint16_t get_aac_frame_length(const ADTS& adts)
{
	return adts.aac_frame_lengthH << 11 | adts.aac_frame_lengthM << 3 | adts.aac_frame_lengthL;
}
void set_aac_frame_length(ADTS& adts, uint16_t aac_frame_length)
{
	adts.aac_frame_lengthH = aac_frame_length >> 11 & 0x03;
	adts.aac_frame_lengthM = aac_frame_length >> 3 & 0x0f;
	adts.aac_frame_lengthL = aac_frame_length & 0x07;
}

uint16_t get_adts_buffer_fullness(const ADTS& adts)
{
	return adts.adts_buffer_fullnessH << 6 | adts.adts_buffer_fullnessL;
}
void set_adts_buffer_fullness(ADTS& adts, uint16_t adts_buffer_fullness)
{
	adts.adts_buffer_fullnessH = adts_buffer_fullness >> 6 & 0x1F;
	adts.adts_buffer_fullnessL = adts_buffer_fullness & 0x3f;
}

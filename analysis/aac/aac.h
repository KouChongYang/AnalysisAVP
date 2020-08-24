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
		uint8_t syncword : 12;
	}adts_fixed_header;
	struct __ADTS_VARIABLE_HEADER//可变头信息
	{

	}adts_variable_header;
};

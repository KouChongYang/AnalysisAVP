#include "h264.h"

const char* nal_parse_idc(uint8_t idc)
{
	switch (idc)
	{
	case NAL_REF_IDC_PRIORITY_HIGHEST:
		return "HIGHEST";
	case  NAL_REF_IDC_PRIORITY_HIGH:
		return "HEIGHT";
	case  NAL_REF_IDC_PRIORITY_LOW:
		return "LOW";
	case  NAL_REF_IDC_PRIORITY_DISPOSABLE:
		return "DISPOSABLE";
	default:
		return "UNKNOWN";
	}
}

const char* nal_parse_type(uint8_t type)
{
	switch (type)
	{
	case NAL_UNIT_TYPE_UNSPECIFIED:
		return "Unspecified";
	case NAL_UNIT_TYPE_CODED_SLICE_NON_IDR:
		return "Coded slice of a non-IDR picture";
	case NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A:
		return "Coded slice data partition A";
	case NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B:
		return "Coded slice data partition B";
	case NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C:
		return "Coded slice data partition C";
	case NAL_UNIT_TYPE_CODED_SLICE_IDR:
		return "Coded slice of an IDR picture";
	case NAL_UNIT_TYPE_SEI:
		return "Supplemental enhancement information (SEI)";
	case NAL_UNIT_TYPE_SPS:
		return "Sequence parameter set";
	case NAL_UNIT_TYPE_PPS:
		return "Picture parameter set";
	case NAL_UNIT_TYPE_AUD:
		return "Access unit delimiter";
	case NAL_UNIT_TYPE_END_OF_SEQUENCE:
		return "End of sequence";
	case NAL_UNIT_TYPE_END_OF_STREAM:
		return "End of stream";
	case NAL_UNIT_TYPE_FILLER:
		return "Filler data";
	case NAL_UNIT_TYPE_SPS_EXT:
		return "Sequence parameter set extension";
	case NAL_UNIT_TYPE_CODED_SLICE_AUX:
		return "Coded slice of an auxiliary coded picture without partitioning";
	default:
		return "UNKNOWN";
	}
}

std::ostream& operator<<(std::ostream& os, const NALHEADER& nalheader)
{
	os << "nal_ref_idc : " << nal_parse_idc(nalheader.nal_ref_idc)
		<< "\nnal_unit_type : " << nal_parse_type(nalheader.nal_unit_type);
	return os;
}

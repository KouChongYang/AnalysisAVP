#pragma once

//NAL ref idc codes
#define NAL_REF_IDC_PRIORITY_HIGHEST    3
#define NAL_REF_IDC_PRIORITY_HIGH       2
#define NAL_REF_IDC_PRIORITY_LOW        1
#define NAL_REF_IDC_PRIORITY_DISPOSABLE 0

//Table 7-1 NAL unit type codes
#define NAL_UNIT_TYPE_UNSPECIFIED                    0    // Unspecified
#define NAL_UNIT_TYPE_CODED_SLICE_NON_IDR            1    // Coded slice of a non-IDR picture
#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A   2    // Coded slice data partition A
#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B   3    // Coded slice data partition B
#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C   4    // Coded slice data partition C
#define NAL_UNIT_TYPE_CODED_SLICE_IDR                5    // Coded slice of an IDR picture
#define NAL_UNIT_TYPE_SEI                            6    // Supplemental enhancement information (SEI)
#define NAL_UNIT_TYPE_SPS                            7    // Sequence parameter set
#define NAL_UNIT_TYPE_PPS                            8    // Picture parameter set
#define NAL_UNIT_TYPE_AUD                            9    // Access unit delimiter
#define NAL_UNIT_TYPE_END_OF_SEQUENCE               10    // End of sequence
#define NAL_UNIT_TYPE_END_OF_STREAM                 11    // End of stream
#define NAL_UNIT_TYPE_FILLER                        12    // Filler data
#define NAL_UNIT_TYPE_SPS_EXT                       13    // Sequence parameter set extension
											 // 14..18    // Reserved
#define NAL_UNIT_TYPE_CODED_SLICE_AUX               19    // Coded slice of an auxiliary coded picture without partitioning
											 // 20..23    // Reserved
											 // 24..31    // Unspecified

typedef struct __NALHEADER
{
	unsigned int nal_unit_type : 5;//NAL_UNIT_TYPE_XXX，NAL单元的类型
	unsigned int nal_ref_idc : 2;//NAL_REF_IDC_PRIORITY_XXX，标识重要性，3最高
	unsigned int forbidden_zero_bit : 1;//必须为0
}NALHEADER;

const char* parse_idc(unsigned int idc)
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

const char* parse_type(unsigned int type)
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
	os << "nal_ref_idc : " << parse_idc(nalheader.nal_ref_idc)
		<< "\nnal_unit_type : " << parse_type(nalheader.nal_unit_type);
	return os;
}

#include "QBaseStdAfx.h"
#include "QBase.h"

namespace q
{
namespace util
{

void memcpy(uint8_t* dst, size_t dst_stride_bytes, uint8_t const* src, size_t src_stride_bytes, size_t element_size_bytes, size_t count)
{
    QASSERT(src && dst && count && element_size_bytes);
    src_stride_bytes = src_stride_bytes ? src_stride_bytes : element_size_bytes;
    dst_stride_bytes = dst_stride_bytes ? dst_stride_bytes : element_size_bytes;

	if ((((size_t)dst | (size_t)src) & 0x3) != 0 ||
        ((dst_stride_bytes | src_stride_bytes) & 0x3) != 0)
	{
		//slow, unaligned version
		for (size_t i = 0; i < count; i++)
		{
            ::memcpy(dst, src, element_size_bytes);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		return;
	}

    switch (element_size_bytes)
	{
	case 1:
		for (size_t i = 0; i < count; i++)
		{
			(*dst) = (*src);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 2:
		for (size_t i = 0; i < count; i++)
		{
			(*(uint16_t*)dst) = (*(uint16_t*)src);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 4:
		for (size_t i = 0; i < count; i++)
		{
			(*(uint32_t*)dst) = (*(uint32_t*)src);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 8:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 12:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 16:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 20:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 24:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
			((uint32_t*)dst)[5] = ((uint32_t*)src)[5];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 28:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
			((uint32_t*)dst)[5] = ((uint32_t*)src)[5];
			((uint32_t*)dst)[6] = ((uint32_t*)src)[6];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 32:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
			((uint32_t*)dst)[5] = ((uint32_t*)src)[5];
			((uint32_t*)dst)[6] = ((uint32_t*)src)[6];
			((uint32_t*)dst)[7] = ((uint32_t*)src)[7];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	default:
		//slow version
		for (size_t i = 0; i < count; i++)
		{
            ::memcpy(dst, src, element_size_bytes);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
}
}

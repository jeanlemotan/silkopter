#pragma once

#include <stdint.h>
#include "All_Real_Traits.h"

namespace ts
{

typedef Scalar_Type_Traits<int8_t>  Int8_Type_Traits;
typedef Scalar_Type_Traits<uint8_t> UInt8_Type_Traits;
typedef Scalar_Type_Traits<int16_t>  Int16_Type_Traits;
typedef Scalar_Type_Traits<uint16_t> UInt16_Type_Traits;
typedef Scalar_Type_Traits<int32_t>  Int32_Type_Traits;
typedef Scalar_Type_Traits<uint32_t> UInt32_Type_Traits;
typedef Scalar_Type_Traits<int64_t>  Int64_Type_Traits;

typedef Vec2_Type_Traits<int8_t>    Vec2s8_Type_Traits;
typedef Vec2_Type_Traits<uint8_t>   Vec2u8_Type_Traits;
typedef Vec2_Type_Traits<int16_t>   Vec2s16_Type_Traits;
typedef Vec2_Type_Traits<uint16_t>  Vec2u16_Type_Traits;
typedef Vec2_Type_Traits<int32_t>   Vec2s32_Type_Traits;
typedef Vec2_Type_Traits<uint32_t>  Vec2u32_Type_Traits;

typedef Vec3_Type_Traits<int8_t>    Vec3s8_Type_Traits;
typedef Vec3_Type_Traits<uint8_t>   Vec3u8_Type_Traits;
typedef Vec3_Type_Traits<int16_t>   Vec3s16_Type_Traits;
typedef Vec3_Type_Traits<uint16_t>  Vec3u16_Type_Traits;
typedef Vec3_Type_Traits<int32_t>   Vec3s32_Type_Traits;
typedef Vec3_Type_Traits<uint32_t>  Vec3u32_Type_Traits;

typedef Vec4_Type_Traits<int8_t>    Vec4s8_Type_Traits;
typedef Vec4_Type_Traits<uint8_t>   Vec4u8_Type_Traits;
typedef Vec4_Type_Traits<int16_t>   Vec4s16_Type_Traits;
typedef Vec4_Type_Traits<uint16_t>  Vec4u16_Type_Traits;
typedef Vec4_Type_Traits<int32_t>   Vec4s32_Type_Traits;
typedef Vec4_Type_Traits<uint32_t>  Vec4u32_Type_Traits;

}

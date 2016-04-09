#pragma once

#include "Integral_Type_Template.h"
#include "All_Integral_Traits.h"

namespace ts
{

typedef Integral_Type_Template<UInt8_Type_Traits>   UInt8_Type;
typedef Integral_Type_Template<Int8_Type_Traits>    Int8_Type;
typedef Integral_Type_Template<UInt16_Type_Traits>  UInt16_Type;
typedef Integral_Type_Template<Int16_Type_Traits>   Int16_Type;
typedef Integral_Type_Template<UInt32_Type_Traits>  UInt32_Type;
typedef Integral_Type_Template<Int32_Type_Traits>   Int32_Type;
typedef Integral_Type_Template<Int64_Type_Traits>   Int64_Type;

typedef Integral_Type_Template<Vec2u8_Type_Traits>  Vec2u8_Type;
typedef Integral_Type_Template<Vec2s8_Type_Traits>  Vec2s8_Type;
typedef Integral_Type_Template<Vec2u16_Type_Traits> Vec2u16_Type;
typedef Integral_Type_Template<Vec2s16_Type_Traits> Vec2s16_Type;
typedef Integral_Type_Template<Vec2u32_Type_Traits> Vec2u32_Type;
typedef Integral_Type_Template<Vec2s32_Type_Traits> Vec2s32_Type;

typedef Integral_Type_Template<Vec3u8_Type_Traits>  Vec3u8_Type;
typedef Integral_Type_Template<Vec3s8_Type_Traits>  Vec3s8_Type;
typedef Integral_Type_Template<Vec3u16_Type_Traits> Vec3u16_Type;
typedef Integral_Type_Template<Vec3s16_Type_Traits> Vec3s16_Type;
typedef Integral_Type_Template<Vec3u32_Type_Traits> Vec3u32_Type;
typedef Integral_Type_Template<Vec3s32_Type_Traits> Vec3s32_Type;

typedef Integral_Type_Template<Vec4u8_Type_Traits>  Vec4u8_Type;
typedef Integral_Type_Template<Vec4s8_Type_Traits>  Vec4s8_Type;
typedef Integral_Type_Template<Vec4u16_Type_Traits> Vec4u16_Type;
typedef Integral_Type_Template<Vec4s16_Type_Traits> Vec4s16_Type;
typedef Integral_Type_Template<Vec4u32_Type_Traits> Vec4u32_Type;
typedef Integral_Type_Template<Vec4s32_Type_Traits> Vec4s32_Type;

}

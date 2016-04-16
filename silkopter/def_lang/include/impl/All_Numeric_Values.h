#pragma once

#include "Numeric_Value_Template.h"
#include "impl/All_Numeric_Types.h"

namespace ts
{

typedef Numeric_Value_Template<Scalar_Numeric_Traits<float>>     Float_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<double>>    Double_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<int8_t>>    Int8_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<uint8_t>>   UInt8_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<int16_t>>   Int16_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<uint16_t>>  UInt16_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<int32_t>>   Int32_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<uint32_t>>  UInt32_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<int64_t>>   Int64_Value;

typedef Numeric_Value_Template<Vec2_Numeric_Traits<float>>       Vec2f_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<double>>      Vec2d_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<int8_t>>      Vec2s8_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<uint8_t>>     Vec2u8_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<int16_t>>     Vec2s16_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<uint16_t>>    Vec2u16_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<int32_t>>     Vec2s32_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<uint32_t>>    Vec2u32_Value;

typedef Numeric_Value_Template<Vec3_Numeric_Traits<float>>       Vec3f_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<double>>      Vec3d_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<int8_t>>      Vec3s8_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<uint8_t>>     Vec3u8_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<int16_t>>     Vec3s16_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<uint16_t>>    Vec3u16_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<int32_t>>     Vec3s32_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<uint32_t>>    Vec3u32_Value;

typedef Numeric_Value_Template<Vec4_Numeric_Traits<float>>       Vec4f_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<double>>      Vec4d_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<int8_t>>      Vec4s8_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<uint8_t>>     Vec4u8_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<int16_t>>     Vec4s16_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<uint16_t>>    Vec4u16_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<int32_t>>     Vec4s32_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<uint32_t>>    Vec4u32_Value;


}

#pragma once

#include "INumeric_Value_Template.h"
#include "All_INumeric_Types.h"

namespace ts
{

typedef INumeric_Value_Template<Scalar_INumeric_Traits<float>>     IFloat_Value;
typedef INumeric_Value_Template<Scalar_INumeric_Traits<double>>    IDouble_Value;
typedef INumeric_Value_Template<Scalar_INumeric_Traits<int64_t>>   IInt64_Value;

typedef INumeric_Value_Template<Vec2_INumeric_Traits<float>>       IVec2f_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<double>>      IVec2d_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<int8_t>>      IVec2s8_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<uint8_t>>     IVec2u8_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<int16_t>>     IVec2s16_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<uint16_t>>    IVec2u16_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<int32_t>>     IVec2s32_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<uint32_t>>    IVec2u32_Value;

typedef INumeric_Value_Template<Vec3_INumeric_Traits<float>>       IVec3f_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<double>>      IVec3d_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<int8_t>>      IVec3s8_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<uint8_t>>     IVec3u8_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<int16_t>>     IVec3s16_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<uint16_t>>    IVec3u16_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<int32_t>>     IVec3s32_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<uint32_t>>    IVec3u32_Value;

typedef INumeric_Value_Template<Vec4_INumeric_Traits<float>>       IVec4f_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<double>>      IVec4d_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<int8_t>>      IVec4s8_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<uint8_t>>     IVec4u8_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<int16_t>>     IVec4s16_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<uint16_t>>    IVec4u16_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<int32_t>>     IVec4s32_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<uint32_t>>    IVec4u32_Value;

}

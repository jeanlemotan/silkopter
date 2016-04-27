#pragma once

#include "INumeric_Value_Template.h"
#include "All_INumeric_Types.h"

namespace ts
{

typedef INumeric_Value_Template<Scalar_INumeric_Traits<float>>     IFloat_Value;
typedef INumeric_Value_Template<Scalar_INumeric_Traits<double>>    IDouble_Value;
typedef INumeric_Value_Template<Scalar_INumeric_Traits<int64_t>>   IInt_Value;

typedef INumeric_Value_Template<Vec2_INumeric_Traits<float>>       IVec2f_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<double>>      IVec2d_Value;
typedef INumeric_Value_Template<Vec2_INumeric_Traits<int64_t>>     IVec2i_Value;

typedef INumeric_Value_Template<Vec3_INumeric_Traits<float>>       IVec3f_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<double>>      IVec3d_Value;
typedef INumeric_Value_Template<Vec3_INumeric_Traits<int64_t>>     IVec3i_Value;

typedef INumeric_Value_Template<Vec4_INumeric_Traits<float>>       IVec4f_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<double>>      IVec4d_Value;
typedef INumeric_Value_Template<Vec4_INumeric_Traits<int64_t>>     IVec4i_Value;

}

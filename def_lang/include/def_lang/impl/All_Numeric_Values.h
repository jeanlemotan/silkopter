#pragma once

#include "Numeric_Value_Template.h"
#include "def_lang/impl/All_Numeric_Types.h"

namespace ts
{

typedef Numeric_Value_Template<Scalar_Numeric_Traits<float>>     Float_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<double>>    Double_Value;
typedef Numeric_Value_Template<Scalar_Numeric_Traits<int64_t>>   Int_Value;

typedef Numeric_Value_Template<Vec2_Numeric_Traits<float>>       Vec2f_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<double>>      Vec2d_Value;
typedef Numeric_Value_Template<Vec2_Numeric_Traits<int64_t>>     Vec2i_Value;

typedef Numeric_Value_Template<Vec3_Numeric_Traits<float>>       Vec3f_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<double>>      Vec3d_Value;
typedef Numeric_Value_Template<Vec3_Numeric_Traits<int64_t>>     Vec3i_Value;

typedef Numeric_Value_Template<Vec4_Numeric_Traits<float>>       Vec4f_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<double>>      Vec4d_Value;
typedef Numeric_Value_Template<Vec4_Numeric_Traits<int64_t>>     Vec4i_Value;


}

#pragma once

#include "IReal_Value_Template.h"
#include "types/All_Real_Traits.h"

namespace ts
{

typedef IReal_Value_Template<Float_Type_Traits> IFloat_Value;
typedef IReal_Value_Template<Double_Type_Traits> IDouble_Value;

typedef IReal_Value_Template<Vec2f_Type_Traits> IVec2f_Value;
typedef IReal_Value_Template<Vec2d_Type_Traits> IVec2d_Value;

typedef IReal_Value_Template<Vec3f_Type_Traits> IVec3f_Value;
typedef IReal_Value_Template<Vec3d_Type_Traits> IVec3d_Value;

typedef IReal_Value_Template<Vec4f_Type_Traits> IVec4f_Value;
typedef IReal_Value_Template<Vec4d_Type_Traits> IVec4d_Value;

}

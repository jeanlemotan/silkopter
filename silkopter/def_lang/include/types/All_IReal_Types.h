#pragma once

#include "IReal_Type_Template.h"
#include "All_Real_Traits.h"

namespace ts
{

typedef IReal_Type_Template<Float_Type_Traits> IFloat_Type;
typedef IReal_Type_Template<Double_Type_Traits> IDouble_Type;

typedef IReal_Type_Template<Vec2f_Type_Traits> IVec2f_Type;
typedef IReal_Type_Template<Vec2d_Type_Traits> IVec2d_Type;

typedef IReal_Type_Template<Vec3f_Type_Traits> IVec3f_Type;
typedef IReal_Type_Template<Vec3d_Type_Traits> IVec3d_Type;

typedef IReal_Type_Template<Vec4f_Type_Traits> IVec4f_Type;
typedef IReal_Type_Template<Vec4d_Type_Traits> IVec4d_Type;

}

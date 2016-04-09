#pragma once

#include "Real_Type_Template.h"
#include "All_Real_Traits.h"

namespace ts
{

typedef Real_Type_Template<Float_Type_Traits>    Float_Type;
typedef Real_Type_Template<Double_Type_Traits>   Double_Type;

typedef Real_Type_Template<Vec2f_Type_Traits>    Vec2f_Type;
typedef Real_Type_Template<Vec2d_Type_Traits>    Vec2d_Type;

typedef Real_Type_Template<Vec3f_Type_Traits>    Vec3f_Type;
typedef Real_Type_Template<Vec3d_Type_Traits>    Vec3d_Type;

typedef Real_Type_Template<Vec4f_Type_Traits>    Vec4f_Type;
typedef Real_Type_Template<Vec4d_Type_Traits>    Vec4d_Type;

}

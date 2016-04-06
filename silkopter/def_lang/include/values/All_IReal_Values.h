#pragma once

#include "IReal_Value.h"
#include "types/All_Real_Traits.h"

namespace ts
{

typedef IReal_Value<Float_Type_Traits> IFloat_Value;
typedef IReal_Value<Double_Type_Traits> IDouble_Value;

typedef IReal_Value<Vec2f_Type_Traits> IVec2f_Value;
typedef IReal_Value<Vec2d_Type_Traits> IVec2d_Value;
typedef IReal_Value<Vec2u8_Type_Traits> IVec2u8_Value;
typedef IReal_Value<Vec2s8_Type_Traits> IVec2s8_Value;
typedef IReal_Value<Vec2u16_Type_Traits> IVec2u16_Value;
typedef IReal_Value<Vec2s16_Type_Traits> IVec2s16_Value;
typedef IReal_Value<Vec2u32_Type_Traits> IVec2u32_Value;
typedef IReal_Value<Vec2s32_Type_Traits> IVec2s32_Value;

typedef IReal_Value<Vec3f_Type_Traits> IVec3f_Value;
typedef IReal_Value<Vec3d_Type_Traits> IVec3d_Value;
typedef IReal_Value<Vec3u8_Type_Traits> IVec3u8_Value;
typedef IReal_Value<Vec3s8_Type_Traits> IVec3s8_Value;
typedef IReal_Value<Vec3u16_Type_Traits> IVec3u16_Value;
typedef IReal_Value<Vec3s16_Type_Traits> IVec3s16_Value;
typedef IReal_Value<Vec3u32_Type_Traits> IVec3u32_Value;
typedef IReal_Value<Vec3s32_Type_Traits> IVec3s32_Value;

typedef IReal_Value<Vec4f_Type_Traits> IVec4f_Value;
typedef IReal_Value<Vec4d_Type_Traits> IVec4d_Value;
typedef IReal_Value<Vec4u8_Type_Traits> IVec4u8_Value;
typedef IReal_Value<Vec4s8_Type_Traits> IVec4s8_Value;
typedef IReal_Value<Vec4u16_Type_Traits> IVec4u16_Value;
typedef IReal_Value<Vec4s16_Type_Traits> IVec4s16_Value;
typedef IReal_Value<Vec4u32_Type_Traits> IVec4u32_Value;
typedef IReal_Value<Vec4s32_Type_Traits> IVec4s32_Value;

}

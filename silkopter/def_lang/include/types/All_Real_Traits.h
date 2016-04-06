#pragma once

#include <stdint.h>

namespace ts
{

struct Float_Type_Traits
{
    typedef float fundamental_type;
};

struct Double_Type_Traits
{
    typedef double fundamental_type;
};


template<class T> struct vec2
{
    T x, y;
};
template<class T> struct Vec2_Type_Traits
{
    typedef vec2<T> fundamental_type;
};

typedef Vec2_Type_Traits<float> Vec2f_Type_Traits;
typedef Vec2_Type_Traits<float> Vec2d_Type_Traits;
typedef Vec2_Type_Traits<int8_t> Vec2s8_Type_Traits;
typedef Vec2_Type_Traits<uint8_t> Vec2u8_Type_Traits;
typedef Vec2_Type_Traits<int16_t> Vec2s16_Type_Traits;
typedef Vec2_Type_Traits<uint16_t> Vec2u16_Type_Traits;
typedef Vec2_Type_Traits<int32_t> Vec2s32_Type_Traits;
typedef Vec2_Type_Traits<uint32_t> Vec2u32_Type_Traits;


template<class T> struct vec3
{
    T x, y, z;
};
template<class T> struct Vec3_Type_Traits
{
    typedef vec3<T> fundamental_type;
};

typedef Vec3_Type_Traits<float> Vec3f_Type_Traits;
typedef Vec3_Type_Traits<float> Vec3d_Type_Traits;
typedef Vec3_Type_Traits<int8_t> Vec3s8_Type_Traits;
typedef Vec3_Type_Traits<uint8_t> Vec3u8_Type_Traits;
typedef Vec3_Type_Traits<int16_t> Vec3s16_Type_Traits;
typedef Vec3_Type_Traits<uint16_t> Vec3u16_Type_Traits;
typedef Vec3_Type_Traits<int32_t> Vec3s32_Type_Traits;
typedef Vec3_Type_Traits<uint32_t> Vec3u32_Type_Traits;


template<class T> struct vec4
{
    T x, y, z, w;
};
template<class T> struct Vec4_Type_Traits
{
    typedef vec4<T> fundamental_type;
};

typedef Vec4_Type_Traits<float> Vec4f_Type_Traits;
typedef Vec4_Type_Traits<float> Vec4d_Type_Traits;
typedef Vec4_Type_Traits<int8_t> Vec4s8_Type_Traits;
typedef Vec4_Type_Traits<uint8_t> Vec4u8_Type_Traits;
typedef Vec4_Type_Traits<int16_t> Vec4s16_Type_Traits;
typedef Vec4_Type_Traits<uint16_t> Vec4u16_Type_Traits;
typedef Vec4_Type_Traits<int32_t> Vec4s32_Type_Traits;
typedef Vec4_Type_Traits<uint32_t> Vec4u32_Type_Traits;

}

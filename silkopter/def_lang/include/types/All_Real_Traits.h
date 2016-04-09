#pragma once

#include <stdint.h>

namespace ts
{

template<class T> struct Scalar_Type_Traits
{
    typedef T fundamental_type;
};


typedef Scalar_Type_Traits<float> Float_Type_Traits;
typedef Scalar_Type_Traits<double> Double_Type_Traits;

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

}

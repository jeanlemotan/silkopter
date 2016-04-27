#pragma once

#include "Numeric_Type_Template.h"
#include "def_lang/All_INumeric_Types.h"

namespace ts
{

template<typename Traits> class Numeric_Value_Template;
template<typename Traits> class Numeric_Type_Template;


template<class T> struct Scalar_Numeric_Traits : public Scalar_INumeric_Traits<T>
{
    typedef Numeric_Value_Template<Scalar_Numeric_Traits<T>> value_implementation;
    typedef Numeric_Type_Template<Scalar_Numeric_Traits<T>> type_implementation;
    typedef Scalar_Numeric_Traits<T> component_implementation_traits;
};

template<class T> struct Vec2_Numeric_Traits : public Vec2_INumeric_Traits<T>
{
    typedef Numeric_Value_Template<Vec2_Numeric_Traits<T>> value_implementation;
    typedef Numeric_Type_Template<Vec2_Numeric_Traits<T>> type_implementation;
    typedef Scalar_Numeric_Traits<T> component_implementation_traits;
};

template<class T> struct Vec3_Numeric_Traits : public Vec3_INumeric_Traits<T>
{
    typedef Numeric_Value_Template<Vec3_Numeric_Traits<T>> value_implementation;
    typedef Numeric_Type_Template<Vec3_Numeric_Traits<T>> type_implementation;
    typedef Scalar_Numeric_Traits<T> component_implementation_traits;
};

template<class T> struct Vec4_Numeric_Traits : public Vec4_INumeric_Traits<T>
{
    typedef Numeric_Value_Template<Vec4_Numeric_Traits<T>> value_implementation;
    typedef Numeric_Type_Template<Vec4_Numeric_Traits<T>> type_implementation;
    typedef Scalar_Numeric_Traits<T> component_implementation_traits;
};


typedef Numeric_Type_Template<Scalar_Numeric_Traits<float>>     Float_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<double>>    Double_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<int64_t>>   Int_Type;

typedef Numeric_Type_Template<Vec2_Numeric_Traits<float>>       Vec2f_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<double>>      Vec2d_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<int64_t>>     Vec2i_Type;

typedef Numeric_Type_Template<Vec3_Numeric_Traits<float>>       Vec3f_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<double>>      Vec3d_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<int64_t>>     Vec3i_Type;

typedef Numeric_Type_Template<Vec4_Numeric_Traits<float>>       Vec4f_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<double>>      Vec4d_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<int64_t>>     Vec4i_Type;

}

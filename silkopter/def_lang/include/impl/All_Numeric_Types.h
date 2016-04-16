#pragma once

#include "Numeric_Type_Template.h"
#include "All_INumeric_Types.h"

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
typedef Numeric_Type_Template<Scalar_Numeric_Traits<int8_t>>    Int8_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<uint8_t>>   UInt8_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<int16_t>>   Int16_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<uint16_t>>  UInt16_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<int32_t>>   Int32_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<uint32_t>>  UInt32_Type;
typedef Numeric_Type_Template<Scalar_Numeric_Traits<int64_t>>   Int64_Type;

typedef Numeric_Type_Template<Vec2_Numeric_Traits<float>>       Vec2f_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<double>>      Vec2d_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<int8_t>>      Vec2s8_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<uint8_t>>     Vec2u8_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<int16_t>>     Vec2s16_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<uint16_t>>    Vec2u16_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<int32_t>>     Vec2s32_Type;
typedef Numeric_Type_Template<Vec2_Numeric_Traits<uint32_t>>    Vec2u32_Type;

typedef Numeric_Type_Template<Vec3_Numeric_Traits<float>>       Vec3f_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<double>>      Vec3d_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<int8_t>>      Vec3s8_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<uint8_t>>     Vec3u8_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<int16_t>>     Vec3s16_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<uint16_t>>    Vec3u16_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<int32_t>>     Vec3s32_Type;
typedef Numeric_Type_Template<Vec3_Numeric_Traits<uint32_t>>    Vec3u32_Type;

typedef Numeric_Type_Template<Vec4_Numeric_Traits<float>>       Vec4f_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<double>>      Vec4d_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<int8_t>>      Vec4s8_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<uint8_t>>     Vec4u8_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<int16_t>>     Vec4s16_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<uint16_t>>    Vec4u16_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<int32_t>>     Vec4s32_Type;
typedef Numeric_Type_Template<Vec4_Numeric_Traits<uint32_t>>    Vec4u32_Type;

}

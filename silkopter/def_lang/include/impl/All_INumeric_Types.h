#pragma once

#include "INumeric_Type_Template.h"
#include "Math.h"

namespace ts
{

template<typename Traits> class INumeric_Value_Template;
template<typename Traits> class INumeric_Type_Template;

template<class T> struct Scalar_INumeric_Traits
{
    typedef T fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 1;
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Scalar_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Scalar_INumeric_Traits<T>> type_interface;
};

template<class T> struct Vec2_INumeric_Traits
{
    typedef vec2<T> fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 2;
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Vec2_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Vec2_INumeric_Traits<T>> type_interface;
};

template<class T> struct Vec3_INumeric_Traits
{
    typedef vec3<T> fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 3;
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Vec3_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Vec3_INumeric_Traits<T>> type_interface;
};

template<class T> struct Vec4_INumeric_Traits
{
    typedef vec4<T> fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 4;
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Vec4_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Vec4_INumeric_Traits<T>> type_interface;
};

typedef INumeric_Type_Template<Scalar_INumeric_Traits<float>>     IFloat_Type;
typedef INumeric_Type_Template<Scalar_INumeric_Traits<double>>    IDouble_Type;
typedef INumeric_Type_Template<Scalar_INumeric_Traits<int64_t>>   IInt64_Type;

typedef INumeric_Type_Template<Vec2_INumeric_Traits<float>>       IVec2f_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<double>>      IVec2d_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<int8_t>>      IVec2s8_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<uint8_t>>     IVec2u8_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<int16_t>>     IVec2s16_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<uint16_t>>    IVec2u16_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<int32_t>>     IVec2s32_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<uint32_t>>    IVec2u32_Type;

typedef INumeric_Type_Template<Vec3_INumeric_Traits<float>>       IVec3f_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<double>>      IVec3d_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<int8_t>>      IVec3s8_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<uint8_t>>     IVec3u8_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<int16_t>>     IVec3s16_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<uint16_t>>    IVec3u16_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<int32_t>>     IVec3s32_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<uint32_t>>    IVec3u32_Type;

typedef INumeric_Type_Template<Vec4_INumeric_Traits<float>>       IVec4f_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<double>>      IVec4d_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<int8_t>>      IVec4s8_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<uint8_t>>     IVec4u8_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<int16_t>>     IVec4s16_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<uint16_t>>    IVec4u16_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<int32_t>>     IVec4s32_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<uint32_t>>    IVec4u32_Type;

}

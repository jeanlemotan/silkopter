#pragma once

#include "INumeric_Type_Template.h"
#include "Math.h"
#include <array>

namespace ts
{

template<typename Traits> class INumeric_Value_Template;
template<typename Traits> class INumeric_Type_Template;

template<class T> struct Scalar_INumeric_Traits
{
    typedef T fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 1;
    constexpr static std::array<const char*, component_count> component_names = { "x" };
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Scalar_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Scalar_INumeric_Traits<T>> type_interface;
};
template<class T> constexpr std::array<const char*, Scalar_INumeric_Traits<T>::component_count> Scalar_INumeric_Traits<T>::component_names;

template<class T> struct Vec2_INumeric_Traits
{
    typedef math::vec2<T> fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 2;
    constexpr static std::array<const char*, component_count> component_names = { "x", "y" };
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Vec2_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Vec2_INumeric_Traits<T>> type_interface;
};
template<class T> constexpr std::array<const char*, Vec2_INumeric_Traits<T>::component_count> Vec2_INumeric_Traits<T>::component_names;

template<class T> struct Vec3_INumeric_Traits
{
    typedef math::vec3<T> fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 3;
    constexpr static std::array<const char*, component_count> component_names = { "x", "y", "z" };
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Vec3_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Vec3_INumeric_Traits<T>> type_interface;
};
template<class T> constexpr std::array<const char*, Vec3_INumeric_Traits<T>::component_count> Vec3_INumeric_Traits<T>::component_names;

template<class T> struct Vec4_INumeric_Traits
{
    typedef math::vec4<T> fundamental_type;
    typedef T component_type;
    constexpr static size_t component_count = 4;
    constexpr static std::array<const char*, component_count> component_names = { "x", "y", "z", "w" };
    constexpr static T min_value = std::numeric_limits<T>::lowest();
    constexpr static T max_value = std::numeric_limits<T>::max();
    constexpr static bool supports_decimals_attribute = std::is_floating_point<T>::value;
    typedef Scalar_INumeric_Traits<component_type> component_interfaces_traits;
    typedef INumeric_Value_Template<Vec4_INumeric_Traits<T>> value_interface;
    typedef INumeric_Type_Template<Vec4_INumeric_Traits<T>> type_interface;
};
template<class T> constexpr std::array<const char*, Vec4_INumeric_Traits<T>::component_count> Vec4_INumeric_Traits<T>::component_names;

typedef INumeric_Type_Template<Scalar_INumeric_Traits<float>>     IFloat_Type;
typedef INumeric_Type_Template<Scalar_INumeric_Traits<double>>    IDouble_Type;
typedef INumeric_Type_Template<Scalar_INumeric_Traits<int64_t>>   IInt_Type;

typedef INumeric_Type_Template<Vec2_INumeric_Traits<float>>       IVec2f_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<double>>      IVec2d_Type;
typedef INumeric_Type_Template<Vec2_INumeric_Traits<int64_t>>     IVec2i_Type;

typedef INumeric_Type_Template<Vec3_INumeric_Traits<float>>       IVec3f_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<double>>      IVec3d_Type;
typedef INumeric_Type_Template<Vec3_INumeric_Traits<int64_t>>     IVec3i_Type;

typedef INumeric_Type_Template<Vec4_INumeric_Traits<float>>       IVec4f_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<double>>      IVec4d_Type;
typedef INumeric_Type_Template<Vec4_INumeric_Traits<int64_t>>     IVec4i_Type;

}

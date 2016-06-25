#pragma once

#include <stdint.h>
#include <sstream>
//#include "qmath.h"

namespace ts
{

template<typename T>
struct vec2
{
    vec2() = default;
    explicit vec2(T v) : x(v), y(v) {}
    vec2(T x, T y) : x(x), y(y) {}
    bool operator==(vec2<T> const& other) const { return x == other.x && y == other.y; }
    bool operator!=(vec2<T> const& other) const { return !operator==(other); }

    T x = 0;
    T y = 0;
};
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int64_t> vec2i;


template<typename T>
struct vec3
{
    vec3() = default;
    explicit vec3(T v) : x(v), y(v), z(v) {}
    vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    bool operator==(vec3<T> const& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(vec3<T> const& other) const { return !operator==(other); }

    T x = 0;
    T y = 0;
    T z = 0;
};
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<int64_t> vec3i;


template<typename T>
struct vec4
{
    vec4() = default;
    explicit vec4(T v) : x(v), y(v), z(v), w(v) {}
    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    bool operator==(vec4<T> const& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
    bool operator!=(vec4<T> const& other) const { return !operator==(other); }

    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;
};
typedef vec4<float> vec4f;
typedef vec4<double> vec4d;
typedef vec4<int64_t> vec4i;



namespace detail
{

template<typename T>
void set_component(T& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx == 0);
    value = component_value;
}
template<typename T>
T get_component(T const& value, size_t idx)
{
    TS_ASSERT(idx == 0);
    return value;
}
template<typename T>
void set_component(ts::vec2<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 2);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
}
template<typename T>
T get_component(ts::vec2<T> const& value, size_t idx)
{
    TS_ASSERT(idx < 2);
    if (idx == 0) return value.x;
    return value.y;
}
template<typename T>
void set_component(ts::vec3<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 3);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
}
template<typename T>
T get_component(ts::vec3<T> const& value, size_t idx)
{
    TS_ASSERT(idx < 3);
    if (idx == 0) return value.x;
    else if (idx == 1) return value.y;
    return value.z;
}
template<typename T>
void set_component(ts::vec4<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 4);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
    else if (idx == 4) value.w = component_value;
}
template<typename T>
T get_component(ts::vec4<T> const& value, size_t idx)
{
    TS_ASSERT(idx < 4);
    if (idx == 0) return value.x;
    else if (idx == 1) return value.y;
    else if (idx == 2) return value.z;
    return value.w;
}

template<typename T> std::string to_string(T const& value, size_t decimals)
{
    std::stringstream ss;
    ss.precision(decimals);
    ss << value;
    return ss.str();
}
template<typename T> std::string to_string(ts::vec2<T> const& value, size_t decimals) { return to_string(value.x, decimals) + ", " + to_string(value.y, decimals); }
template<typename T> std::string to_string(ts::vec3<T> const& value, size_t decimals) { return to_string(value.x, decimals) + ", " + to_string(value.y, decimals) + ", " + to_string(value.z, decimals); }
template<typename T> std::string to_string(ts::vec4<T> const& value, size_t decimals) { return to_string(value.x, decimals) + ", " + to_string(value.y, decimals) + ", " + to_string(value.z, decimals) + ", " + to_string(value.w, decimals); }

template<typename T> bool is_smaller(T const& value, T min) { return value < min; }
template<typename T> bool is_smaller(ts::vec2<T> const& value, T min) { return value.x < min || value.y < min; }
template<typename T> bool is_smaller(ts::vec2<T> const& value, ts::vec2<T> const& min) { return value.x < min.x || value.y < min.y; }
template<typename T> bool is_smaller(ts::vec3<T> const& value, T min) { return value.x < min || value.y < min || value.z < min; }
template<typename T> bool is_smaller(ts::vec3<T> const& value, ts::vec3<T> const& min) { return value.x < min.x || value.y < min.y || value.z < min.z; }
template<typename T> bool is_smaller(ts::vec4<T> const& value, T min) { return value.x < min || value.y < min || value.z < min || value.w < min; }
template<typename T> bool is_smaller(ts::vec4<T> const& value, ts::vec4<T> const& min) { return value.x < min.x || value.y < min.y || value.z < min.z || value.w < min.w; }

template<typename T> bool is_greater(T const& value, T max) { return value > max; }
template<typename T> bool is_greater(ts::vec2<T> const& value, T max) { return value.x > max || value.y > max; }
template<typename T> bool is_greater(ts::vec2<T> const& value, ts::vec2<T> const& max) { return value.x > max.x || value.y > max.y; }
template<typename T> bool is_greater(ts::vec3<T> const& value, T max) { return value.x > max || value.y > max || value.z > max; }
template<typename T> bool is_greater(ts::vec3<T> const& value, ts::vec3<T> const& max) { return value.x > max.x || value.y > max.y || value.z > max.z; }
template<typename T> bool is_greater(ts::vec4<T> const& value, T max) { return value.x > max || value.y > max || value.z > max || value.w > max; }
template<typename T> bool is_greater(ts::vec4<T> const& value, ts::vec4<T> const& max) { return value.x > max.x || value.y > max.y || value.z > max.z || value.w > max.w; }

}

}

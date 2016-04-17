#pragma once

#include <stdint.h>

namespace ts
{

template<class T> struct vec2
{
    T x, y;

    vec2() : x(0), y(0) {}
    explicit vec2(T v) : x(v), y(v) {}
    bool operator==(vec2<T> const& other) const { return x == other.x && y == other.y; }
};
template<class T> struct vec3
{
    T x, y, z;

    vec3() : x(0), y(0), z(0) {}
    explicit vec3(T v) : x(v), y(v), z(v) {}
    bool operator==(vec3<T> const& other) const { return x == other.x && y == other.y && z == other.z; }
};
template<class T> struct vec4
{
    T x, y, z, w;

    vec4() : x(0), y(0), z(0), w(0) {}
    explicit vec4(T v) : x(v), y(v), z(v), w(v) {}
    bool operator==(vec4<T> const& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
};

namespace detail
{

template<typename T>
void set_component(T& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx == 0);
    value = component_value;
}
template<typename T>
void set_component(vec2<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 2);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
}
template<typename T>
void set_component(vec3<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 3);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
}
template<typename T>
void set_component(vec4<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 4);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
    else if (idx == 4) value.w = component_value;
}

template<typename T> bool is_smaller(T const& value, T min) { return value < min; }
template<typename T> bool is_smaller(vec2<T> const& value, T min) { return value.x < min || value.y < min; }
template<typename T> bool is_smaller(vec2<T> const& value, vec2<T> const& min) { return value.x < min.x || value.y < min.y; }
template<typename T> bool is_smaller(vec3<T> const& value, T min) { return value.x < min || value.y < min || value.z < min; }
template<typename T> bool is_smaller(vec3<T> const& value, vec3<T> const& min) { return value.x < min.x || value.y < min.y || value.z < min.z; }
template<typename T> bool is_smaller(vec4<T> const& value, T min) { return value.x < min || value.y < min || value.z < min || value.w < min; }
template<typename T> bool is_smaller(vec4<T> const& value, vec4<T> const& min) { return value.x < min.x || value.y < min.y || value.z < min.z || value.w < min.w; }

template<typename T> bool is_greater(T const& value, T max) { return value > max; }
template<typename T> bool is_greater(vec2<T> const& value, T max) { return value.x > max || value.y > max; }
template<typename T> bool is_greater(vec2<T> const& value, vec2<T> const& max) { return value.x > max.x || value.y > max.y; }
template<typename T> bool is_greater(vec3<T> const& value, T max) { return value.x > max || value.y > max || value.z > max; }
template<typename T> bool is_greater(vec3<T> const& value, vec3<T> const& max) { return value.x > max.x || value.y > max.y || value.z > max.z; }
template<typename T> bool is_greater(vec4<T> const& value, T max) { return value.x > max || value.y > max || value.z > max || value.w > max; }
template<typename T> bool is_greater(vec4<T> const& value, vec4<T> const& max) { return value.x > max.x || value.y > max.y || value.z > max.z || value.w > max.w; }

}

}

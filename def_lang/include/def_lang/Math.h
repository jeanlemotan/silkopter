#pragma once

#include <stdint.h>
#include "qmath.h"

namespace ts
{

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
void set_component(math::vec2<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 2);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
}
template<typename T>
T get_component(math::vec2<T> const& value, size_t idx)
{
    TS_ASSERT(idx < 2);
    if (idx == 0) return value.x;
    return value.y;
}
template<typename T>
void set_component(math::vec3<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 3);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
}
template<typename T>
T get_component(math::vec3<T> const& value, size_t idx)
{
    TS_ASSERT(idx < 3);
    if (idx == 0) return value.x;
    else if (idx == 1) return value.y;
    return value.z;
}
template<typename T>
void set_component(math::vec4<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 4);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
    else if (idx == 4) value.w = component_value;
}
template<typename T>
T get_component(math::vec4<T> const& value, size_t idx)
{
    TS_ASSERT(idx < 4);
    if (idx == 0) return value.x;
    else if (idx == 1) return value.y;
    else if (idx == 2) return value.z;
    return value.w;
}

template<typename T> bool is_smaller(T const& value, T min) { return value < min; }
template<typename T> bool is_smaller(math::vec2<T> const& value, T min) { return value.x < min || value.y < min; }
template<typename T> bool is_smaller(math::vec2<T> const& value, math::vec2<T> const& min) { return value.x < min.x || value.y < min.y; }
template<typename T> bool is_smaller(math::vec3<T> const& value, T min) { return value.x < min || value.y < min || value.z < min; }
template<typename T> bool is_smaller(math::vec3<T> const& value, math::vec3<T> const& min) { return value.x < min.x || value.y < min.y || value.z < min.z; }
template<typename T> bool is_smaller(math::vec4<T> const& value, T min) { return value.x < min || value.y < min || value.z < min || value.w < min; }
template<typename T> bool is_smaller(math::vec4<T> const& value, math::vec4<T> const& min) { return value.x < min.x || value.y < min.y || value.z < min.z || value.w < min.w; }

template<typename T> bool is_greater(T const& value, T max) { return value > max; }
template<typename T> bool is_greater(math::vec2<T> const& value, T max) { return value.x > max || value.y > max; }
template<typename T> bool is_greater(math::vec2<T> const& value, math::vec2<T> const& max) { return value.x > max.x || value.y > max.y; }
template<typename T> bool is_greater(math::vec3<T> const& value, T max) { return value.x > max || value.y > max || value.z > max; }
template<typename T> bool is_greater(math::vec3<T> const& value, math::vec3<T> const& max) { return value.x > max.x || value.y > max.y || value.z > max.z; }
template<typename T> bool is_greater(math::vec4<T> const& value, T max) { return value.x > max || value.y > max || value.z > max || value.w > max; }
template<typename T> bool is_greater(math::vec4<T> const& value, math::vec4<T> const& max) { return value.x > max.x || value.y > max.y || value.z > max.z || value.w > max.w; }

}

}

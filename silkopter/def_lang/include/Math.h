#pragma once

#include <stdint.h>

namespace ts
{

template<class T> struct vec2
{
    T x, y;

    bool operator==(vec2<T> const& other) const { return x == other.x && y == other.y; }
};
template<class T> struct vec3
{
    T x, y, z;

    bool operator==(vec3<T> const& other) const { return x == other.x && y == other.y && z == other.z; }
};
template<class T> struct vec4
{
    T x, y, z, w;

    bool operator==(vec4<T> const& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
};

}

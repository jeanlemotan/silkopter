#pragma once

#include <memory>

namespace ts
{

class IValue;

class IInitializer
{
public:
    virtual ~IInitializer() = default;

    virtual auto get_initializer_value() const -> IValue const* = 0;

    template<typename T>
    auto get_specialized_initializer_value() const -> T const*;
};

}

#include "IInitializer.inl"

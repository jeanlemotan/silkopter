#pragma once

#include <memory>

namespace ts
{

class IValue;

class IInitializer
{
public:
    virtual ~IInitializer() = default;

    virtual IValue const* get_initializer_value() const = 0;

    template<typename T>
    T const* get_specialized_initializer_value() const;
};

}

#include "impl/IInitializer.inl"

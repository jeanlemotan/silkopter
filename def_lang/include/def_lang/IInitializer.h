#pragma once

#include <memory>

namespace ts
{

class IValue;

class IInitializer
{
public:
    virtual ~IInitializer() = default;

    virtual std::shared_ptr<const IValue> get_initializer_value() const = 0;

    template<typename T>
    std::shared_ptr<const T> get_specialized_initializer_value() const;
};

}

#include "impl/IInitializer.inl"

#pragma once

#include <memory>

namespace ts
{

class IValue;

class IInitializer
{
public:
    virtual ~IInitializer() = default;
};

}

#include "impl/IInitializer.inl"

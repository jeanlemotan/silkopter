#pragma once

#include <string>
#include "Result.h"

namespace ts
{

class IInitializer;

class IAttribute
{
public:
    virtual ~IAttribute() = default;

    virtual auto get_name() const -> std::string = 0;
};

}

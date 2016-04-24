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

    virtual std::string get_name() const = 0;
};

}

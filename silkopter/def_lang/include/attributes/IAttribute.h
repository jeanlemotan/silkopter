#pragma once

#include <string>

namespace ts
{

class IAttribute
{
public:
    virtual ~IAttribute() = default;

    virtual auto get_name() const -> std::string = 0;
};

}

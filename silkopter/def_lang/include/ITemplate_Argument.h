#pragma once

#include <string>

namespace ts
{

class ITemplate_Argument
{
public:
    virtual ~ITemplate_Argument() = default;

    virtual auto get_template_instantiation_string() const -> std::string = 0;
};

}

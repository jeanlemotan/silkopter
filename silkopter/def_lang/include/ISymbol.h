#pragma once

#include <string>

namespace ts
{

class ISymbol
{
public:
    virtual ~ISymbol() = default;

    virtual std::string const& get_name() const = 0;
};

}

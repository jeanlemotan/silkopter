#pragma once

#include <string>

namespace ts
{

class ISymbol
{
public:
    virtual ~ISymbol() = default;

    virtual auto get_name() const -> std::string const& = 0;
};

}

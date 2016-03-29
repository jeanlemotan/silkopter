#pragma once

#include "IDeclaration_Scope.h"

namespace ts
{

class ISymbol
{
public:
    virtual ~ISymbol() = default;

    virtual std::string const& get_name() const = 0;
    //virtual Declaration_Scope const& get_declaration_scope() const = 0;
};

}

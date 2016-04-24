#pragma once

#include <string>
#include "Symbol_Path.h"

namespace ts
{

class IDeclaration_Scope;

class ISymbol
{
public:
    virtual ~ISymbol() = default;

    virtual std::string const& get_name() const = 0;
    virtual Symbol_Path get_symbol_path() const = 0;

    virtual IDeclaration_Scope const* get_parent_scope() const = 0;
    virtual IDeclaration_Scope* get_parent_scope() = 0;

//protected:
    virtual void set_parent_scope(IDeclaration_Scope* declaration_scope) = 0;
};

}

#pragma once

#include <memory>
#include <string>
#include "ISymbol.h"

namespace ts
{

class IDeclaration_Scope;

class Symbol_EP : virtual public ISymbol
{
public:

    Symbol_EP(std::string const& name);

    std::string const& get_name() const override;

private:
    std::string m_name;
};

}

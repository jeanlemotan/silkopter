#pragma once

#include "IDeclaration_Scope.h"
#include "ISymbol.h"

namespace ast
{

class INamespace : virtual public ISymbol, virtual public IDeclaration_Scope
{
public:
};

}

#pragma once

#include "IDeclaration_Scope.h"
#include "types/IType.h"

namespace ts
{

class Type_System: virtual public IDeclaration_Scope
{
public:

    void add_type(Declaration_Scope const& scope, std::unique_ptr<IType> type);

};

}

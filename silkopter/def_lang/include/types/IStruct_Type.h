#pragma once

#include "IDeclaration_Scope.h"
#include "IMember_Def_Container.h"
#include "types/IType.h"

namespace ts
{

class IStruct_Value;

class IStruct_Type: virtual public IType, virtual public IDeclaration_Scope, virtual public IMember_Def_Container
{
public:

    virtual auto create_specialized_value() const -> std::unique_ptr<IStruct_Value>;

};

}

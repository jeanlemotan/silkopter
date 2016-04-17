#pragma once

#include "IDeclaration_Scope.h"
#include "IMember_Def_Container.h"
#include "IType.h"

namespace ts
{

class IStruct_Value;

class IStruct_Type: virtual public IType, virtual public IDeclaration_Scope, virtual public IMember_Def_Container
{
public:
    virtual ~IStruct_Type() = default;

    typedef IStruct_Value value_type;

    virtual std::unique_ptr<IStruct_Value> create_specialized_value() const = 0;

};

}

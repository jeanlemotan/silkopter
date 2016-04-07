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
    typedef IStruct_Value value_type;

    virtual auto get_specialized_default_value() const -> std::shared_ptr<const value_type> = 0;
    virtual auto create_specialized_value() const -> std::unique_ptr<IStruct_Value> = 0;

};

}

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

    virtual std::shared_ptr<IStruct_Value> create_specialized_value() const = 0;

    virtual std::shared_ptr<const IStruct_Type> get_base_struct() const = 0;
    virtual bool is_base_of(IStruct_Type const& type) const = 0;
};

}

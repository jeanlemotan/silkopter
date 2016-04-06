#pragma once

#include "types/IStruct_Type.h"
#include "impl/Declaration_Scope_Impl.h"
#include "impl/Symbol_Impl.h"
#include "impl/Member_Def_Container_Impl.h"
#include "impl/Attribute_Container_Impl.h"

namespace ts
{

class Struct_Type final: virtual public IStruct_Type, public Symbol_Impl, public Declaration_Scope_Impl, public Member_Def_Container_Impl, public Attribute_Container_Impl
{
public:

    Struct_Type(std::string const& name);

    auto clone(std::string const& name) const -> std::unique_ptr<IType> override;

    auto get_default_value() const -> std::shared_ptr<const IValue> override;
    auto create_value() const -> std::unique_ptr<IValue> override;
    auto create_specialized_value() const -> std::unique_ptr<IStruct_Value> override;

};

}

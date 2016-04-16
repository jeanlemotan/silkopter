#pragma once

#include "IStruct_Type.h"
#include "ep/Declaration_Scope_EP.h"
#include "ep/Symbol_EP.h"
#include "ep/Member_Def_Container_EP.h"
#include "ep/Attribute_Container_EP.h"

namespace ts
{

class Struct_Type final: virtual public IStruct_Type, public Symbol_EP, public Declaration_Scope_EP, public Member_Def_Container_EP, public Attribute_Container_EP
{
public:
    typedef IStruct_Value value_type;

    Struct_Type(std::string const& name);

    auto clone(std::string const& name) const -> std::unique_ptr<IType> override;

    auto get_template_instantiation_string() const -> std::string override;

    auto create_value() const -> std::unique_ptr<IValue> override;
    auto create_specialized_value() const -> std::unique_ptr<IStruct_Value> override;

protected:
    auto validate_attribute(IAttribute const& attribute) -> Result<void> override;
};

}

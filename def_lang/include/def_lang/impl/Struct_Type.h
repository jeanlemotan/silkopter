#pragma once

#include "def_lang/IStruct_Type.h"
#include "def_lang/ep/Declaration_Scope_EP.h"
#include "def_lang/ep/Member_Def_Container_EP.h"
#include "def_lang/ep/Attribute_Container_EP.h"

namespace ts
{

class Struct_Type final: virtual public IStruct_Type, public Declaration_Scope_EP, public Member_Def_Container_EP, public Attribute_Container_EP, public std::enable_shared_from_this<Struct_Type>
{
public:
    typedef IStruct_Value value_type;

    Struct_Type(std::string const& name);

    std::shared_ptr<IType> clone(std::string const& name) const override;

    std::string get_template_instantiation_string() const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<IStruct_Value> create_specialized_value() const override;

protected:
    Result<void> validate_attribute(IAttribute const& attribute) override;
};

}

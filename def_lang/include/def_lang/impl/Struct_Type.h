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

    Struct_Type(std::string const& name, std::shared_ptr<const IStruct_Type> parent);
    Struct_Type(Struct_Type const& other, std::string const& name);

    std::string const& get_ui_name() const override;
    Symbol_Path get_native_type() const override;

    std::shared_ptr<IType> clone(std::string const& name) const override;
    std::shared_ptr<IType> alias(std::string const& name) const override;
    std::shared_ptr<const IType> get_aliased_type() const override;

    size_t get_first_noninhereted_member_def_index() const override;
    size_t get_noninherited_member_def_count() const override;
    std::shared_ptr<IMember_Def const> get_noninherited_member_def(size_t idx) const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<IStruct_Value> create_specialized_value() const override;

    std::shared_ptr<const IStruct_Type> get_base_struct() const override;
    bool is_base_of(IStruct_Type const& type) const override;

protected:
    Result<void> validate_symbol(std::shared_ptr<const ISymbol> symbol) override;
    Result<void> validate_attribute(IAttribute const& attribute) override;

private:
    void replicate_base_struct();

    std::shared_ptr<const IStruct_Type> m_base_struct;
    std::shared_ptr<const IType> m_aliased_type;
    std::string m_ui_name;
    Symbol_Path m_native_type;
    size_t m_first_noninherited_member_def_index = 0;
};

}

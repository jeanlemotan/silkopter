#pragma once

#include "def_lang/IVariant_Type.h"
#include "def_lang/ep/Attribute_Container_EP.h"
#include "def_lang/ep/Symbol_EP.h"

namespace ts
{

class Variant_Type final: virtual public IVariant_Type, public Symbol_EP, public Attribute_Container_EP, public std::enable_shared_from_this<Variant_Type>
{
public:
    typedef IVariant_Value value_type;

    Variant_Type(std::string const& name);
    Variant_Type(Variant_Type const& other, std::string const& name);

    Result<void> init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) override;

    std::shared_ptr<IType> clone(std::string const& name) const override;

    std::string const& get_ui_name() const override;
    Symbol_Path get_native_type() const override;

    std::string get_template_instantiation_string() const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<IVariant_Value> create_specialized_value() const override;

    size_t get_inner_type_count() const override;
    std::shared_ptr<const IType> get_inner_type(size_t idx) const override;
    boost::optional<size_t> find_inner_type_idx(std::shared_ptr<const IType> type) const override;

protected:
    Result<void> validate_attribute(IAttribute const& attribute) override;

private:
    std::vector<std::shared_ptr<const IType>> m_inner_types;
    std::string m_ui_name;
    Symbol_Path m_native_type;
};

}

#pragma once

#include "IIntegral_Type_Template.h"
#include "impl/Symbol_Impl.h"
#include "impl/Attribute_Container_Impl.h"
#include "values/All_IIntegral_Values.h"

namespace ts
{

template<typename Traits> class IIntegral_Value_Template;


template<typename Traits>
class Integral_Type_Template final : virtual public IIntegral_Type_Template<Traits>, public Symbol_Impl, public Attribute_Container_Impl
{
public:
    typedef Traits traits;
    typedef IIntegral_Value_Template<Traits> value_type;
    typedef typename Traits::fundamental_type fundamental_type;

    Integral_Type_Template(std::string const& name);

    auto clone(std::string const& name) const -> std::unique_ptr<IType> override;

    auto get_template_instantiation_string() const -> std::string override;

    auto get_default_value() const -> std::shared_ptr<const IValue> override;
    auto create_value() const -> std::unique_ptr<IValue> override;

    auto get_specialized_default_value() const -> std::shared_ptr<const value_type> override;
    auto create_specialized_value() const -> std::unique_ptr<value_type> override;
};

}

#include "Integral_Type_Template.inl"

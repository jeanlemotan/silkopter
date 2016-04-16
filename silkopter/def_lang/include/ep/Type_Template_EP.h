#pragma once

#include <memory>
#include "IValue.h"
#include "ep/Symbol_EP.h"
#include "ep/Attribute_Container_EP.h"


namespace ts
{

template<typename Traits>
class Type_Template_EP : public virtual Traits::type_interface, public Symbol_EP, public Attribute_Container_EP
{
public:
    typedef typename Traits::value_interface        value_interface;
    typedef typename Traits::value_implementation   value_implementation;
    typedef typename Traits::type_interface         type_interface;
    typedef typename Traits::type_implementation    type_implementation;
    typedef typename Traits::fundamental_type       fundamental_type;

    Type_Template_EP(std::string const& name);

    auto clone(std::string const& name) const -> std::unique_ptr<IType> override;

    auto get_template_instantiation_string() const -> std::string override;

    auto create_value() const -> std::unique_ptr<IValue> override;
    auto create_specialized_value() const -> std::unique_ptr<value_interface> override;

private:
};

}

#include "Type_Template_EP.inl"

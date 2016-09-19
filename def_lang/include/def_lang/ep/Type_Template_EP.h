#pragma once

#include <memory>
#include "def_lang/IValue.h"
#include "def_lang/IString_Value.h"
#include "def_lang/ep/Symbol_EP.h"
#include "def_lang/ep/Attribute_Container_EP.h"
#include "def_lang/ILiteral_Attribute.h"

namespace ts
{

template<typename Traits>
class Type_Template_EP : public virtual Traits::type_interface, public Symbol_EP, public Attribute_Container_EP, public std::enable_shared_from_this<Type_Template_EP<Traits>>
{
public:
    typedef typename Traits::value_interface        value_interface;
    typedef typename Traits::value_implementation   value_implementation;
    typedef typename Traits::type_interface         type_interface;
    typedef typename Traits::type_implementation    type_implementation;
    typedef typename Traits::fundamental_type       fundamental_type;

    Type_Template_EP(std::string const& name);
    Type_Template_EP(Type_Template_EP<Traits> const& other, std::string const& name);

    std::string const& get_ui_name() const override;
    Symbol_Path get_native_type() const override;

    std::shared_ptr<IType> clone(std::string const& name) const override;
    std::shared_ptr<IType> alias(std::string const& name) const override;

    std::shared_ptr<const IType> get_aliased_type() const override;

    std::shared_ptr<IValue> create_value() const override;
    std::shared_ptr<value_interface> create_specialized_value() const override;

protected:
    virtual Result<void> validate_attribute_impl(IAttribute const& attribute) = 0;

private:
    Result<void> validate_attribute(IAttribute const& attribute) final override;

    std::shared_ptr<const IType> m_aliased_type;
    std::string m_ui_name;
    Symbol_Path m_native_type;
};

}

#include "Type_Template_EP.inl"

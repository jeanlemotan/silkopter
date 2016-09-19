namespace ts
{

template<typename Traits>
Type_Template_EP<Traits>::Type_Template_EP(std::string const& name)
    : Symbol_EP(name)
    , m_ui_name(name)
{
}
template<typename Traits>
Type_Template_EP<Traits>::Type_Template_EP(Type_Template_EP<Traits> const& other, std::string const& name)
    : Symbol_EP(other, name)
    , Attribute_Container_EP(other)
    , m_ui_name(name)
    , m_native_type(other.m_native_type)
{
}

template<typename Traits>
std::string const& Type_Template_EP<Traits>::get_ui_name() const
{
    return m_ui_name.empty() ? get_name() : m_ui_name;
}
template<typename Traits>
Symbol_Path Type_Template_EP<Traits>::get_native_type() const
{
    return m_native_type;
}

template<typename Traits>
std::shared_ptr<IType> Type_Template_EP<Traits>::clone(std::string const& name) const
{
    return std::make_shared<type_implementation>(static_cast<const type_implementation&>(*this), name);
}

template<typename Traits>
std::shared_ptr<IType> Type_Template_EP<Traits>::alias(std::string const& name) const
{
    std::shared_ptr<type_implementation> alias = std::make_shared<type_implementation>(static_cast<const type_implementation&>(*this), name);
    alias->m_native_type = Symbol_Path(); //clear the native type as this is an alias
    alias->m_aliased_type = this->shared_from_this();
    return alias;
}

template<typename Traits>
std::shared_ptr<const IType> Type_Template_EP<Traits>::get_aliased_type() const
{
    return m_aliased_type;
}

template<typename Traits>
std::shared_ptr<IValue> Type_Template_EP<Traits>::create_value() const
{
    return create_specialized_value();
}

template<typename Traits>
std::shared_ptr<typename Traits::value_interface> Type_Template_EP<Traits>::create_specialized_value() const
{
    return std::make_shared<value_implementation>(this->shared_from_this());
}

template<typename Traits>
Result<void> Type_Template_EP<Traits>::validate_attribute(IAttribute const& attribute)
{
    if (attribute.get_name() == "ui_name")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IString_Value const* value = dynamic_cast<IString_Value const*>(&att->get_value());
            if (!value)
            {
                return Error("Attribute '" + attribute.get_name() + "' has to be a string.");
            }
            m_ui_name = value->get_value();
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a string literal.");
        }
    }
    else if (attribute.get_name() == "native_type")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IString_Value const* value = dynamic_cast<IString_Value const*>(&att->get_value());
            if (!value)
            {
                return Error("Attribute '" + attribute.get_name() + "' has to be a string.");
            }
            m_native_type = value->get_value();
            return success;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a string literal.");
        }
    }
    else
    {
        return validate_attribute_impl(attribute);
    }
}


}

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
std::string Type_Template_EP<Traits>::get_template_instantiation_string() const
{
    return get_symbol_path().to_string();
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
    if (UI_Name_Attribute const* att = dynamic_cast<UI_Name_Attribute const*>(&attribute))
    {
        m_ui_name = att->get_ui_name();
        return success;
    }
    else if (Native_Type_Attribute const* att = dynamic_cast<Native_Type_Attribute const*>(&attribute))
    {
        m_native_type = att->get_native_type();
        return success;
    }
    else
    {
        return validate_attribute_impl(attribute);
    }
}


}

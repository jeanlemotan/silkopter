namespace ts
{

template<typename Traits>
Type_Template_EP<Traits>::Type_Template_EP(std::string const& name)
    : Symbol_EP(name)
{
}

template<typename Traits>
std::string Type_Template_EP<Traits>::get_template_instantiation_string() const
{
    return get_name();
}

template<typename Traits>
std::unique_ptr<IType> Type_Template_EP<Traits>::clone(std::string const& name) const
{
    return std::unique_ptr<IType>(new type_implementation(name));
}

template<typename Traits>
std::unique_ptr<IValue> Type_Template_EP<Traits>::create_value() const
{
    return create_specialized_value();
}

template<typename Traits>
std::unique_ptr<typename Traits::value_interface> Type_Template_EP<Traits>::create_specialized_value() const
{
    return std::unique_ptr<value_interface>(new value_implementation(this->shared_from_this()));
}

}

namespace ts
{

template<typename Traits>
Type_Template_EP<Traits>::Type_Template_EP(std::string const& name)
    : Symbol_EP(name)
{
}

template<typename Traits>
auto Type_Template_EP<Traits>::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

template<typename Traits>
auto Type_Template_EP<Traits>::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new type_implementation(name));
}

template<typename Traits>
auto Type_Template_EP<Traits>::create_value() const -> std::unique_ptr<IValue>
{
    return create_specialized_value();
}

template<typename Traits>
auto Type_Template_EP<Traits>::create_specialized_value() const -> std::unique_ptr<value_interface>
{
    return std::unique_ptr<value_interface>(new value_implementation(*this));
}

}

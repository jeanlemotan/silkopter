namespace ts
{

template<typename Traits>
Real_Type_Template<Traits>::Real_Type_Template(std::string const& name)
    : Symbol_Impl(name)
{

}

template<typename Traits>
auto Real_Type_Template<Traits>::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Real_Type_Template<Traits>(name));
}

template<typename Traits>
auto Real_Type_Template<Traits>::get_template_instantiation_string() const -> std::string
{
    return get_name();
}

template<typename Traits>
auto Real_Type_Template<Traits>::get_default_value() const -> std::shared_ptr<const IValue>
{
    return nullptr;
}

template<typename Traits>
auto Real_Type_Template<Traits>::create_value() const -> std::unique_ptr<IValue>
{
    return nullptr;
}

template<typename Traits>
auto Real_Type_Template<Traits>::get_specialized_default_value() const -> std::shared_ptr<const value_type>
{
    return nullptr;
}

template<typename Traits>
auto Real_Type_Template<Traits>::create_specialized_value() const -> std::unique_ptr<value_type>
{
    return nullptr;
}


}

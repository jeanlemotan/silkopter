namespace ts
{

template<typename Traits>
Real_Type<Traits>::Real_Type(std::string const& name)
    : Symbol_Impl(name)
{

}

template<typename Traits>
auto Real_Type<Traits>::clone(std::string const& name) const -> std::unique_ptr<IType>
{
    return std::unique_ptr<IType>(new Real_Type<Traits>(name));
}

template<typename Traits>
auto Real_Type<Traits>::get_default_value() const -> std::shared_ptr<const IValue>
{
    return nullptr;
}

template<typename Traits>
auto Real_Type<Traits>::create_value() const -> std::unique_ptr<IValue>
{
    return nullptr;
}

template<typename Traits>
auto Real_Type<Traits>::get_specialized_default_value() const -> std::shared_ptr<const value_type>
{
    return nullptr;
}

template<typename Traits>
auto Real_Type<Traits>::create_specialized_value() const -> std::unique_ptr<value_type>
{
    return nullptr;
}


}

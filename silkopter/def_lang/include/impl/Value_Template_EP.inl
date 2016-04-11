namespace ts
{

template<typename Traits>
Value_Template_EP<Traits>::Value_Template_EP(type_interface const& type)
    : m_type(type)
    , m_value()
{
}

template<typename Traits>
auto Value_Template_EP<Traits>::is_equal(IValue const& other) const -> Result<bool>
{
    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("incompatible types");
    }

    return get_value() == v->get_value();
}

template<typename Traits>
auto Value_Template_EP<Traits>::copy_assign(IValue const& other) -> Result<void>
{
    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("incompatible types");
    }

    return set_value(v->get_value());
}

//template<typename Traits>
//auto Value_Template_EP<Traits>::copy_assign(IInitializer const& initializer) -> Result<void>
//{
//    IValue const* v = dynamic_cast<value_interface>(&other);
//    if (!v)
//    {
//        return Error("incompatible types");
//    }

//    return set_value(v->get_value());
//}

template<typename Traits>
auto Value_Template_EP<Traits>::clone() const -> std::unique_ptr<IValue>
{
    std::unique_ptr<value_interface> v = get_specialized_type().create_specialized_value();
    auto result = v->copy_assign(*this);
    TS_ASSERT(result == success);
    return std::move(v);
}

template<typename Traits>
auto Value_Template_EP<Traits>::get_type() const -> IType const&
{
    return m_type;
}

template<typename Traits>
auto Value_Template_EP<Traits>::parse_from_ui_string(std::string const& str) -> Result<void>
{
    return Error("Not implemented");
}
template<typename Traits>
auto Value_Template_EP<Traits>::get_ui_string() const -> std::string
{
    return "Not implemented";
}

template<typename Traits>
auto Value_Template_EP<Traits>::select(Value_Selector const& selector) const -> IValue const*
{
    return nullptr;
}
template<typename Traits>
auto Value_Template_EP<Traits>::select(Value_Selector const& selector) -> IValue*
{
    return nullptr;
}

template<typename Traits>
auto Value_Template_EP<Traits>::get_specialized_type() const -> type_interface const&
{
    return m_type;
}

template<typename Traits>
auto Value_Template_EP<Traits>::set_value(fundamental_type value) -> Result<void>
{
    m_value = value;
    return ts::success;
}
template<typename Traits>
auto Value_Template_EP<Traits>::get_value() const -> fundamental_type
{
    return m_value;
}

}

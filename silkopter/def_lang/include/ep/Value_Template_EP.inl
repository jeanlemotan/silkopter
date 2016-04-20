namespace ts
{

template<typename Traits>
Value_Template_EP<Traits>::Value_Template_EP(std::shared_ptr<type_interface const> type)
    : m_type(type)
    , m_value()
{
}

template<typename Traits>
Result<bool> Value_Template_EP<Traits>::is_equal(IValue const& other) const
{
    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    return get_value() == v->get_value();
}

template<typename Traits>
Result<void> Value_Template_EP<Traits>::copy_assign(IValue const& other)
{
    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    return set_value(v->get_value());
}

//template<typename Traits>
//Result<void> Value_Template_EP<Traits>::copy_assign(IInitializer const& initializer)
//{
//    IValue const* v = dynamic_cast<value_interface>(&other);
//    if (!v)
//    {
//        return Error("incompatible values");
//    }

//    return set_value(v->get_value());
//}

//template<typename Traits>
//std::shared_ptr<IValue> Value_Template_EP<Traits>::clone() const
//{
//    std::shared_ptr<value_interface> v = get_specialized_type().create_specialized_value();
//    auto result = v->copy_assign(*this);
//    TS_ASSERT(result == success);
//    return std::move(v);
////    return std::shared_ptr<IValue>(new Value_Template_EP<Traits>(*this));
//}

template<typename Traits>
std::shared_ptr<IType const> Value_Template_EP<Traits>::get_type() const
{
    return m_type;
}

template<typename Traits>
Result<void> Value_Template_EP<Traits>::parse_from_ui_string(std::string const& str)
{
    return Error("Not Supported");
}
template<typename Traits>
Result<std::string> Value_Template_EP<Traits>::get_ui_string() const
{
    return Error("Not Supported");
}

template<typename Traits>
std::shared_ptr<const IValue> Value_Template_EP<Traits>::select(Value_Selector&& selector) const
{
    return nullptr;
}
template<typename Traits>
std::shared_ptr<IValue> Value_Template_EP<Traits>::select(Value_Selector&& selector)
{
    return nullptr;
}

template<typename Traits>
std::shared_ptr<typename Traits::type_interface const> Value_Template_EP<Traits>::get_specialized_type() const
{
    return m_type;
}

template<typename Traits>
Result<void> Value_Template_EP<Traits>::set_value(fundamental_type value)
{
    m_value = value;
    return ts::success;
}
template<typename Traits>
typename Traits::fundamental_type Value_Template_EP<Traits>::get_value() const
{
    return m_value;
}

}

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
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed values");
    }
    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("Incompatible values");
    }

    return get_value() == v->get_value();
}

template<typename Traits>
bool Value_Template_EP<Traits>::is_constructed() const
{
    return m_is_constructed;
}

template<typename Traits>
void Value_Template_EP<Traits>::set_constructed(bool constructed)
{
    m_is_constructed = constructed;
}

template<typename Traits>
Result<void> Value_Template_EP<Traits>::copy_construct(IValue const& other)
{
    if (is_constructed())
    {
        TS_ASSERT(false);
        return Error("Already constructed value");
    }
    if (!other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    m_is_constructed = true;

    auto result = set_value(v->get_value());
    if (result != success)
    {
        m_is_constructed = false;
        return result;
    }

    return success;
}

template<typename Traits>
Result<void> Value_Template_EP<Traits>::copy_assign(IValue const& other)
{
    if (!is_constructed() || !other.is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed values");
    }

    value_interface const* v = dynamic_cast<const value_interface*>(&other);
    if (!v)
    {
        return Error("incompatible values");
    }

    return set_value(v->get_value());
}

template<typename Traits>
std::shared_ptr<IType const> Value_Template_EP<Traits>::get_type() const
{
    return m_type;
}

template<typename Traits>
std::shared_ptr<const IValue> Value_Template_EP<Traits>::select(Value_Selector&& selector) const
{
    TS_ASSERT(is_constructed());
    return nullptr;
}
template<typename Traits>
std::shared_ptr<IValue> Value_Template_EP<Traits>::select(Value_Selector&& selector)
{
    TS_ASSERT(is_constructed());
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
    TS_ASSERT(is_constructed());
    if (!is_constructed())
    {
        return Error("Unconstructed value");
    }
    if (this->m_value != value)
    {
        m_value = value;
        this->sig_value_changed();
    }
    return ts::success;
}
template<typename Traits>
typename Traits::fundamental_type Value_Template_EP<Traits>::get_value() const
{
    TS_ASSERT(is_constructed());
    return m_value;
}

}

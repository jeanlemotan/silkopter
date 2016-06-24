#pragma once

template<typename T>
Qualified_Value<T>::Qualified_Value(std::shared_ptr<value_type> mutable_value)
    : m_const_value(mutable_value)
    , m_mutable_value(mutable_value)
{
}

template<typename T>
Qualified_Value<T>::Qualified_Value(std::shared_ptr<const value_type> const_value)
    : m_const_value(const_value)
{
}

template<typename T>
template<typename U>
Qualified_Value<T>::Qualified_Value(Qualified_Value<U>& other)
    : m_const_value(std::dynamic_pointer_cast<const value_type>(other.get_const_value()))
    , m_mutable_value(std::dynamic_pointer_cast<value_type>(other.get_mutable_value()))
{
    static_assert((std::is_base_of<typename std::remove_const<T>::type, typename std::remove_const<U>::type>::value), "Can only upcast with this constructor");
}

template<typename T>
auto Qualified_Value<T>::get_const_value() const -> std::shared_ptr<const value_type>
{
    return m_const_value;
}

template<typename T>
auto Qualified_Value<T>::get_mutable_value() const -> std::shared_ptr<const value_type>
{
    return m_mutable_value;
}

template<typename T>
auto Qualified_Value<T>::get_mutable_value() -> std::shared_ptr<value_type>
{
    return m_mutable_value;
}

template<typename T>
bool Qualified_Value<T>::is_read_only() const
{
    return m_mutable_value == nullptr;
}


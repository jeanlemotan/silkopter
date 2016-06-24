#pragma once

template<typename T>
class Qualified_Value
{
public:
    typedef typename std::remove_cv<T>::type value_type;

    Qualified_Value(std::shared_ptr<value_type> mutable_value);
    Qualified_Value(std::shared_ptr<const value_type> const_value);

    Qualified_Value(const Qualified_Value<T>& other) = default;

    template<typename U>
    Qualified_Value(Qualified_Value<U>& other);

    std::shared_ptr<const value_type> get_const_value() const;
    std::shared_ptr<const value_type> get_mutable_value() const;
    std::shared_ptr<value_type> get_mutable_value();

    bool is_read_only() const;

protected:
    std::shared_ptr<const value_type> m_const_value;
    std::shared_ptr<value_type> m_mutable_value;
};

#include "details/Qualified_Value.inl"

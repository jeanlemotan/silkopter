namespace ts
{

template<typename Traits>
Numeric_Type_Template<Traits>::Numeric_Type_Template(std::string const& name)
    : Type_Template_EP<Traits>(name)
{

}

template<typename Traits>
typename Traits::fundamental_type const& Numeric_Type_Template<Traits>::get_min_value() const
{
    return m_min_value;
}

template<typename Traits>
typename Traits::fundamental_type const& Numeric_Type_Template<Traits>::get_max_value() const
{
    return m_max_value;
}

template<typename Traits>
size_t Numeric_Type_Template<Traits>::get_decimals() const
{
    return m_decimals;
}

template<typename Traits>
Result<void> Numeric_Type_Template<Traits>::validate_attribute(IAttribute const& attribute)
{
    if (Min_Attribute const* att = dynamic_cast<Min_Attribute const*>(&attribute))
    {
        typename Traits::value_interface const* v = dynamic_cast<typename Traits::value_interface const*>(&att->get_min_value());
        if (!v)
        {
            return Error("Min attribute is not of the correct type. Expected " + this->get_name() + ", got " + att->get_min_value().get_type()->get_name());
        }

        if (detail::is_smaller(v->get_value(), Traits::min_value))
        {
            return Error("Attribute min is too small. Minimum value is : " + std::to_string(Traits::min_value));
        }
        m_min_value = v->get_value();
    }
    else if (Max_Attribute const* att = dynamic_cast<Max_Attribute const*>(&attribute))
    {
        typename Traits::value_interface const* v = dynamic_cast<typename Traits::value_interface const*>(&att->get_max_value());
        if (!v)
        {
            return Error("Attribute max is not of the correct type. Expected " + this->get_name() + ", got " + att->get_max_value().get_type()->get_name());
        }

        if (detail::is_greater(v->get_value(), Traits::max_value))
        {
            return Error("Attribute max is too big. Maximum value is : " + std::to_string(Traits::max_value));
        }
        m_max_value = v->get_value();
    }
    else if (Decimals_Attribute const* att = dynamic_cast<Decimals_Attribute const*>(&attribute))
    {
        if (!Traits::supports_decimals_attribute)
        {
            return Error("Attribute decimals not supported on integral types");
        }
        m_decimals = att->get_decimals();
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }

    return success;
}

}

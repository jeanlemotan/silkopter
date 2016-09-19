namespace ts
{

template<typename Traits>
Numeric_Type_Template<Traits>::Numeric_Type_Template(std::string const& name)
    : Type_Template_EP<Traits>(name)
{
}

template<typename Traits>
Numeric_Type_Template<Traits>::Numeric_Type_Template(Numeric_Type_Template<Traits> const& other, std::string const& name)
    : Type_Template_EP<Traits>(other, name)
    , m_min_value(other.m_min_value)
    , m_max_value(other.m_max_value)
    , m_decimals(other.m_decimals)
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
bool Numeric_Type_Template<Traits>::is_hex() const
{
    return m_is_hex;
}

template<typename Traits>
Result<void> Numeric_Type_Template<Traits>::validate_attribute_impl(IAttribute const& attribute)
{
    if (attribute.get_name() == "min")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            typename Traits::value_interface const* v = dynamic_cast<typename Traits::value_interface const*>(&att->get_value());
            if (!v)
            {
                return Error("Min attribute is not of the correct type. Expected " + this->get_name() + ", got " + att->get_value().get_type()->get_name());
            }

            if (detail::is_smaller(v->get_value(), Traits::min_value))
            {
                return Error("Attribute min is too small. Minimum value is : " + std::to_string(Traits::min_value));
            }
            m_min_value = v->get_value();
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a literal.");
        }
    }
    else if (attribute.get_name() == "max")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            typename Traits::value_interface const* v = dynamic_cast<typename Traits::value_interface const*>(&att->get_value());
            if (!v)
            {
                return Error("Max attribute is not of the correct type. Expected " + this->get_name() + ", got " + att->get_value().get_type()->get_name());
            }

            if (detail::is_greater(v->get_value(), Traits::max_value))
            {
                return Error("Attribute max is too big. Maximum value is : " + std::to_string(Traits::max_value));
            }
            m_max_value = v->get_value();
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be a literal.");
        }
    }
    else if (attribute.get_name() == "decimals")
    {
        if (ILiteral_Attribute const* att = dynamic_cast<ILiteral_Attribute const*>(&attribute))
        {
            IInt_Value const* v = dynamic_cast<IInt_Value const*>(&att->get_value());
            if (!v)
            {
                return Error("Decimals attribute is not an int literal");
            }
            m_decimals = v->get_value();
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' has to be an int literal.");
        }
    }
    else if (attribute.get_name() == "hex")
    {
        if (dynamic_cast<IName_Attribute const*>(&attribute))
        {
            if (std::is_floating_point<typename Traits::component_type>::value ||
                    Traits::component_count > 1)
            {
                return Error("'" + attribute.get_name() + "' attribute is only for scalar integral types.");
            }

            m_is_hex = true;
        }
        else
        {
            return Error("Attribute '" + attribute.get_name() + "' doesn't support a value.");
        }
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }

    return success;
}

}

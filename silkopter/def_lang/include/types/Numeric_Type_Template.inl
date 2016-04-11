namespace ts
{

template<typename Traits>
Numeric_Type_Template<Traits>::Numeric_Type_Template(std::string const& name)
    : Type_Template_EP<Traits>(name)
{

}

namespace detail
{
template<typename T> bool is_smaller(T const& value, T min) { return value < min; }
template<typename T> bool is_smaller(vec2<T> const& value, T min) { return value.x < min || value.y < min; }
template<typename T> bool is_smaller(vec3<T> const& value, T min) { return value.x < min || value.y < min || value.z < min; }
template<typename T> bool is_smaller(vec4<T> const& value, T min) { return value.x < min || value.y < min || value.z < min || value.w < min; }

template<typename T> bool is_greater(T const& value, T min) { return value > min; }
template<typename T> bool is_greater(vec2<T> const& value, T min) { return value.x > min || value.y > min; }
template<typename T> bool is_greater(vec3<T> const& value, T min) { return value.x > min || value.y > min || value.z > min; }
template<typename T> bool is_greater(vec4<T> const& value, T min) { return value.x > min || value.y > min || value.z > min || value.w > min; }
}

template<typename Traits>
auto Numeric_Type_Template<Traits>::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    if (Min_Attribute const* att = dynamic_cast<Min_Attribute const*>(&attribute))
    {
        typename Traits::value_interface const* v = dynamic_cast<typename Traits::value_interface const*>(&att->get_min_value());
        if (!v)
        {
            return Error("Min attribute is not of the correct type. Expected " + this->get_name() + ", got " + att->get_min_value().get_type().get_name());
        }

        if (detail::is_smaller(v->get_value(), Traits::min_value))
        {
            return Error("Attribute min is too small. Minimum value is : " + std::to_string(Traits::min_value));
        }
        m_min_attribute = att;
    }
    else if (Max_Attribute const* att = dynamic_cast<Max_Attribute const*>(&attribute))
    {
        typename Traits::value_interface const* v = dynamic_cast<typename Traits::value_interface const*>(&att->get_max_value());
        if (!v)
        {
            return Error("Attribute max is not of the correct type. Expected " + this->get_name() + ", got " + att->get_max_value().get_type().get_name());
        }

        if (detail::is_greater(v->get_value(), Traits::max_value))
        {
            return Error("Attribute max is too big. Maximum value is : " + std::to_string(Traits::max_value));
        }
        m_max_attribute = att;
    }
    else if (Decimals_Attribute const* att = dynamic_cast<Decimals_Attribute const*>(&attribute))
    {
        if (!Traits::supports_decimals_attribute)
        {
            return Error("Attribute decimals not supported on integral types");
        }
        m_decimals_attribute = att;
    }
    else
    {
        return Error("Attribute " + attribute.get_name() + " not supported");
    }

    return success;
}

}

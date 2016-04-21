namespace ts
{
namespace mapper
{
namespace detail
{

template<typename SRC_T, typename DST_T> Result<void> getter(IValue const& src, DST_T& dst)
{
    if (SRC_T const* value = dynamic_cast<SRC_T const*>(&src))
    {
        dst = value->get_value();
        return success;
    }
    return Error("Cannot get " + src.get_type()->get_name());
}

template<typename T> Result<void> integral_getter(IValue const& src, T& dst)
{
    if (IInt64_Value const* value = dynamic_cast<IInt64_Value const*>(&src))
    {
        int64_t x = value->get_value();
        if (x < std::numeric_limits<T>::lowest() || x > std::numeric_limits<T>::max())
        {
            return Error("Value is out of bounds");
        }
        dst = static_cast<T>(x);
        return success;
    }
    return Error("Cannot get " + src.get_type()->get_name());
}

}

Result<void> get(IValue const& src, std::string& dst)
{
    return detail::getter<IString_Value, std::string>(src, dst);
}
Result<void> get(IValue const& src, float& dst)
{
    return detail::getter<IFloat_Value, float>(src, dst);
}
Result<void> get(IValue const& src, double& dst)
{
    return detail::getter<IDouble_Value, double>(src, dst);
}
Result<void> get(IValue const& src, uint8_t& dst)
{
    return detail::integral_getter(src, dst);
}
Result<void> get(IValue const& src, int8_t& dst)
{
    return detail::integral_getter(src, dst);
}
Result<void> get(IValue const& src, uint16_t& dst)
{
    return detail::integral_getter(src, dst);
}
Result<void> get(IValue const& src, int16_t& dst)
{
    return detail::integral_getter(src, dst);
}
Result<void> get(IValue const& src, uint32_t& dst)
{
    return detail::integral_getter(src, dst);
}
Result<void> get(IValue const& src, int32_t& dst)
{
    return detail::integral_getter(src, dst);
}
Result<void> get(IValue const& src, int64_t& dst)
{
    return detail::integral_getter(src, dst);
}



template<typename T>
Result<void> get(IValue const& src, Value_Selector&& selector, T& dst)
{
    if (selector.empty())
    {
        return get(src, dst);
    }

    std::string selector_str = selector.to_string(); //convert to string before calling select, as it will consume the selector
    std::shared_ptr<const IValue> v = src.select(std::move(selector));
    if (!v)
    {
        return Error("cannot find value '" + selector_str + "'");
    }

    return get(*v, dst);
}

}
}

namespace ts
{
namespace mapper
{
namespace detail
{

template<typename VALUE_T, typename NATIVE_T> Result<void> getter(IValue const& ivalue, NATIVE_T& native)
{
    if (VALUE_T const* value = dynamic_cast<VALUE_T const*>(&ivalue))
    {
        native = value->get_value();
        return success;
    }
    return Error("Cannot get " + ivalue.get_type()->get_name());
}
template<typename VALUE_T, typename NATIVE_T> Result<void> setter(IValue& ivalue, NATIVE_T const& native)
{
    if (VALUE_T* value = dynamic_cast<VALUE_T*>(&ivalue))
    {
        return value->set_value(native);
    }
    return Error("Cannot set " + ivalue.get_type()->get_name());
}

template<typename T> Result<void> integral_getter(IValue const& ivalue, T& native)
{
    if (IInt64_Value const* value = dynamic_cast<IInt64_Value const*>(&ivalue))
    {
        int64_t x = value->get_value();
        if (x < std::numeric_limits<T>::lowest() || x > std::numeric_limits<T>::max())
        {
            return Error("Value is out of bounds");
        }
        native = static_cast<T>(x);
        return success;
    }
    return Error("Cannot get " + ivalue.get_type()->get_name());
}

}

template<>
inline Result<void> get(IValue const& src, std::string& dst)
{
    return detail::getter<IString_Value, std::string>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, std::string const& src)
{
    return detail::setter<IString_Value, std::string>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, bool& dst)
{
    return detail::getter<IBool_Value, bool>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, bool const& src)
{
    return detail::setter<IBool_Value, bool>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, float& dst)
{
    return detail::getter<IFloat_Value, float>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, float const& src)
{
    return detail::setter<IFloat_Value, float>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, double& dst)
{
    return detail::getter<IDouble_Value, double>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, double const& src)
{
    return detail::setter<IDouble_Value, double>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, uint8_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, uint8_t const& src)
{
    return detail::setter<IInt64_Value, uint8_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, int8_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, int8_t const& src)
{
    return detail::setter<IInt64_Value, int8_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, uint16_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, uint16_t const& src)
{
    return detail::setter<IInt64_Value, uint16_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, int16_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, int16_t const& src)
{
    return detail::setter<IInt64_Value, int16_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, uint32_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, uint32_t const& src)
{
    return detail::setter<IInt64_Value, uint32_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, int32_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, int32_t const& src)
{
    return detail::setter<IInt64_Value, int32_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, int64_t& dst)
{
    return detail::integral_getter(src, dst);
}
template<>
inline Result<void> set(IValue& dst, int64_t const& src)
{
    return detail::setter<IInt64_Value, int64_t>(dst, src);
}

template<>
inline Result<void> get(IValue const& src, math::vec2f& dst)
{
    return detail::getter<IVec2f_Value, math::vec2f>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, math::vec2f const& src)
{
    return detail::setter<IVec2f_Value, math::vec2f>(dst, src);
}
template<>
inline Result<void> get(IValue const& src, math::vec3f& dst)
{
    return detail::getter<IVec3f_Value, math::vec3f>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, math::vec3f const& src)
{
    return detail::setter<IVec3f_Value, math::vec3f>(dst, src);
}
template<>
inline Result<void> get(IValue const& src, math::vec4f& dst)
{
    return detail::getter<IVec4f_Value, math::vec4f>(src, dst);
}
template<>
inline Result<void> set(IValue& dst, math::vec4f const& src)
{
    return detail::setter<IVec4f_Value, math::vec4f>(dst, src);
}


template<class T>
Result<void> get(IValue const& src, std::vector<T>& dst)
{
    if (IVector_Value const* value = dynamic_cast<IVector_Value const*>(&src))
    {
        dst.resize(value->get_value_count());
        for (size_t i = 0; i < value->get_value_count(); i++)
        {
            auto result = get(*value->get_value(i), dst[i]);
            if (result != ts::success)
            {
                return result;
            }
        }
        return success;
    }
    return Error("Cannot get vector from " + src.get_type()->get_name());
}
template<class T>
Result<void> set(IValue& dst, std::vector<T> const& src)
{
    if (IVector_Value* value = dynamic_cast<IVector_Value*>(&dst))
    {
        value->clear();
        for (size_t i = 0; i < src.size(); i++)
        {
            auto result = value->insert_default_value(value->get_value_count());
            if (result != ts::success)
            {
                return result;
            }

            result = set(*value->get_value(i), src[i]);
            if (result != ts::success)
            {
                return result;
            }
        }
        return success;
    }
    return Error("Cannot set vector to " + dst.get_type()->get_name());
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
template<typename T>
Result<void> set(IValue& dst, Value_Selector&& selector, T const& src)
{
    if (selector.empty())
    {
        return set(dst, src);
    }

    std::string selector_str = selector.to_string(); //convert to string before calling select, as it will consume the selector
    std::shared_ptr<IValue> v = dst.select(std::move(selector));
    if (!v)
    {
        return Error("cannot find value '" + selector_str + "'");
    }

    return set(*v, src);
}

}
}


namespace ts
{

template<typename Traits>
Numeric_Value_Template<Traits>::Numeric_Value_Template(typename Traits::type_interface const& type)
    : Value_Template_EP<Traits>(type)
{

}

namespace detail
{

template<typename T>
void set_component(T& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx == 0);
    value = component_value;
}
template<typename T>
void set_component(vec2<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 2);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
}
template<typename T>
void set_component(vec3<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 3);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
}
template<typename T>
void set_component(vec4<T>& value, T const& component_value, size_t idx)
{
    TS_ASSERT(idx < 4);
    if (idx == 0) value.x = component_value;
    else if (idx == 1) value.y = component_value;
    else if (idx == 2) value.z = component_value;
    else if (idx == 4) value.w = component_value;
}

}

template<typename Traits>
auto Numeric_Value_Template<Traits>::copy_assign(IInitializer const& initializer) -> Result<void>
{
    typename Traits::fundamental_type value;

    Numeric_Type_Template<typename Traits::component_implementation_traits> component_type("temp");
    Numeric_Value_Template<typename Traits::component_implementation_traits> component_value(component_type);

    if (IInitializer_List const* ilist = dynamic_cast<IInitializer_List const*>(&initializer))
    {
        if (ilist->get_initializer_count() != Traits::component_count)
        {
            return Error("Too many initializers. Expected " + std::to_string(Traits::component_count) + ", got " + std::to_string(ilist->get_initializer_count()));
        }

        for (size_t i = 0; i < ilist->get_initializer_count(); i++)
        {
            Result<void> result = component_value.copy_assign(ilist->get_initializer(i));
            if (result != success)
            {
                return Error("Cannot evaluate initializer list, item " + std::to_string(i) + ": " + result.error().what());
            }
            detail::set_component(value, component_value.get_value(), i);
        }

        return set_value(value);
    }
    else
    {
        if (Traits::component_count != 1)
        {
            return Error("Too few initializers. Expected " + std::to_string(Traits::component_count) + ", got 1");
        }

        IValue const* v = initializer.get_initializer_value();
        if (!v)
        {
            return Error("Cannot evaluate initializer");
        }
        Result<void> result = component_value.copy_assign(*v);
        if (result != success)
        {
            return result;
        }

        detail::set_component(value, component_value.get_value(), 0);

        return set_value(value);
    }

    return Error("Invalid initialization list");
}

}



namespace ts
{

template<typename Traits>
Numeric_Value_Template<Traits>::Numeric_Value_Template(std::shared_ptr<typename Traits::type_interface const> type)
    : Value_Template_EP<Traits>(type)
{
}

template<typename Traits>
Result<void> Numeric_Value_Template<Traits>::construct(IInitializer_List const& initializer_list)
{
    if (this->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Already constructed value");
    }

    typename Traits::fundamental_type value;

    if (initializer_list.get_initializer_count() == 0)
    {
        this->set_constructed(true);
        typename Traits::fundamental_type min_value = get_specialized_type()->get_min_value();
        for (size_t i = 0; i < Traits::component_count; i++)
        {
            detail::set_component(value, std::max(detail::get_component(min_value, i), (typename Traits::component_type)0), i);
        }
        return set_value(value);
    }

    if (initializer_list.get_initializer_count() > Traits::component_count)
    {
        return Error("Too many initializers. Expected " + std::to_string(Traits::component_count) + ", got " + std::to_string(initializer_list.get_initializer_count()));
    }
    if (initializer_list.get_initializer_count() < Traits::component_count)
    {
        return Error("Too few initializers. Expected " + std::to_string(Traits::component_count) + ", got " + std::to_string(initializer_list.get_initializer_count()));
    }

    if (Traits::component_count == 1)
    {
        //special case for single component values

        std::shared_ptr<const ILiteral_Initializer> initializer = std::dynamic_pointer_cast<const ILiteral_Initializer>(initializer_list.get_initializer(0));
        if (!initializer)
        {
            return Error("Cannot evaluate initializer list");
        }

        return copy_construct(*initializer->get_literal()->get_value());
    }

    //multi-component values

    auto component_type = std::make_shared<Numeric_Type_Template<typename Traits::component_implementation_traits>>("temp");

    for (size_t i = 0; i < initializer_list.get_initializer_count(); i++)
    {
        Initializer_List component_initializer_list({initializer_list.get_initializer(i)});
        Numeric_Value_Template<typename Traits::component_implementation_traits> component_value(component_type);
        Result<void> result = component_value.construct(component_initializer_list);
        if (result != success)
        {
            return Error("Cannot evaluate initializer list, item " + std::to_string(i) + ": " + result.error().what());
        }
        detail::set_component(value, component_value.get_value(), i);
    }

    if (detail::is_smaller(value, get_specialized_type()->get_min_value()))
    {
        return Error("Initializer is smaller than the min value");
    }
    if (detail::is_greater(value, get_specialized_type()->get_max_value()))
    {
        return Error("Initializer is greater than the max value");
    }

    this->set_constructed(true);

    auto result = set_value(value);
    if (result != success)
    {
        this->set_constructed(false);
        return result;
    }

    return result;
}

template<typename Traits>
Result<void> Numeric_Value_Template<Traits>::copy_assign(IInitializer_List const& initializer_list)
{
    if (!this->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    Numeric_Value_Template<Traits> value(this->get_specialized_type());
    auto result = value.construct(initializer_list);
    if (result != success)
    {
        return result;
    }

    return copy_assign(value);
//    }
//    else
//    {
//        if (Traits::component_count != 1)
//        {
//            return Error("Too few initializers. Expected " + std::to_string(Traits::component_count) + ", got 1");
//        }

//        std::shared_ptr<const IValue> v = initializer.get_initializer_value();
//        if (!v)
//        {
//            return Error("Cannot evaluate initializer");
//        }
//        Result<void> result = component_value.copy_assign(*v);
//        if (result != success)
//        {
//            return result;
//        }

//        detail::set_component(value, component_value.get_value(), 0);

//        if (detail::is_smaller(value, get_specialized_type()->get_min_value()))
//        {
//            return Error("Initializer is smaller than the min value");
//        }
//        if (detail::is_greater(value, get_specialized_type()->get_max_value()))
//        {
//            return Error("Initializer is greater than the max value");
//        }

//        return set_value(value);
//    }

//    return Error("Invalid initialization list");
}

template<typename Traits>
Result<serialization::Value> Numeric_Value_Template<Traits>::serialize() const
{
    if (!this->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (Traits::component_count == 1)
    {
        return serialization::Value(detail::get_component(this->get_value(), 0));
    }
    else
    {
        serialization::Value svalue(serialization::Value::Type::OBJECT);

        for (size_t i = 0; i < Traits::component_count; i++)
        {
            const char* name = Traits::component_names[i];
            svalue.add_object_member(name, serialization::Value(detail::get_component(this->get_value(), i)));
        }

        return std::move(svalue);
    }
}

template<typename Traits>
Result<void> Numeric_Value_Template<Traits>::deserialize(serialization::Value const& sz_value)
{
    if (!this->is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (Traits::component_count == 1)
    {
        typename Traits::fundamental_type value = this->get_value();

        if (Traits::supports_decimals_attribute)
        {
            if (!sz_value.is_number())
            {
                return Error("Expected number");
            }
            detail::set_component(value, static_cast<typename Traits::component_type>(sz_value.get_as_number()), 0);
        }
        else
        {
            if (!sz_value.is_integral_number())
            {
                return Error("Expected integral number");
            }
            detail::set_component(value, static_cast<typename Traits::component_type>(sz_value.get_as_integral_number()), 0);
        }

        return set_value(value);
    }
    else
    {
        if (!sz_value.is_object())
        {
            return Error("Expected object");
        }
        if (sz_value.get_object_member_count() != Traits::component_count)
        {
            return Error("Wrong number of components");
        }

        typename Traits::fundamental_type value = this->get_value();

        for (size_t i = 0; i < Traits::component_count; i++)
        {
            const char* name = Traits::component_names[i];
            if (sz_value.get_object_member_name(i) != name)
            {
                return Error("Unexpected member: '" + sz_value.get_object_member_name(i) + "'. Expected: '" + name + "'");
            }

            serialization::Value const& sz_member_value = sz_value.get_object_member_value(i);

            if (Traits::supports_decimals_attribute)
            {
                if (!sz_member_value.is_number())
                {
                    return Error("Expected number");
                }
                detail::set_component(value, static_cast<typename Traits::component_type>(sz_member_value.get_as_number()), i);
            }
            else
            {
                if (!sz_member_value.is_integral_number())
                {
                    return Error("Expected integral number");
                }
                detail::set_component(value, static_cast<typename Traits::component_type>(sz_member_value.get_as_integral_number()), i);
            }
        }

        return set_value(value);
    }
}

}


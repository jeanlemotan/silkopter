
namespace ts
{

template<typename Traits>
Numeric_Value_Template<Traits>::Numeric_Value_Template(std::shared_ptr<typename Traits::type_interface const> type)
    : Value_Template_EP<Traits>(type)
{

}

template<typename Traits>
Result<void> Numeric_Value_Template<Traits>::copy_assign(IInitializer const& initializer)
{
    typename Traits::fundamental_type value;

    auto component_type = std::make_shared<Numeric_Type_Template<typename Traits::component_implementation_traits>>("temp");
    Numeric_Value_Template<typename Traits::component_implementation_traits> component_value(component_type);

    if (IInitializer_List const* ilist = dynamic_cast<IInitializer_List const*>(&initializer))
    {
        if (ilist->get_initializer_count() > Traits::component_count)
        {
            return Error("Too many initializers. Expected " + std::to_string(Traits::component_count) + ", got " + std::to_string(ilist->get_initializer_count()));
        }
        if (ilist->get_initializer_count() < Traits::component_count)
        {
            return Error("Too few initializers. Expected " + std::to_string(Traits::component_count) + ", got " + std::to_string(ilist->get_initializer_count()));
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

        if (detail::is_smaller(value, get_specialized_type()->get_min_value()))
        {
            return Error("Initializer is smaller than the min value");
        }
        if (detail::is_greater(value, get_specialized_type()->get_max_value()))
        {
            return Error("Initializer is greater than the max value");
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

        if (detail::is_smaller(value, get_specialized_type()->get_min_value()))
        {
            return Error("Initializer is smaller than the min value");
        }
        if (detail::is_greater(value, get_specialized_type()->get_max_value()))
        {
            return Error("Initializer is greater than the max value");
        }

        return set_value(value);
    }

    return Error("Invalid initialization list");
}

template<typename Traits>
std::shared_ptr<IValue> Numeric_Value_Template<Traits>::clone() const
{
    return std::make_shared<Numeric_Value_Template<Traits>>(*this);
}

template<typename Traits>
Result<serialization::Value> Numeric_Value_Template<Traits>::serialize() const
{
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

}


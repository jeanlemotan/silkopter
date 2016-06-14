#include "def_lang/IInitializer_List.h"
#include "def_lang/ILiteral.h"
#include "def_lang/ILiteral_Initializer.h"
#include "def_lang/impl/Bool_Value.h"
#include "def_lang/Serialization.h"

namespace ts
{

Bool_Value::Bool_Value(std::shared_ptr<IBool_Type const> type)
    : Value_Template_EP<Bool_Traits>(type)
{
}

std::string Bool_Value::get_template_instantiation_string() const
{
    return get_value() ? "true" : "false";
}
Result<void> Bool_Value::construct(IInitializer_List const& initializer_list)
{
    if (is_constructed())
    {
        TS_ASSERT(false);
        return Error("Already constructed value");
    }

    if (initializer_list.get_initializer_count() == 0)
    {
        set_constructed(true);
        return success;
    }

    if (initializer_list.get_initializer_count() != 1)
    {
        return Error("Expected an initializer list with one element");
    }
    std::shared_ptr<const ILiteral_Initializer> initializer = std::dynamic_pointer_cast<const ILiteral_Initializer>(initializer_list.get_initializer(0));
    if (!initializer)
    {
        return Error("Cannot evaluate initializer list");
    }

    return copy_construct(*initializer->get_literal()->get_value());
}
Result<void> Bool_Value::copy_assign(IInitializer_List const& initializer_list)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (initializer_list.get_initializer_count() != 1)
    {
        return Error("Expected an initializer list with one element");
    }
    std::shared_ptr<const ILiteral_Initializer> initializer = std::dynamic_pointer_cast<const ILiteral_Initializer>(initializer_list.get_initializer(0));
    if (!initializer)
    {
        return Error("Cannot evaluate initializer list");
    }

    return copy_assign(*initializer->get_literal()->get_value());
}

Result<sz::Value> Bool_Value::serialize() const
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    return sz::Value(get_value());
}

Result<void> Bool_Value::deserialize(sz::Value const& sz_value)
{
    if (!is_constructed())
    {
        TS_ASSERT(false);
        return Error("Unconstructed value");
    }

    if (!sz_value.is_bool())
    {
        return Error("Expected bool value when deserializing");
    }
    return set_value(sz_value.get_as_bool());
}

}

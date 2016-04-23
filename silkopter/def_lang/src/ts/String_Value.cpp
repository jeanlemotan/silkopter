#include "impl/String_Value.h"
#include "IInitializer_List.h"
#include "ILiteral.h"
#include "Serialization.h"

namespace ts
{

String_Value::String_Value(std::shared_ptr<IString_Type const> type)
    : Value_Template_EP<String_Traits>(type)
{
}

Result<void> String_Value::copy_assign(IInitializer const& initializer)
{
    IValue const* value = initializer.get_initializer_value();
    if (!value)
    {
        return Error("Cannot evaluate initializer list");
    }

    return copy_assign(*value);
}

std::shared_ptr<IValue> String_Value::clone() const
{
    return std::make_shared<String_Value>(*this);
}

Result<serialization::Value> String_Value::serialize() const
{
    return serialization::Value(get_value());
}

Result<void> String_Value::deserialize(serialization::Value const& sz_value)
{
    if (sz_value.get_type() != serialization::Value::Type::STRING)
    {
        return Error("Expected string value when deserializing");
    }
    return set_value(sz_value.get_as_string());
}


}

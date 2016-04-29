#include "def_lang/IInitializer_List.h"
#include "def_lang/ILiteral.h"
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
Result<void> Bool_Value::copy_assign(IInitializer const& initializer)
{
    std::shared_ptr<const IValue> value = initializer.get_initializer_value();
    if (!value)
    {
        return Error("Cannot evaluate initializer list");
    }

    return copy_assign(*value);
}

std::shared_ptr<IValue> Bool_Value::clone() const
{
    return std::make_shared<Bool_Value>(*this);
}

Result<serialization::Value> Bool_Value::serialize() const
{
    return serialization::Value(get_value());
}

Result<void> Bool_Value::deserialize(serialization::Value const& sz_value)
{
    if (!sz_value.is_bool())
    {
        return Error("Expected bool value when deserializing");
    }
    return set_value(sz_value.get_as_bool());
}

}

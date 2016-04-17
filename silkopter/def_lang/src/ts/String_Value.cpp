#include "impl/String_Value.h"
#include "IInitializer_List.h"
#include "ILiteral.h"

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

std::unique_ptr<IValue> String_Value::clone() const
{
    return std::unique_ptr<IValue>(new String_Value(*this));
}


}

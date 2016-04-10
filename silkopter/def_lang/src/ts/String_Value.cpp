#include "values/String_Value.h"
#include "IInitializer_List.h"
#include "ILiteral.h"

namespace ts
{

String_Value::String_Value(IString_Type const& type)
    : Value_Template_EP<String_Traits>(type)
{
}

auto String_Value::copy_assign(IInitializer const& initializer) -> Result<void>
{
    IValue const* value = initializer.get_initializer_value();
    if (!value)
    {
        return Error("Cannot evaluate initializer list");
    }

    return copy_assign(*value);
}


}

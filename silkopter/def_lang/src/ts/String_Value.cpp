#include "values/String_Value.h"

namespace ts
{

String_Value::String_Value(IString_Type const& type)
    : Value_Template_EP<String_Value_Traits>(type)
{
}

auto String_Value::copy_assign(IInitializer const& initializer) -> Result<void>
{
    return Error("not implemented");
}


}

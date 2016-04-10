#include "values/Bool_Value.h"
#include "IInitializer_List.h"
#include "ILiteral.h"

namespace ts
{

Bool_Value::Bool_Value(IBool_Type const& type)
    : Value_Template_EP<Bool_Traits>(type)
{
}

auto Bool_Value::get_template_instantiation_string() const -> std::string
{
    return get_value() ? "true" : "false";
}
auto Bool_Value::copy_assign(IInitializer const& initializer) -> Result<void>
{
    IValue const* value = initializer.get_initializer_value();
    if (!value)
    {
        return Error("Cannot evaluate initializer list");
    }

    return copy_assign(*value);
}


}

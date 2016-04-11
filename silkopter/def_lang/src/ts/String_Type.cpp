#include "types/String_Type.h"
#include "values/String_Value.h"
#include "attributes/IAttribute.h"

namespace ts
{

String_Type::String_Type(std::string const& name)
    : Type_Template_EP(name)
{

}

auto String_Type::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}


}

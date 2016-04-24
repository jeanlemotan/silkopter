#include "def_lang/impl/String_Type.h"
#include "def_lang/impl//String_Value.h"
#include "def_lang/IAttribute.h"

namespace ts
{

String_Type::String_Type(std::string const& name)
    : Type_Template_EP(name)
{

}

Result<void> String_Type::validate_attribute(IAttribute const& attribute)
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}


}

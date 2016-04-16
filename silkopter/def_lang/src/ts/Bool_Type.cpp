#include "IAttribute.h"
#include "impl/Bool_Type.h"
#include "impl/Bool_Value.h"

namespace ts
{

Bool_Type::Bool_Type(std::string const& name)
    : Type_Template_EP(name)
{

}

auto Bool_Type::validate_attribute(IAttribute const& attribute) -> Result<void>
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}


}

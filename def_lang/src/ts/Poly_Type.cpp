#include "def_lang/impl/Poly_Type.h"
#include "def_lang/impl/Poly_Value.h"
#include "def_lang/IAttribute.h"

namespace ts
{

Poly_Type::Poly_Type(std::string const& name)
    : Symbol_EP(name)
{

}

Result<void> Poly_Type::init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments)
{
    if (arguments.size() != 1)
    {
        return Error("Expected only one template argument, got " + std::to_string(arguments.size()));
    }

    m_inner_type = std::dynamic_pointer_cast<const IType>(arguments[0]);
    if (!m_inner_type)
    {
        return Error("Invalid template argument. Expected type");
    }

    return success;
}

Result<void> Poly_Type::validate_attribute(IAttribute const& attribute)
{
    return Error("Attribute " + attribute.get_name() + " not supported");
}

std::shared_ptr<IType> Poly_Type::clone(std::string const& name) const
{
    return std::shared_ptr<IType>(new Poly_Type(*this));
}

std::string Poly_Type::get_template_instantiation_string() const
{
    return get_name();
}

std::shared_ptr<const IType> Poly_Type::get_inner_type() const
{
    return m_inner_type;
}

std::shared_ptr<IValue> Poly_Type::create_value() const
{
    return create_specialized_value();
}

std::shared_ptr<Poly_Type::value_type> Poly_Type::create_specialized_value() const
{
    return std::make_shared<Poly_Value>(shared_from_this());
}


}

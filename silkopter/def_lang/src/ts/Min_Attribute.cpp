#include "attributes/Min_Attribute.h"
#include "IInitializer.h"
#include "values/IValue.h"

namespace ts
{

Min_Attribute::~Min_Attribute()
{

}

auto Min_Attribute::get_name() const -> std::string
{
    return "min";
}
auto Min_Attribute::init(IInitializer const& initializer) -> Result<void>
{
    IValue const* v = initializer.get_initializer_value();
    if (!v)
    {
        return Error("Invalid initializer");
    }

    m_value = v->clone();

    return success;
}
auto Min_Attribute::get_min_value() const -> IValue const&
{
    return *m_value;
}

}

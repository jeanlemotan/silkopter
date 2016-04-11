#pragma once

#include "INumeric_Type_Template.h"
#include "values/All_INumeric_Values.h"
#include "impl/Type_Template_EP.h"
#include "attributes/IAttribute.h"

namespace ts
{

template<typename Traits>
class Numeric_Type_Template final : public Type_Template_EP<Traits>
{
public:
    Numeric_Type_Template(std::string const& name);

protected:
    auto validate_attribute(IAttribute const& attribute) -> Result<void> override;
};

}

#include "Numeric_Type_Template.inl"

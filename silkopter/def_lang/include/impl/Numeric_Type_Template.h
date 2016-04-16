#pragma once

#include "INumeric_Type_Template.h"
#include "impl/All_INumeric_Values.h"
#include "ep/Type_Template_EP.h"
#include "IAttribute.h"
#include "impl/Min_Attribute.h"
#include "impl/Max_Attribute.h"
#include "impl/Decimals_Attribute.h"

namespace ts
{


template<typename Traits>
class Numeric_Type_Template final : public Type_Template_EP<Traits>
{
public:
    Numeric_Type_Template(std::string const& name);

protected:
    auto validate_attribute(IAttribute const& attribute) -> Result<void> override;

    auto get_min_value() const -> typename Traits::value_interface const*;
    auto get_max_value() const -> typename Traits::value_interface const*;
    auto get_decimals() const -> size_t;

private:
    typename Traits::value_interface const* m_min_value = nullptr;
    typename Traits::value_interface const* m_max_value = nullptr;
    size_t m_decimals = 0;
};

}

#include "Numeric_Type_Template.inl"

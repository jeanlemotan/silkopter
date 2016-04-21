#pragma once

#include "INumeric_Type_Template.h"
#include "All_INumeric_Values.h"
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
    Result<void> validate_attribute(IAttribute const& attribute) override;

    typename Traits::fundamental_type const& get_min_value() const override;
    typename Traits::fundamental_type const& get_max_value() const override;
    size_t get_decimals() const override;

private:
    typename Traits::fundamental_type m_min_value = typename Traits::fundamental_type(Traits::min_value);
    typename Traits::fundamental_type m_max_value = typename Traits::fundamental_type(Traits::max_value);
    size_t m_decimals = 0;
};

}

#include "Numeric_Type_Template.inl"

#pragma once

#include "def_lang/INumeric_Type_Template.h"
#include "def_lang/All_INumeric_Values.h"
#include "def_lang/ep/Type_Template_EP.h"
#include "def_lang/IAttribute.h"
#include "def_lang/impl/Min_Attribute.h"
#include "def_lang/impl/Max_Attribute.h"
#include "def_lang/impl/Decimals_Attribute.h"

namespace ts
{


template<typename Traits>
class Numeric_Type_Template final : public Type_Template_EP<Traits>
{
public:
    Numeric_Type_Template(std::string const& name);
    Numeric_Type_Template(Numeric_Type_Template<Traits> const& other, std::string const& name);

protected:
    Result<void> validate_attribute_impl(IAttribute const& attribute) override;

    typename Traits::fundamental_type const& get_min_value() const override;
    typename Traits::fundamental_type const& get_max_value() const override;
    size_t get_decimals() const override;

private:
    typename Traits::fundamental_type m_min_value = typename Traits::fundamental_type(Traits::min_value);
    typename Traits::fundamental_type m_max_value = typename Traits::fundamental_type(Traits::max_value);
    size_t m_decimals = 3;
};

}

#include "Numeric_Type_Template.inl"

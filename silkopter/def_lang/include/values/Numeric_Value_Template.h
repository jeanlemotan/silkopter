#pragma once

#include "types/INumeric_Type_Template.h"
#include "INumeric_Value_Template.h"
#include "impl/Value_Template_EP.h"
#include "IInitializer_List.h"

namespace ts
{

template<typename Traits>
class Numeric_Value_Template final : public Value_Template_EP<Traits>
{
public:

    Numeric_Value_Template(typename Traits::type_interface const& type);

    auto copy_assign(IInitializer const& initializer) -> Result<void> override;

    using Value_Template_EP<Traits>::copy_assign;
    using Value_Template_EP<Traits>::set_value;

private:


};

}

#include "Numeric_Value_Template.inl"

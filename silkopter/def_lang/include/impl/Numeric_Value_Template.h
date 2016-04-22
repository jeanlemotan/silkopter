#pragma once

#include "INumeric_Type_Template.h"
#include "INumeric_Value_Template.h"
#include "ep/Value_Template_EP.h"
#include "IInitializer_List.h"
#include "Serialization.h"

namespace ts
{

template<typename Traits>
class Numeric_Value_Template final : public Value_Template_EP<Traits>
{
public:

    Numeric_Value_Template(std::shared_ptr<typename Traits::type_interface const> type);

    Result<void> copy_assign(IInitializer const& initializer) override;

    std::shared_ptr<IValue> clone() const override;

    using Value_Template_EP<Traits>::copy_assign;
    using Value_Template_EP<Traits>::set_value;
    using Value_Template_EP<Traits>::get_specialized_type;

    Result<serialization::Value> serialize() const override;


private:


};

}

#include "Numeric_Value_Template.inl"

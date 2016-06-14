#pragma once

#include "def_lang/INumeric_Type_Template.h"
#include "def_lang/INumeric_Value_Template.h"
#include "def_lang/ep/Value_Template_EP.h"
#include "def_lang/IInitializer_List.h"
#include "def_lang/ILiteral_Initializer.h"
#include "def_lang/ILiteral.h"
#include "def_lang/impl/Initializer_List.h"
#include "def_lang/Serialization.h"

namespace ts
{

template<typename Traits>
class Numeric_Value_Template final : public Value_Template_EP<Traits>
{
public:

    Numeric_Value_Template(std::shared_ptr<typename Traits::type_interface const> type);

    using Value_Template_EP<Traits>::copy_construct;
    Result<void> construct(IInitializer_List const& initializer_list = Initializer_List({})) override;

    using Value_Template_EP<Traits>::copy_assign;
    Result<void> copy_assign(IInitializer_List const& initializer_list) override;

    using Value_Template_EP<Traits>::set_value;
    using Value_Template_EP<Traits>::get_specialized_type;

    Result<sz::Value> serialize() const override;
    Result<void> deserialize(sz::Value const&) override;


private:


};

}

#include "Numeric_Value_Template.inl"

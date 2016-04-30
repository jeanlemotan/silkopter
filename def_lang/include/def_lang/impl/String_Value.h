#pragma once

#include "def_lang/IString_Type.h"
#include "def_lang/IString_Value.h"
#include "def_lang/ep/Value_Template_EP.h"

namespace ts
{

class String_Value;

struct String_Traits : public IString_Traits
{
    typedef String_Value value_implementation;
};

class String_Value final : public Value_Template_EP<String_Traits>
{
public:

    String_Value(std::shared_ptr<IString_Type const> type);

    using Value_Template_EP<String_Traits>::copy_construct;
    Result<void> construct(IInitializer_List const& initializer_list = Initializer_List({})) override;

    using Value_Template_EP<String_Traits>::copy_assign;
    Result<void> copy_assign(IInitializer_List const& initializer_list) override;

    Result<serialization::Value> serialize() const override;
    Result<void> deserialize(serialization::Value const&) override;

private:
};

}

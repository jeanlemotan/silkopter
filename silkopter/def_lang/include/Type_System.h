#pragma once

#include "impl/Declaration_Scope_Impl.h"
#include "types/IType.h"

namespace ts
{

class Type_System final : virtual public Declaration_Scope_Impl
{
public:

    void populate_builtin_types();

    using Declaration_Scope_Impl::add_symbol;
    using Declaration_Scope_Impl::get_symbol_count;
    using Declaration_Scope_Impl::get_symbol;
    using Declaration_Scope_Impl::get_symbol;

};

}

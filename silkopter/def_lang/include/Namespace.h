#pragma once

#include <vector>

#include "impl/Declaration_Scope_Impl.h"
#include "impl/Symbol_Impl.h"

namespace ts
{

class Namespace final : public Symbol_Impl, public Declaration_Scope_Impl
{
public:
    Namespace(std::string const& name);

    using Symbol_Impl::get_name;

    using Declaration_Scope_Impl::add_symbol;
    using Declaration_Scope_Impl::get_symbol_count;
    using Declaration_Scope_Impl::get_symbol;
    using Declaration_Scope_Impl::get_symbol;

private:
};

}

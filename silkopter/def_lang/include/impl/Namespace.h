#pragma once

#include <vector>

#include "ep/Declaration_Scope_EP.h"
#include "ep/Symbol_EP.h"

namespace ts
{

class Namespace final : public Symbol_EP, public Declaration_Scope_EP
{
public:
    Namespace(std::string const& name);

    using Symbol_EP::get_name;

    using Declaration_Scope_EP::add_symbol;
    using Declaration_Scope_EP::get_symbol_count;
    using Declaration_Scope_EP::get_symbol;
    using Declaration_Scope_EP::get_symbol;

private:
};

}

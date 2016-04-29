#pragma once

#include <vector>

#include "def_lang/ep/Declaration_Scope_EP.h"

namespace ts
{

class Namespace final : public Declaration_Scope_EP
{
public:
    Namespace(std::string const& name);

    using Declaration_Scope_EP::get_name;
    using Declaration_Scope_EP::add_symbol;
    using Declaration_Scope_EP::get_symbol_count;
    using Declaration_Scope_EP::get_symbol;
    using Declaration_Scope_EP::get_symbol;

private:
    Result<void> validate_symbol(std::shared_ptr<const ISymbol> symbol) override;
};

}

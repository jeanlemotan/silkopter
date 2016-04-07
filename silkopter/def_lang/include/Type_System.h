#pragma once

#include "impl/Declaration_Scope_Impl.h"
#include "types/IType.h"

namespace ts
{

class ITemplate_Argument;
class ITemplated_Type;

class Type_System final : virtual public Declaration_Scope_Impl
{
public:
    auto instantiate_template(std::string const& name, std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) -> std::shared_ptr<const ITemplated_Type>;

    void populate_builtin_types();

    using Declaration_Scope_Impl::add_symbol;
    using Declaration_Scope_Impl::get_symbol_count;
    using Declaration_Scope_Impl::get_symbol;
    using Declaration_Scope_Impl::get_symbol;

};

}

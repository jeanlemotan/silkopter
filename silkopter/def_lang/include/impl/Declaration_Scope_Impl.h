#pragma once

#include <vector>
#include "IDeclaration_Scope.h"

namespace ts
{

class ISymbol;

class Declaration_Scope_Impl : virtual public IDeclaration_Scope
{
public:

    auto add_symbol(std::unique_ptr<ISymbol> symbol) -> bool override;

    auto get_symbol_count() -> const size_t override;
    auto get_symbol(size_t idx) const -> std::shared_ptr<const ISymbol> override;
    auto get_symbol(size_t idx) -> std::shared_ptr<ISymbol> override;

    auto find_symbol_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_symbol_by_name(std::string const& name) const -> std::shared_ptr<const ISymbol> override;
    auto find_symbol_by_name(std::string const& name) -> std::shared_ptr<ISymbol> override;

private:
    std::vector<std::shared_ptr<ISymbol>> m_symbols;
};

}

#pragma once

#include <vector>
#include "IDeclaration_Scope.h"

namespace ts
{

class ISymbol;

class Declaration_Scope_Impl : virtual public IDeclaration_Scope, std::enable_shared_from_this<Declaration_Scope_Impl>
{
public:

    auto add_symbol(std::unique_ptr<ISymbol> symbol) -> bool override;

    auto get_symbol_count() -> const size_t override;
    auto get_symbol(size_t idx) const -> std::shared_ptr<const ISymbol> override;
    auto get_symbol(size_t idx) -> std::shared_ptr<ISymbol> override;

    auto find_symbol_idx_by_name(std::string const& name) const -> boost::optional<size_t> override;

    auto find_symbol_by_name(std::string const& name) const -> std::shared_ptr<const ISymbol> override;
    auto find_symbol_by_name(std::string const& name) -> std::shared_ptr<ISymbol> override;

    auto find_symbol_by_path(Symbol_Path const& path) const -> std::shared_ptr<const ISymbol> override;
    auto find_symbol_by_path(Symbol_Path const& path) -> std::shared_ptr<ISymbol> override;

    auto get_parent_scope() const -> IDeclaration_Scope const* const;
    auto get_parent_scope() -> IDeclaration_Scope*;

private:
    void set_parent_scope(IDeclaration_Scope* declaration_scope);

    std::vector<std::shared_ptr<ISymbol>> m_symbols;
    IDeclaration_Scope* m_parent_scope = nullptr;
};

}

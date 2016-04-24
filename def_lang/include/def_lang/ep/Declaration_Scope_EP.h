#pragma once

#include <vector>
#include "def_lang/IDeclaration_Scope.h"
#include "def_lang/ep/Symbol_EP.h"

namespace ts
{

class ISymbol;

class Declaration_Scope_EP : virtual public IDeclaration_Scope, public Symbol_EP
{
public:
    Declaration_Scope_EP(std::string const& name);

    Result<std::shared_ptr<const ISymbol>> add_symbol(std::shared_ptr<ISymbol> symbol) override;

    size_t get_symbol_count() const override;
    std::shared_ptr<const ISymbol> get_symbol(size_t idx) const override;
    std::shared_ptr<ISymbol> get_symbol(size_t idx) override;

    boost::optional<size_t> find_symbol_idx_by_name(std::string const& name) const override;

    std::shared_ptr<const ISymbol> find_symbol_by_name(std::string const& name) const override;
    std::shared_ptr<ISymbol> find_symbol_by_name(std::string const& name) override;

    std::shared_ptr<const ISymbol> find_symbol_by_path(Symbol_Path const& path) const override;
    std::shared_ptr<ISymbol> find_symbol_by_path(Symbol_Path const& path) override;

private:
    std::vector<std::shared_ptr<ISymbol>> m_symbols;
};

}

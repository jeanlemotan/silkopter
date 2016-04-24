#pragma once

#include <vector>
#include "def_lang/IDeclaration_Scope.h"

namespace ts
{

class ISymbol;

class Declaration_Scope_EP : virtual public IDeclaration_Scope
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

    std::string const& get_name() const override;
    Symbol_Path get_symbol_path() const override;

protected:
    IDeclaration_Scope const* get_parent_scope() const override;
    IDeclaration_Scope* get_parent_scope() override;

private:
    void set_parent_scope(IDeclaration_Scope* declaration_scope) override;

    std::string m_name;
    IDeclaration_Scope* m_parent_scope = nullptr;
    std::vector<std::shared_ptr<ISymbol>> m_symbols;
};

}

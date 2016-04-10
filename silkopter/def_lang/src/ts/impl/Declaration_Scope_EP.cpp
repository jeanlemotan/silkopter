#include "impl/Declaration_Scope_EP.h"
#include "ISymbol.h"

namespace ts
{

auto Declaration_Scope_EP::add_symbol(std::unique_ptr<ISymbol> symbol) -> Result<std::shared_ptr<const ISymbol>>
{
    if (!symbol)
    {
        return Error("cannot add null symbol");
    }
    if (find_symbol_by_name(symbol->get_name()) != nullptr)
    {
        return Error("Duplicated symbol '" + symbol->get_name() + "'");
    }

    if (IDeclaration_Scope* ds = dynamic_cast<IDeclaration_Scope*>(symbol.get()))
    {
        ds->set_parent_scope(this);
    }

    m_symbols.push_back(std::move(symbol));

    return m_symbols.back();
}

void Declaration_Scope_EP::set_parent_scope(IDeclaration_Scope* declaration_scope)
{
    m_parent_scope = declaration_scope;
}
auto Declaration_Scope_EP::get_parent_scope() const -> IDeclaration_Scope const* const
{
    return m_parent_scope;
}
auto Declaration_Scope_EP::get_parent_scope() -> IDeclaration_Scope*
{
    return m_parent_scope;
}

auto Declaration_Scope_EP::get_symbol_count() -> const size_t
{
    return m_symbols.size();
}
auto Declaration_Scope_EP::get_symbol(size_t idx) const -> std::shared_ptr<const ISymbol>
{
    return m_symbols[idx];
}
auto Declaration_Scope_EP::get_symbol(size_t idx) -> std::shared_ptr<ISymbol>
{
    return m_symbols[idx];
}

auto Declaration_Scope_EP::find_symbol_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    auto it = std::find_if(m_symbols.begin(), m_symbols.end(), [&](std::shared_ptr<ISymbol> const& s) { return s->get_name() == name; });
    return it != m_symbols.end() ? std::distance(m_symbols.begin(), it) : boost::optional<size_t>();
}

auto Declaration_Scope_EP::find_symbol_by_name(std::string const& name) const -> std::shared_ptr<const ISymbol>
{
    auto it = std::find_if(m_symbols.begin(), m_symbols.end(), [&](std::shared_ptr<ISymbol> const& s) { return s->get_name() == name; });
    return it != m_symbols.end() ? *it : std::shared_ptr<const ISymbol>();
}
auto Declaration_Scope_EP::find_symbol_by_name(std::string const& name) -> std::shared_ptr<ISymbol>
{
    auto it = std::find_if(m_symbols.begin(), m_symbols.end(), [&](std::shared_ptr<ISymbol> const& s) { return s->get_name() == name; });
    return it != m_symbols.end() ? *it : std::shared_ptr<ISymbol>();
}

auto Declaration_Scope_EP::find_symbol_by_path(Symbol_Path const& path) const -> std::shared_ptr<const ISymbol>
{
    return const_cast<Declaration_Scope_EP*>(this)->find_symbol_by_path(path);
}
auto Declaration_Scope_EP::find_symbol_by_path(Symbol_Path const& path) -> std::shared_ptr<ISymbol>
{
    if (path.empty())
    {
        return nullptr;
    }

    //for absolute paths, search in the root scope (the typesystem directly)
    if (path.is_absolute() && get_parent_scope())
    {
        IDeclaration_Scope* root_scope = get_parent_scope();
        while (root_scope->get_parent_scope())
        {
            root_scope = root_scope->get_parent_scope();
        }
        return root_scope->find_symbol_by_path(path);
    }

    //simple case - a path with one element. Search a symbol by name
    if (path.get_count() == 1)
    {
        std::shared_ptr<ISymbol> symbol = find_symbol_by_name(path.get(0));
        if (symbol)
        {
            return symbol;
        }
    }
    else
    {
        std::shared_ptr<IDeclaration_Scope> scope = find_specialized_symbol_by_name<IDeclaration_Scope>(path.get(0));
        if (scope)
        {
            Symbol_Path new_path = path;
            new_path.pop_front();
            std::shared_ptr<ISymbol> symbol = scope->find_symbol_by_path(new_path);
            if (symbol)
            {
                return symbol;
            }
        }
    }

    //if the path is not absolute, try in the parent as well
    if (!path.is_absolute() && get_parent_scope())
    {
        return get_parent_scope()->find_symbol_by_path(path);
    }
    return nullptr;
}


}

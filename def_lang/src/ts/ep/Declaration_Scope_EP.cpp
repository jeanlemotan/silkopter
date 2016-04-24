#include "def_lang/ep/Declaration_Scope_EP.h"
#include "def_lang/ISymbol.h"

namespace ts
{

Declaration_Scope_EP::Declaration_Scope_EP(std::string const& name)
    : Symbol_EP(name)
{

}

Result<std::shared_ptr<const ISymbol>> Declaration_Scope_EP::add_symbol(std::shared_ptr<ISymbol> symbol)
{
    if (!symbol)
    {
        return Error("cannot add null symbol");
    }
    if (find_symbol_by_name(symbol->get_name()) != nullptr)
    {
        return Error("Duplicated symbol '" + symbol->get_name() + "'");
    }

    symbol->set_parent_scope(this);

    m_symbols.push_back(std::move(symbol));

    return m_symbols.back();
}

size_t Declaration_Scope_EP::get_symbol_count() const
{
    return m_symbols.size();
}
std::shared_ptr<const ISymbol> Declaration_Scope_EP::get_symbol(size_t idx) const
{
    return m_symbols[idx];
}
std::shared_ptr<ISymbol> Declaration_Scope_EP::get_symbol(size_t idx)
{
    return m_symbols[idx];
}

boost::optional<size_t> Declaration_Scope_EP::find_symbol_idx_by_name(std::string const& name) const
{
    auto it = std::find_if(m_symbols.begin(), m_symbols.end(), [&](std::shared_ptr<ISymbol> const& s) { return s->get_name() == name; });
    return it != m_symbols.end() ? std::distance(m_symbols.begin(), it) : boost::optional<size_t>();
}

std::shared_ptr<const ISymbol> Declaration_Scope_EP::find_symbol_by_name(std::string const& name) const
{
    auto it = std::find_if(m_symbols.begin(), m_symbols.end(), [&](std::shared_ptr<ISymbol> const& s) { return s->get_name() == name; });
    return it != m_symbols.end() ? *it : std::shared_ptr<const ISymbol>();
}
std::shared_ptr<ISymbol> Declaration_Scope_EP::find_symbol_by_name(std::string const& name)
{
    auto it = std::find_if(m_symbols.begin(), m_symbols.end(), [&](std::shared_ptr<ISymbol> const& s) { return s->get_name() == name; });
    return it != m_symbols.end() ? *it : std::shared_ptr<ISymbol>();
}

std::shared_ptr<const ISymbol> Declaration_Scope_EP::find_symbol_by_path(Symbol_Path const& path) const
{
    return const_cast<Declaration_Scope_EP*>(this)->find_symbol_by_path(path);
}
std::shared_ptr<ISymbol> Declaration_Scope_EP::find_symbol_by_path(Symbol_Path const& path)
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

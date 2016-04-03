#include "impl/Declaration_Scope_Impl.h"
#include "ISymbol.h"

namespace ts
{

auto Declaration_Scope_Impl::add_symbol(std::unique_ptr<ISymbol> symbol) -> bool
{
    if (!symbol)
    {
        return false;
    }
    if (find_symbol_by_name(symbol->get_name()) != nullptr)
    {
        return false;
    }

    m_symbols.push_back(std::move(symbol));

    return true;
}

auto Declaration_Scope_Impl::get_symbol_count() -> const size_t
{
    return m_symbols.size();
}
auto Declaration_Scope_Impl::get_symbol(size_t idx) const -> std::shared_ptr<const ISymbol>
{
    return m_symbols[idx];
}
auto Declaration_Scope_Impl::get_symbol(size_t idx) -> std::shared_ptr<ISymbol>
{
    return m_symbols[idx];
}

auto Declaration_Scope_Impl::find_symbol_idx_by_name(std::string const& name) const -> boost::optional<size_t>
{
    return boost::none;
}

auto Declaration_Scope_Impl::find_symbol_by_name(std::string const& name) const -> std::shared_ptr<const ISymbol>
{
    return nullptr;
}
auto Declaration_Scope_Impl::find_symbol_by_name(std::string const& name) -> std::shared_ptr<ISymbol>
{
    return nullptr;
}


}

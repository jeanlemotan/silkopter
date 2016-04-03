#pragma once

#include <memory>
#include <boost/optional.hpp>

namespace ts
{

class ISymbol;

class IDeclaration_Scope
{
public:
    virtual ~IDeclaration_Scope() = default;

    virtual auto add_symbol(std::unique_ptr<ISymbol> symbol) -> bool = 0;

    virtual auto get_symbol_count() -> const size_t = 0;
    virtual auto get_symbol(size_t idx) const -> std::shared_ptr<const ISymbol> = 0;
    virtual auto get_symbol(size_t idx) -> std::shared_ptr<ISymbol> = 0;

    virtual auto find_symbol_idx_by_name(std::string const& name) const -> boost::optional<size_t> = 0;

    virtual auto find_symbol_by_name(std::string const& name) const -> std::shared_ptr<const ISymbol> = 0;
    virtual auto find_symbol_by_name(std::string const& name) -> std::shared_ptr<ISymbol> = 0;

    template<typename T>
    auto find_specialized_symbol_by_name(std::string const& name) const -> std::shared_ptr<const T>;

    template<typename T>
    auto find_specialized_symbol_by_name(std::string const& name) -> std::shared_ptr<T>;
};


template<typename T>
auto IDeclaration_Scope::find_specialized_symbol_by_name(std::string const& name) const -> std::shared_ptr<const T>
{
    return nullptr;
}

template<typename T>
auto IDeclaration_Scope::find_specialized_symbol_by_name(std::string const& name) -> std::shared_ptr<T>
{
    return nullptr;
}


}

#pragma once

#include <memory>
#include <boost/optional.hpp>
#include "Symbol_Path.h"

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
    template<typename T> auto find_specialized_symbol_by_name(std::string const& name) const -> std::shared_ptr<const T>;
    template<typename T> auto find_specialized_symbol_by_name(std::string const& name) -> std::shared_ptr<T>;

    virtual auto find_symbol_by_path(Symbol_Path const& path) const -> std::shared_ptr<const ISymbol> = 0;
    virtual auto find_symbol_by_path(Symbol_Path const& path) -> std::shared_ptr<ISymbol> = 0;
    template<typename T> auto find_specialized_symbol_by_path(Symbol_Path const& path) const -> std::shared_ptr<const T>;
    template<typename T> auto find_specialized_symbol_by_path(Symbol_Path const& path) -> std::shared_ptr<T>;

    virtual auto get_parent_scope() const -> std::shared_ptr<const IDeclaration_Scope> const = 0;
    virtual auto get_parent_scope() -> std::shared_ptr<IDeclaration_Scope> = 0;

//protected:
    virtual void set_parent_scope(std::shared_ptr<IDeclaration_Scope> declaration_scope) = 0;
};


template<typename T>
auto IDeclaration_Scope::find_specialized_symbol_by_name(std::string const& name) const -> std::shared_ptr<const T>
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_name(name));
}

template<typename T>
auto IDeclaration_Scope::find_specialized_symbol_by_name(std::string const& name) -> std::shared_ptr<T>
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_name(name));
}

template<typename T>
auto IDeclaration_Scope::find_specialized_symbol_by_path(Symbol_Path const& path) const -> std::shared_ptr<const T>
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_path(path));
}

template<typename T>
auto IDeclaration_Scope::find_specialized_symbol_by_path(Symbol_Path const& path) -> std::shared_ptr<T>
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_path(path));
}

}

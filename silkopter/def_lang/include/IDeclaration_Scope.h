#pragma once

#include <memory>
#include <boost/optional.hpp>
#include "Result.h"
#include "Symbol_Path.h"

namespace ts
{

class ISymbol;

class IDeclaration_Scope
{
public:
    virtual ~IDeclaration_Scope() = default;

    virtual Result<std::shared_ptr<const ISymbol>> add_symbol(std::shared_ptr<ISymbol> symbol) = 0;

    virtual size_t get_symbol_count() const  = 0;
    virtual std::shared_ptr<const ISymbol> get_symbol(size_t idx) const = 0;
    virtual std::shared_ptr<ISymbol> get_symbol(size_t idx) = 0;

    virtual boost::optional<size_t> find_symbol_idx_by_name(std::string const& name) const = 0;

    virtual std::shared_ptr<const ISymbol> find_symbol_by_name(std::string const& name) const = 0;
    virtual std::shared_ptr<ISymbol> find_symbol_by_name(std::string const& name) = 0;
    template<typename T> std::shared_ptr<const T> find_specialized_symbol_by_name(std::string const& name) const;
    template<typename T> std::shared_ptr<T> find_specialized_symbol_by_name(std::string const& name);

    virtual std::shared_ptr<const ISymbol> find_symbol_by_path(Symbol_Path const& path) const = 0;
    virtual std::shared_ptr<ISymbol> find_symbol_by_path(Symbol_Path const& path) = 0;
    template<typename T> std::shared_ptr<const T> find_specialized_symbol_by_path(Symbol_Path const& path) const;
    template<typename T> std::shared_ptr<T> find_specialized_symbol_by_path(Symbol_Path const& path);

    virtual IDeclaration_Scope const* get_parent_scope() const = 0;
    virtual IDeclaration_Scope* get_parent_scope() = 0;

//protected:
    virtual void set_parent_scope(IDeclaration_Scope* declaration_scope) = 0;
};


template<typename T>
std::shared_ptr<const T> IDeclaration_Scope::find_specialized_symbol_by_name(std::string const& name) const
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_name(name));
}

template<typename T>
std::shared_ptr<T> IDeclaration_Scope::find_specialized_symbol_by_name(std::string const& name)
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_name(name));
}

template<typename T>
std::shared_ptr<const T> IDeclaration_Scope::find_specialized_symbol_by_path(Symbol_Path const& path) const
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_path(path));
}

template<typename T>
std::shared_ptr<T> IDeclaration_Scope::find_specialized_symbol_by_path(Symbol_Path const& path)
{
    return std::dynamic_pointer_cast<T>(find_symbol_by_path(path));
}

}

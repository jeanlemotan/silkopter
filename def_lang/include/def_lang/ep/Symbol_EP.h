#pragma once

#include <memory>
#include <string>
#include "def_lang/ISymbol.h"

namespace ts
{

class IDeclaration_Scope;

class Symbol_EP : virtual public ISymbol
{
public:

    Symbol_EP(std::string const& name);

    std::string const& get_name() const override;
    Symbol_Path get_symbol_path() const override;

protected:
    IDeclaration_Scope const* get_parent_scope() const override;
    IDeclaration_Scope* get_parent_scope() override;

private:
    void set_parent_scope(IDeclaration_Scope* declaration_scope) override;

    std::string m_name;
    IDeclaration_Scope* m_parent_scope = nullptr;
};

}

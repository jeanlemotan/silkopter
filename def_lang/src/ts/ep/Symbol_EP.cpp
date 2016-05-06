#include "def_lang/ep/Symbol_EP.h"
#include "def_lang/IDeclaration_Scope.h"

namespace ts
{

Symbol_EP::Symbol_EP(std::string const& name)
    : m_name(name)
{
}

Symbol_EP::Symbol_EP(Symbol_EP const& other, std::string const& name)
    : m_name(name)
{

}

std::string const& Symbol_EP::get_name() const
{
    return m_name;
}

Symbol_Path Symbol_EP::get_symbol_path() const
{
    std::string path = m_name;
    IDeclaration_Scope* parent = m_parent_scope;
    while (parent)
    {
        path = parent->get_name() + "::" + path;
        parent = parent->get_parent_scope();
    }

    path = "::" + path;

    return Symbol_Path(path);
}

IDeclaration_Scope const* Symbol_EP::get_parent_scope() const
{
    return m_parent_scope;
}
IDeclaration_Scope* Symbol_EP::get_parent_scope()
{
    return m_parent_scope;
}

void Symbol_EP::set_parent_scope(IDeclaration_Scope* declaration_scope)
{
    m_parent_scope = declaration_scope;
}


}

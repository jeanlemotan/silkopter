#pragma once

#include "Result.h"
#include "def_lang/impl/Namespace.h"
#include "IType.h"

namespace ts
{

class ITemplate_Argument;
class ITemplated_Type;

class Type_System final
{
public:
    Type_System();
    Result<std::shared_ptr<const ITemplated_Type>> instantiate_template(std::string const& name, std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments);

    void populate_builtin_types();

    std::shared_ptr<IValue> create_value(Symbol_Path const& type_path) const;

    template<typename T>
    std::shared_ptr<T> create_specialized_value(Symbol_Path const& type_path) const;

    std::shared_ptr<IDeclaration_Scope> get_root_scope();
    std::shared_ptr<const IDeclaration_Scope> get_root_scope() const;

protected:
    std::shared_ptr<Namespace> m_root_scope;
};

template<typename T>
std::shared_ptr<T> Type_System::create_specialized_value(Symbol_Path const& type_path) const
{
    return std::dynamic_pointer_cast<T>(create_value(type_path));
}



}

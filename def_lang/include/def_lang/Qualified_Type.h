#pragma once

#include "ITemplate_Argument.h"

namespace ts
{
class IType;

class Qualified_Type : virtual public ITemplate_Argument
{
public:
    Qualified_Type(std::shared_ptr<const IType> type, bool is_const);

    std::shared_ptr<const IType> get_type() const;
    bool is_const() const;

    std::string get_template_instantiation_string() const override;

private:
    std::shared_ptr<const IType> m_type;
    bool m_is_const = false;
};


}

#pragma once

#include "def_lang/ILiteral_Attribute.h"
#include <string>
#include <memory>

namespace ts
{

class IValue;

class Literal_Attribute : public virtual ILiteral_Attribute
{
public:
    Literal_Attribute(std::string const& name, std::shared_ptr<IValue> value);
    ~Literal_Attribute();

    std::string get_name() const override;

    IValue const& get_value() const;

private:
    std::string m_name;
    std::shared_ptr<IValue> m_value;
};

}

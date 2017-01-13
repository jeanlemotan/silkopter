#pragma once

#include "def_lang/IName_Attribute.h"
#include <string>
#include <memory>

namespace ts
{

class IValue;

class Name_Attribute : public virtual IName_Attribute
{
public:
    Name_Attribute(std::string const& name);
    ~Name_Attribute();

    std::string get_name() const override;

private:
    std::string m_name;
};

}

#pragma once

#include "def_lang/IAttribute.h"
#include <string>

namespace ts
{

class Native_Type_Attribute : public virtual IAttribute
{
public:
    Native_Type_Attribute(std::string const& native_type);
    ~Native_Type_Attribute();

    std::string get_name() const override;
    std::string const& get_native_type() const;

private:
    std::string m_native_type;
};

}

#pragma once

#include "def_lang/IAttribute.h"
#include "def_lang/Symbol_Path.h"
#include <string>

namespace ts
{

class Native_Type_Attribute : public virtual IAttribute
{
public:
    Native_Type_Attribute(Symbol_Path const& native_type);
    ~Native_Type_Attribute();

    std::string get_name() const override;
    Symbol_Path const& get_native_type() const;

private:
    Symbol_Path m_native_type;
};

}

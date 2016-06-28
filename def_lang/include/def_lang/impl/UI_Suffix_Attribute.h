#pragma once

#include "def_lang/IAttribute.h"
#include <string>

namespace ts
{

class UI_Suffix_Attribute : public virtual IAttribute
{
public:
    UI_Suffix_Attribute(std::string const& ui_suffix);
    ~UI_Suffix_Attribute();

    std::string get_name() const override;
    std::string const& get_ui_suffix() const;

private:
    std::string m_ui_suffix;
};

}

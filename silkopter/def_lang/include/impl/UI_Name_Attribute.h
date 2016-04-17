#pragma once

#include "IAttribute.h"
#include <string>

namespace ts
{

class UI_Name_Attribute : public virtual IAttribute
{
public:
    UI_Name_Attribute(std::string const& ui_name);
    ~UI_Name_Attribute();

    std::string get_name() const override;
    std::string const& get_ui_name() const;

private:
    std::string m_ui_name;
};

}

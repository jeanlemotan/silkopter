#include "def_lang/impl/UI_Name_Attribute.h"

namespace ts
{

UI_Name_Attribute::UI_Name_Attribute(std::string const& ui_name)
    : m_ui_name(ui_name)
{
}
UI_Name_Attribute::~UI_Name_Attribute()
{
}
std::string UI_Name_Attribute::get_name() const
{
    return "ui_name";
}
std::string const& UI_Name_Attribute::get_ui_name() const
{
    return m_ui_name;
}

}

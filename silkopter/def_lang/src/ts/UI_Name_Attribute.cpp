#include "impl/UI_Name_Attribute.h"
#include "IInitializer.h"
#include "IString_Value.h"

namespace ts
{

UI_Name_Attribute::UI_Name_Attribute(std::string const& ui_name)
    : m_ui_name(ui_name)
{
}
UI_Name_Attribute::~UI_Name_Attribute()
{
}
auto UI_Name_Attribute::get_name() const -> std::string
{
    return "ui_name";
}
auto UI_Name_Attribute::get_ui_name() const -> std::string const&
{
    return m_ui_name;
}

}

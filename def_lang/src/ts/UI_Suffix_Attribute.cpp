#include "def_lang/impl/UI_Suffix_Attribute.h"

namespace ts
{

UI_Suffix_Attribute::UI_Suffix_Attribute(std::string const& ui_suffix)
    : m_ui_suffix(ui_suffix)
{
}
UI_Suffix_Attribute::~UI_Suffix_Attribute()
{
}
std::string UI_Suffix_Attribute::get_name() const
{
    return "ui_suffix";
}
std::string const& UI_Suffix_Attribute::get_ui_suffix() const
{
    return m_ui_suffix;
}

}

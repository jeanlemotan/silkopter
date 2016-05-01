#include "def_lang/impl/Native_Type_Attribute.h"

namespace ts
{

Native_Type_Attribute::Native_Type_Attribute(Symbol_Path const& native_type)
    : m_native_type(native_type)
{
}
Native_Type_Attribute::~Native_Type_Attribute()
{
}
std::string Native_Type_Attribute::get_name() const
{
    return "native_type";
}
Symbol_Path const& Native_Type_Attribute::get_native_type() const
{
    return m_native_type;
}

}

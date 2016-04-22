#include "JSON_Serializer.h"
#include "ts_assert.h"

namespace ts
{
namespace serialization
{

void to_json(Value const& value, std::string& dst, size_t ident)
{
    switch (value.get_type())
    {
    case Value::Type::BOOL: dst += value.get_as_bool() ? "true" : "false"; break;
    case Value::Type::INT8: dst += std::to_string(value.get_as_int8()); break;
    case Value::Type::UINT8: dst += std::to_string(value.get_as_uint8()); break;
    case Value::Type::INT16: dst += std::to_string(value.get_as_int16()); break;
    case Value::Type::UINT16: dst += std::to_string(value.get_as_uint16()); break;
    case Value::Type::INT32: dst += std::to_string(value.get_as_int32()); break;
    case Value::Type::UINT32: dst += std::to_string(value.get_as_uint32()); break;
    case Value::Type::INT64: dst += std::to_string(value.get_as_int64()); break;
    case Value::Type::UINT64: dst += std::to_string(value.get_as_uint64()); break;
    case Value::Type::FLOAT: dst += std::to_string(value.get_as_float()); break;
    case Value::Type::DOUBLE: dst += std::to_string(value.get_as_double()); break;
    case Value::Type::STRING: dst += "\"" + value.get_as_string() + "\""; break;
    case Value::Type::OBJECT:
    {
        std::string ident_str(ident > 0 ? ident - 1 : ident, '\t');
        std::string member_ident_str(ident, '\t');
        dst += "{\n";
        for (size_t i = 0; i < value.get_object_member_count(); i++)
        {
            dst += member_ident_str;
            dst += "\"";
            dst += value.get_object_member_name(i);
            dst += "\"=";
            to_json(value.get_object_member_value(i), dst, ident + 1);
            dst += ",\n";
        }
        dst += ident_str;
        dst += "}";
        break;
    }
    case Value::Type::ARRAY:
    {
        std::string ident_str(ident > 0 ? ident - 1 : ident, '\t');
        std::string element_ident_str(ident, '\t');
        dst += "[\n";
        for (size_t i = 0; i < value.get_array_element_count(); i++)
        {
            dst += element_ident_str;
            to_json(value.get_array_element_value(i), dst, ident + 1);
            dst += ",\n";
        }
        dst += ident_str;
        dst += "]";
        break;
    }
    default: TS_ASSERT(false); break;
    }
}

std::string to_json(Value const& value)
{
    std::string json;
    to_json(value, json, 1);
    return json;
}

}
}

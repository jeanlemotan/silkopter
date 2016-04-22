#include "JSON_Serializer.h"
#include "ts_assert.h"
#include "Result.h"

namespace ts
{
namespace serialization
{

static void to_json(Value const& value, std::string& dst, size_t ident)
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
            dst += "\":";
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

static size_t skip_whitespace(std::string const& json, size_t offset)
{
    return json.find_first_not_of(" \t\n\r", offset);
}

static Result<Value> parse_string_value(std::string const& json, size_t& offset)
{
    if (json[offset] != '"')
    {
        return Error("Unexpected char '" + std::to_string(json[offset]) + "' while parsing string value");
    }
    offset++;

    size_t size = json.size();
    char prev_ch = 0;
    for (size_t i = offset; i < size; ++i)
    {
        char ch = json[i];
        if (ch == '"' && prev_ch != '\\')
        {
            Value value(json.substr(offset, i - offset));
            offset = i + 1;

            return std::move(value);
        }
        prev_ch = ch;
    }

    return Error("Unexpected end of string");
}

static Result<Value> parse_value(std::string const& json, size_t& offset);

static Result<Value> parse_object_value(std::string const& json, size_t& offset)
{
    if (json[offset] != '{')
    {
        return Error("Unexpected char '" + std::to_string(json[offset]) + "' while parsing object value");
    }
    offset++;

    Value object_value(Value::Type::OBJECT);

    while (true)
    {
        offset = skip_whitespace(json, offset);
        if (offset == std::string::npos)
        {
            return Error("Unexpected end of string");
        }

        if (json[offset] == '}')
        {
            offset++;
            return std::move(object_value);
        }

        if (object_value.get_object_member_count() > 0)
        {
            if (json[offset] != ',')
            {
                return Error("Missing ',' while parsing object value");
            }
            offset++;
            //treat the case of stray comma after the last member of an object
            offset = skip_whitespace(json, offset);
            if (offset == std::string::npos)
            {
                return Error("Unexpected end of string");
            }
            if (json[offset] == '}')
            {
                offset++;
                return std::move(object_value);
            }
        }

        auto result = parse_string_value(json, offset);
        if (result != success)
        {
            return std::move(result);
        }
        Value name_value = result.extract_payload();

        offset = skip_whitespace(json, offset);
        if (offset == std::string::npos)
        {
            return Error("Unexpected end of string");
        }

        if (json[offset] != ':')
        {
            return Error("Missing ':' while parsing object value");
        }
        offset++;

        result = parse_value(json, offset);
        if (result != success)
        {
            return std::move(result);
        }

        object_value.add_object_member(name_value.extract_as_string(), result.extract_payload());
    }

    return Error("Unexpected end of string");
}

static Result<Value> parse_array_value(std::string const& json, size_t& offset)
{
    if (json[offset] != '[')
    {
        return Error("Unexpected char '" + std::to_string(json[offset]) + "' while parsing array value");
    }
    offset++;

    Value array_value(Value::Type::ARRAY);

    while (true)
    {
        offset = skip_whitespace(json, offset);
        if (offset == std::string::npos)
        {
            return Error("Unexpected end of string");
        }

        if (json[offset] == ']')
        {
            offset++;
            return std::move(array_value);
        }

        if (array_value.get_array_element_count() > 0)
        {
            if (json[offset] != ',')
            {
                return Error("Missing ',' while parsing array value");
            }
            offset++;

            //handle the case of stray comma after the last element of an array
            offset = skip_whitespace(json, offset);
            if (offset == std::string::npos)
            {
                return Error("Unexpected end of string");
            }

            if (json[offset] == ']')
            {
                offset++;
                return std::move(array_value);
            }
        }

        auto result = parse_value(json, offset);
        if (result != success)
        {
            return std::move(result);
        }

        array_value.add_array_element(result.extract_payload());
    }

    return Error("Unexpected end of string");
}

static Result<Value> parse_true_value(std::string const& json, size_t& offset)
{
    if (json.find("true", offset, 4) != offset)
    {
        return Error("Unexpected value");
    }
    offset += 4;
    return Value(true);
}

static Result<Value> parse_false_value(std::string const& json, size_t& offset)
{
    if (json.find("false", offset, 5) != offset)
    {
        return Error("Unexpected value");
    }
    offset += 5;
    return Value(false);
}

static Result<Value> parse_null_value(std::string const& json, size_t& offset)
{
    if (json.find("null", offset, 4) != offset)
    {
        return Error("Unexpected value");
    }
    offset += 4;
    return Value(Value::Type::EMPTY);
}

static Result<Value> parse_number_value(std::string const& json, size_t& offset)
{
    size_t end = json.find_first_of(" \t,\n\r]}", offset);
    if (end == std::string::npos)
    {
        return Error("Unexpected end of string");
    }

    offset = end;

    return Value(0);
}

static Result<Value> parse_value(std::string const& json, size_t& offset)
{
    offset = skip_whitespace(json, offset);
    if (offset == std::string::npos)
    {
        return Error("Unexpected end of string");
    }

    char ch = json[offset];
    switch (ch)
    {
    case '"': return parse_string_value(json, offset);
    case '{': return parse_object_value(json, offset);
    case '[': return parse_array_value(json, offset);
    case 't': return parse_true_value(json, offset);
    case 'f': return parse_false_value(json, offset);
    case 'n': return parse_null_value(json, offset);
    default: return parse_number_value(json, offset);
    }
}


/////////////////////////////////////////////////////////////////////////////////////

std::string to_json(Value const& value)
{
    std::string json;
    to_json(value, json, 1);
    return json;
}

Result<Value> from_json(std::string const& json)
{
    size_t offset = 0;
    return parse_object_value(json, offset);
}

}
}

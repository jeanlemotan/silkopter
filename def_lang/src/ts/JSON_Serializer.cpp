#include "def_lang/JSON_Serializer.h"
#include "def_lang/ts_assert.h"
#include "def_lang/Result.h"

#include <memory.h>

namespace ts
{
namespace sz
{

static void to_string(std::string& dst, int8_t v)
{
    char buffer[32];
    sprintf(buffer, "%d", int(v));
    dst += buffer;
}
static void to_string(std::string& dst, uint8_t v)
{
    char buffer[32];
    sprintf(buffer, "%d", int(v));
    dst += buffer;
}
static void to_string(std::string& dst, int16_t v)
{
    char buffer[32];
    sprintf(buffer, "%d", int(v));
    dst += buffer;
}
static void to_string(std::string& dst, uint16_t v)
{
    char buffer[32];
    sprintf(buffer, "%d", int(v));
    dst += buffer;
}
static void to_string(std::string& dst, int32_t v)
{
    char buffer[32];
    sprintf(buffer, "%d", v);
    dst += buffer;
}
static void to_string(std::string& dst, uint32_t v)
{
    char buffer[32];
    sprintf(buffer, "%u", v);
    dst += buffer;
}
static void to_string(std::string& dst, int64_t v)
{
    char buffer[32];
    sprintf(buffer, "%lld", v);
    dst += buffer;
}
static void to_string(std::string& dst, uint64_t v)
{
    char buffer[32];
    sprintf(buffer, "%llu", v);
    dst += buffer;
}
static void to_string(std::string& dst, float v)
{
    char buffer[64];
    sprintf(buffer, "%f", v);
    dst += buffer;
}
static void to_string(std::string& dst, double v)
{
    char buffer[64];
    sprintf(buffer, "%f", v);
    dst += buffer;
}

static void to_json(std::string& dst, Value const& value, size_t ident_count, bool nice)
{
    switch (value.get_type())
    {
    case Value::Type::EMPTY: dst += "null"; break;
    case Value::Type::BOOL: dst += value.get_as_bool() ? "true" : "false"; break;
    case Value::Type::INT8: to_string(dst, value.get_as_int8()); break;
    case Value::Type::UINT8: to_string(dst, value.get_as_uint8()); break;
    case Value::Type::INT16: to_string(dst, value.get_as_int16()); break;
    case Value::Type::UINT16: to_string(dst, value.get_as_uint16()); break;
    case Value::Type::INT32: to_string(dst, value.get_as_int32()); break;
    case Value::Type::UINT32: to_string(dst, value.get_as_uint32()); break;
    case Value::Type::INT64: to_string(dst, value.get_as_int64()); break;
    case Value::Type::UINT64: to_string(dst, value.get_as_uint64()); break;
    case Value::Type::FLOAT: to_string(dst, value.get_as_float()); break;
    case Value::Type::DOUBLE: to_string(dst, value.get_as_double()); break;
    case Value::Type::STRING:
    {
        dst.push_back('"');
        dst += value.get_as_string();
        dst.push_back('"');
        break;
    }
    case Value::Type::OBJECT:
    {
        std::string ident_str = nice ? std::string(ident_count > 0 ? ident_count - 1 : ident_count, '\t') : std::string();
        std::string member_ident_str = nice ? std::string(ident_count, '\t') : std::string();
        dst.push_back('{');
        if (nice)
        {
            dst.push_back('\n');
        }
        size_t count = value.get_object_member_count();
        for (size_t i = 0; i < count; i++)
        {
            dst += member_ident_str;
            dst.push_back('"');
            dst += value.get_object_member_name(i);
            dst += "\":";
            to_json(dst, value.get_object_member_value(i), ident_count + 1, nice);
            if (i + 1 < count)
            {
                dst.push_back(',');
            }
            if (nice)
            {
                dst.push_back('\n');
            }
        }
        dst += ident_str;
        dst.push_back('}');
        break;
    }
    case Value::Type::ARRAY:
    {
        std::string ident_str = nice ? std::string(ident_count > 0 ? ident_count - 1 : ident_count, '\t') : std::string();
        std::string element_ident_str = nice ? std::string(ident_count, '\t') : std::string();
        dst.push_back('[');
        if (nice)
        {
            dst.push_back('\n');
        }
        size_t count = value.get_array_element_count();
        for (size_t i = 0; i < count; i++)
        {
            dst += element_ident_str;
            to_json(dst, value.get_array_element_value(i), ident_count + 1, nice);
            if (i + 1 < count)
            {
                dst.push_back(',');
            }
            if (nice)
            {
                dst.push_back('\n');
            }
        }
        dst += ident_str;
        dst.push_back(']');
        break;
    }
    default: TS_ASSERT(false); break;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////


static char const* skip_whitespace(char const*& json_ptr, char const* json_end)
{
    const char* ptr = json_ptr;
    while (ptr < json_end)
    {
        char ch = *ptr;
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
        {
            return ptr;
        }
        ptr++;
    }
    return json_end;
}

static Result<std::string> parse_string(char const*& json_ptr, char const* json_end)
{
    if (json_ptr >= json_end)
    {
        return Error("Unexpected end of data while parsing string value");
    }
    if (*json_ptr != '"')
    {
        return Error("Unexpected char '" + std::to_string(*json_ptr) + "' while parsing string value");
    }
    json_ptr++; //skip the "

    char prev_ch = 0;

    const char* ptr = json_ptr;
    const char* start = ptr;
    while (ptr < json_end)
    {
        char ch = *ptr;
        if (ch == '"' && prev_ch != '\\')
        {
            json_ptr = ptr + 1;
            return std::string(start, ptr);
        }
        prev_ch = ch;
        ptr++;
    }

    return Error("Unexpected end of string");
}

static Result<Value> parse_string_value(char const*& json_ptr, char const* json_end)
{
    auto result = parse_string(json_ptr, json_end);
    if (result != success)
    {
        return result.error();
    }
    return Value(result.extract_payload());
}

static Result<Value> parse_value(char const*& json_ptr, char const* json_end);

static Result<Value> parse_object_value(char const*& json_ptr, char const* json_end)
{
    if (json_ptr >= json_end)
    {
        return Error("Unexpected end of data while parsing object value");
    }
    if (*json_ptr != '{')
    {
        return Error("Unexpected char '" + std::to_string(*json_ptr) + "' while parsing object value");
    }
    json_ptr++; //skip the {

    Value object_value(Value::Type::OBJECT);

    while (true)
    {
        json_ptr = skip_whitespace(json_ptr, json_end);
        if (json_ptr >= json_end)
        {
            return Error("Unexpected end of data");
        }

        if (*json_ptr == '}')
        {
            json_ptr++; //skip the }
            return std::move(object_value);
        }

        if (object_value.get_object_member_count() > 0)
        {
            if (*json_ptr != ',')
            {
                return Error("Missing ',' while parsing object value");
            }
            json_ptr++; //skip the ,
            //treat the case of stray comma after the last member of an object
            json_ptr = skip_whitespace(json_ptr, json_end);
            if (json_ptr >= json_end)
            {
                return Error("Unexpected end of string");
            }
            if (*json_ptr == '}')
            {
                json_ptr++; //skip the }
                return std::move(object_value);
            }
        }

        auto name_result = parse_string(json_ptr, json_end);
        if (name_result != success)
        {
            return name_result.error();
        }
        std::string name = name_result.extract_payload();

        json_ptr = skip_whitespace(json_ptr, json_end);
        if (json_ptr >= json_end)
        {
            return Error("Unexpected end of string");
        }

        if (*json_ptr != ':')
        {
            return Error("Missing ':' while parsing object value");
        }
        json_ptr++; //skip the :

        auto result = parse_value(json_ptr, json_end);
        if (result != success)
        {
            return std::move(result);
        }

        object_value.add_object_member(std::move(name), result.extract_payload());
    }

    return Error("Unexpected end of string");
}

static Result<Value> parse_array_value(char const*& json_ptr, char const* json_end)
{
    if (json_ptr >= json_end)
    {
        return Error("Unexpected end of data while parsing array value");
    }
    if (*json_ptr != '[')
    {
        return Error("Unexpected char '" + std::to_string(*json_ptr) + "' while parsing array value");
    }
    json_ptr++; //skip the [

    Value array_value(Value::Type::ARRAY);

    while (true)
    {
        json_ptr = skip_whitespace(json_ptr, json_end);
        if (json_ptr >= json_end)
        {
            return Error("Unexpected end of string");
        }

        if (*json_ptr == ']')
        {
            json_ptr++; //skip the ]
            return std::move(array_value);
        }

        if (array_value.get_array_element_count() > 0)
        {
            if (*json_ptr != ',')
            {
                return Error("Missing ',' while parsing array value");
            }
            json_ptr++; //skip the ,

            //handle the case of stray comma after the last element of an array
            json_ptr = skip_whitespace(json_ptr, json_end);
            if (json_ptr >= json_end)
            {
                return Error("Unexpected end of string");
            }

            if (*json_ptr == ']')
            {
                json_ptr++; //skip the ]
                return std::move(array_value);
            }
        }

        auto result = parse_value(json_ptr, json_end);
        if (result != success)
        {
            return std::move(result);
        }

        array_value.add_array_element(result.extract_payload());
    }

    return Error("Unexpected end of string");
}

static Result<Value> parse_true_value(char const*& json_ptr, char const* json_end)
{
    if (json_ptr + 4 >= json_end)
    {
        return Error("Unexpected end of data while parsing 'true' value");
    }
    if (memcmp(json_ptr, "true", 4) != 0)
    {
        return Error("Unexpected value");
    }
    json_ptr += 4;
    return Value(true);
}

static Result<Value> parse_false_value(char const*& json_ptr, char const* json_end)
{
    if (json_ptr + 5 >= json_end)
    {
        return Error("Unexpected end of data while parsing 'false' value");
    }
    if (memcmp(json_ptr, "false", 5) != 0)
    {
        return Error("Unexpected value");
    }
    json_ptr += 5;
    return Value(false);
}

static Result<Value> parse_null_value(char const*& json_ptr, char const* json_end)
{
    if (json_ptr + 4 >= json_end)
    {
        return Error("Unexpected end of data while parsing 'null' value");
    }
    if (memcmp(json_ptr, "null", 4) != 0)
    {
        return Error("Unexpected value");
    }
    json_ptr += 4;
    return Value();
}

static Result<Value> parse_number_value(char const*& json_ptr, char const* json_end)
{
    if (json_ptr + 1 >= json_end)
    {
        return Error("Unexpected end of data while parsing number value");
    }

    int64_t sign = 1;
    if (*json_ptr == '-')
    {
        sign = -1;
        json_ptr++; //skip the sign
    }

    int64_t whole = 0;
    int64_t decimal = 0;
    uint32_t decimal_digit_count = 0;
    int64_t exponent = 0;
    int32_t exponent_sign = 1;

    const char* ptr = json_ptr;
    while (ptr != 0)
    {
        char ch = *ptr;
        if (ch < '0' || ch > '9')
        {
            break;
        }
        whole = whole * 10 + (ch - '0');
        ptr++;
    }

    if (*ptr == '.')
    {
        ptr++; //skip the '.'
        while (ptr != 0)
        {
            char ch = *ptr;
            if (ch < '0' || ch > '9')
            {
                break;
            }
            decimal = decimal * 10 + (ch - '0');
            decimal_digit_count++;
            ptr++;
        }
    }
    if (*ptr == 'e' || *ptr == 'E')
    {
        ptr++; //skip the 'e'

        if (*ptr == '-')
        {
            exponent_sign = -1;
            ptr++; //skip the -
        }

        while (ptr != 0)
        {
            char ch = *ptr;
            if (ch < '0' || ch > '9')
            {
                break;
            }
            exponent = exponent * 10 + (ch - '0');
            ptr++;
        }
    }

    json_ptr = ptr;

    whole *= sign;

    if (decimal_digit_count == 0)
    {
        static constexpr int64_t s_pow[19] = {
                1,
                10,
                100,
                1000,
                10000,
                100000,
                1000000,
                10000000,
                100000000,
                1000000000,
                10000000000,
                100000000000,
                1000000000000,
                10000000000000,
                100000000000000,
                1000000000000000,
                10000000000000000,
                100000000000000000,
                1000000000000000000,
        };

        int64_t value = whole * exponent_sign * s_pow[exponent];
        if (value >= 0)
        {
            if (value <= 255)
            {
                return Value(static_cast<uint8_t>(value));
            }
            else if (value <= 65535)
            {
                return Value(static_cast<uint16_t>(value));
            }
            else if (value <= 4294967295)
            {
                return Value(static_cast<uint32_t>(value));
            }
            return Value(static_cast<uint64_t>(value));
        }
        else
        {
            if (value >= -128 && value < 127)
            {
                return Value(static_cast<int8_t>(value));
            }
            else if (value >= -32768 && value < 32767)
            {
                return Value(static_cast<int16_t>(value));
            }
            else if (value >= -2147483648 && value < 2147483647)
            {
                return Value(static_cast<int32_t>(value));
            }
            return Value(value);
        }
    }

    if (decimal_digit_count > 19)
    {
        return Error("Too many decimals");
    }
    if (exponent > 19)
    {
        return Error("Exponent too big");
    }

    static constexpr double s_pow[19] = { 1.0,
                                          10.0,
                                          100.0,
                                          1000.0,
                                          10000.0,
                                          100000.0,
                                          1000000.0,
                                          10000000.0,
                                          100000000.0,
                                          1000000000.0,
                                          10000000000.0,
                                          100000000000.0,
                                          1000000000000.0,
                                          10000000000000.0,
                                          100000000000000.0,
                                          1000000000000000.0,
                                          10000000000000000.0,
                                          100000000000000000.0,
                                          1000000000000000000.0,
                                        };

    double value = static_cast<double>(whole);
    value += static_cast<double>(decimal) / s_pow[decimal_digit_count];

    value *= exponent_sign * s_pow[exponent];

    return Value(value);
}

static Result<Value> parse_value(char const*& json_ptr, char const* json_end)
{
    json_ptr = skip_whitespace(json_ptr, json_end);
    if (json_ptr >= json_end)
    {
        return Error("Unexpected end of string");
    }

    char ch = *json_ptr;
    switch (ch)
    {
    case '"': return parse_string_value(json_ptr, json_end);
    case '{': return parse_object_value(json_ptr, json_end);
    case '[': return parse_array_value(json_ptr, json_end);
    case 't': return parse_true_value(json_ptr, json_end);
    case 'f': return parse_false_value(json_ptr, json_end);
    case 'n': return parse_null_value(json_ptr, json_end);
    default: return parse_number_value(json_ptr, json_end);
    }
}


/////////////////////////////////////////////////////////////////////////////////////

std::string to_json(Value const& value, bool nice)
{
    std::string json;
    to_json(json, value, 1, nice);
    return json;
}

void to_json(std::string& dst, Value const& value, bool nice)
{
    dst.clear();
    dst.reserve(32768);
    to_json(dst, value, 1, nice);
}

Result<Value> from_json(std::string const& json)
{
    char const* ptr = json.c_str();
    return parse_value(ptr, ptr + json.size());
}

Result<Value> from_json(void const* data, size_t size)
{
    TS_ASSERT(data && size > 0);
    if (!data || size == 0)
    {
        return ts::Error("Cannot parse empty string");
    }

    //todo - optimize this to avoid the temp string
    char const* json = reinterpret_cast<char const*>(data);
    return parse_value(json, json + size);
}

}
}

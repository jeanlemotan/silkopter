#include "def_lang/JSON_Serializer.h"
#include "def_lang/ts_assert.h"
#include "def_lang/Result.h"

#include <memory.h>


static const char gDigitsLut[200] =
{
    '0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
    '1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
    '2','0','2','1','2','2','2','3','2','4','2','5','2','6','2','7','2','8','2','9',
    '3','0','3','1','3','2','3','3','3','4','3','5','3','6','3','7','3','8','3','9',
    '4','0','4','1','4','2','4','3','4','4','4','5','4','6','4','7','4','8','4','9',
    '5','0','5','1','5','2','5','3','5','4','5','5','5','6','5','7','5','8','5','9',
    '6','0','6','1','6','2','6','3','6','4','6','5','6','6','6','7','6','8','6','9',
    '7','0','7','1','7','2','7','3','7','4','7','5','7','6','7','7','7','8','7','9',
    '8','0','8','1','8','2','8','3','8','4','8','5','8','6','8','7','8','8','8','9',
    '9','0','9','1','9','2','9','3','9','4','9','5','9','6','9','7','9','8','9','9'
};

// Branching for different cases (forward) Use lookup table of two digits
static void u32toa_branchlut(uint32_t value, char* buffer)
{
    if (value < 10000)
    {
        const uint32_t d1 = (value / 100) << 1;
        const uint32_t d2 = (value % 100) << 1;

        if (value >= 1000)
        {
            *buffer++ = gDigitsLut[d1];
        }
        if (value >= 100)
        {
            *buffer++ = gDigitsLut[d1 + 1];
        }
        if (value >= 10)
        {
            *buffer++ = gDigitsLut[d2];
        }
        *buffer++ = gDigitsLut[d2 + 1];
    }
    else if (value < 100000000)
    {
        // value = bbbbcccc
        const uint32_t b = value / 10000;
        const uint32_t c = value % 10000;

        const uint32_t d1 = (b / 100) << 1;
        const uint32_t d2 = (b % 100) << 1;

        const uint32_t d3 = (c / 100) << 1;
        const uint32_t d4 = (c % 100) << 1;

        if (value >= 10000000)
        {
            *buffer++ = gDigitsLut[d1];
        }
        if (value >= 1000000)
        {
            *buffer++ = gDigitsLut[d1 + 1];
        }
        if (value >= 100000)
        {
            *buffer++ = gDigitsLut[d2];
        }
        *buffer++ = gDigitsLut[d2 + 1];

        *buffer++ = gDigitsLut[d3];
        *buffer++ = gDigitsLut[d3 + 1];
        *buffer++ = gDigitsLut[d4];
        *buffer++ = gDigitsLut[d4 + 1];
    }
    else
    {
        // value = aabbbbcccc in decimal

        const uint32_t a = value / 100000000; // 1 to 42
        value %= 100000000;

        if (a >= 10)
        {
            const unsigned i = a << 1;
            *buffer++ = gDigitsLut[i];
            *buffer++ = gDigitsLut[i + 1];
        }
        else
        {
            *buffer++ = '0' + static_cast<char>(a);
        }

        const uint32_t b = value / 10000; // 0 to 9999
        const uint32_t c = value % 10000; // 0 to 9999

        const uint32_t d1 = (b / 100) << 1;
        const uint32_t d2 = (b % 100) << 1;

        const uint32_t d3 = (c / 100) << 1;
        const uint32_t d4 = (c % 100) << 1;

        *buffer++ = gDigitsLut[d1];
        *buffer++ = gDigitsLut[d1 + 1];
        *buffer++ = gDigitsLut[d2];
        *buffer++ = gDigitsLut[d2 + 1];
        *buffer++ = gDigitsLut[d3];
        *buffer++ = gDigitsLut[d3 + 1];
        *buffer++ = gDigitsLut[d4];
        *buffer++ = gDigitsLut[d4 + 1];
    }
    *buffer++ = '\0';
}

static void i32toa_branchlut(int32_t value, char* buffer)
{
    uint32_t u = static_cast<uint32_t>(value);
    if (value < 0)
    {
        *buffer++ = '-';
        u = ~u + 1;
    }

    u32toa_branchlut(u, buffer);
}

static void u64toa_branchlut(uint64_t value, char* buffer)
{
    if (value < 100000000)
    {
        uint32_t v = static_cast<uint32_t>(value);
        if (v < 10000)
        {
            const uint32_t d1 = (v / 100) << 1;
            const uint32_t d2 = (v % 100) << 1;

            if (v >= 1000)
            {
                *buffer++ = gDigitsLut[d1];
            }
            if (v >= 100)
            {
                *buffer++ = gDigitsLut[d1 + 1];
            }
            if (v >= 10)
            {
                *buffer++ = gDigitsLut[d2];
            }
            *buffer++ = gDigitsLut[d2 + 1];
        }
        else {
            // value = bbbbcccc
            const uint32_t b = v / 10000;
            const uint32_t c = v % 10000;

            const uint32_t d1 = (b / 100) << 1;
            const uint32_t d2 = (b % 100) << 1;

            const uint32_t d3 = (c / 100) << 1;
            const uint32_t d4 = (c % 100) << 1;

            if (value >= 10000000)
            {
                *buffer++ = gDigitsLut[d1];
            }
            if (value >= 1000000)
            {
                *buffer++ = gDigitsLut[d1 + 1];
            }
            if (value >= 100000)
            {
                *buffer++ = gDigitsLut[d2];
            }
            *buffer++ = gDigitsLut[d2 + 1];

            *buffer++ = gDigitsLut[d3];
            *buffer++ = gDigitsLut[d3 + 1];
            *buffer++ = gDigitsLut[d4];
            *buffer++ = gDigitsLut[d4 + 1];
        }
    }
    else if (value < 10000000000000000)
    {
        const uint32_t v0 = static_cast<uint32_t>(value / 100000000);
        const uint32_t v1 = static_cast<uint32_t>(value % 100000000);

        const uint32_t b0 = v0 / 10000;
        const uint32_t c0 = v0 % 10000;

        const uint32_t d1 = (b0 / 100) << 1;
        const uint32_t d2 = (b0 % 100) << 1;

        const uint32_t d3 = (c0 / 100) << 1;
        const uint32_t d4 = (c0 % 100) << 1;

        const uint32_t b1 = v1 / 10000;
        const uint32_t c1 = v1 % 10000;

        const uint32_t d5 = (b1 / 100) << 1;
        const uint32_t d6 = (b1 % 100) << 1;

        const uint32_t d7 = (c1 / 100) << 1;
        const uint32_t d8 = (c1 % 100) << 1;

        if (value >= 1000000000000000)
        {
            *buffer++ = gDigitsLut[d1];
        }
        if (value >= 100000000000000)
        {
            *buffer++ = gDigitsLut[d1 + 1];
        }
        if (value >= 10000000000000)
        {
            *buffer++ = gDigitsLut[d2];
        }
        if (value >= 1000000000000)
        {
            *buffer++ = gDigitsLut[d2 + 1];
        }
        if (value >= 100000000000)
        {
            *buffer++ = gDigitsLut[d3];
        }
        if (value >= 10000000000)
        {
            *buffer++ = gDigitsLut[d3 + 1];
        }
        if (value >= 1000000000)
        {
            *buffer++ = gDigitsLut[d4];
        }
        if (value >= 100000000)
        {
            *buffer++ = gDigitsLut[d4 + 1];
        }

        *buffer++ = gDigitsLut[d5];
        *buffer++ = gDigitsLut[d5 + 1];
        *buffer++ = gDigitsLut[d6];
        *buffer++ = gDigitsLut[d6 + 1];
        *buffer++ = gDigitsLut[d7];
        *buffer++ = gDigitsLut[d7 + 1];
        *buffer++ = gDigitsLut[d8];
        *buffer++ = gDigitsLut[d8 + 1];
    }
    else
    {
        const uint32_t a = static_cast<uint32_t>(value / 10000000000000000); // 1 to 1844
        value %= 10000000000000000;

        if (a < 10)
        {
            *buffer++ = '0' + static_cast<char>(a);
        }
        else if (a < 100)
        {
            const uint32_t i = a << 1;
            *buffer++ = gDigitsLut[i];
            *buffer++ = gDigitsLut[i + 1];
        }
        else if (a < 1000)
        {
            *buffer++ = '0' + static_cast<char>(a / 100);

            const uint32_t i = (a % 100) << 1;
            *buffer++ = gDigitsLut[i];
            *buffer++ = gDigitsLut[i + 1];
        }
        else
        {
            const uint32_t i = (a / 100) << 1;
            const uint32_t j = (a % 100) << 1;
            *buffer++ = gDigitsLut[i];
            *buffer++ = gDigitsLut[i + 1];
            *buffer++ = gDigitsLut[j];
            *buffer++ = gDigitsLut[j + 1];
        }

        const uint32_t v0 = static_cast<uint32_t>(value / 100000000);
        const uint32_t v1 = static_cast<uint32_t>(value % 100000000);

        const uint32_t b0 = v0 / 10000;
        const uint32_t c0 = v0 % 10000;

        const uint32_t d1 = (b0 / 100) << 1;
        const uint32_t d2 = (b0 % 100) << 1;

        const uint32_t d3 = (c0 / 100) << 1;
        const uint32_t d4 = (c0 % 100) << 1;

        const uint32_t b1 = v1 / 10000;
        const uint32_t c1 = v1 % 10000;

        const uint32_t d5 = (b1 / 100) << 1;
        const uint32_t d6 = (b1 % 100) << 1;

        const uint32_t d7 = (c1 / 100) << 1;
        const uint32_t d8 = (c1 % 100) << 1;

        *buffer++ = gDigitsLut[d1];
        *buffer++ = gDigitsLut[d1 + 1];
        *buffer++ = gDigitsLut[d2];
        *buffer++ = gDigitsLut[d2 + 1];
        *buffer++ = gDigitsLut[d3];
        *buffer++ = gDigitsLut[d3 + 1];
        *buffer++ = gDigitsLut[d4];
        *buffer++ = gDigitsLut[d4 + 1];
        *buffer++ = gDigitsLut[d5];
        *buffer++ = gDigitsLut[d5 + 1];
        *buffer++ = gDigitsLut[d6];
        *buffer++ = gDigitsLut[d6 + 1];
        *buffer++ = gDigitsLut[d7];
        *buffer++ = gDigitsLut[d7 + 1];
        *buffer++ = gDigitsLut[d8];
        *buffer++ = gDigitsLut[d8 + 1];
    }

    *buffer = '\0';
}

static void i64toa_branchlut(int64_t value, char* buffer)
{
    uint64_t u = static_cast<uint64_t>(value);
    if (value < 0)
    {
        *buffer++ = '-';
        u = ~u + 1;
    }

    u64toa_branchlut(u, buffer);
}


namespace ts
{
namespace sz
{

static void to_string(std::string& dst, int8_t v)
{
    char buffer[32];
    //sprintf(buffer, "%d", int(v));
    i32toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, uint8_t v)
{
    char buffer[32];
    //sprintf(buffer, "%d", int(v));
    u32toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, int16_t v)
{
    char buffer[32];
    //sprintf(buffer, "%d", int(v));
    i32toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, uint16_t v)
{
    char buffer[32];
    //sprintf(buffer, "%d", int(v));
    u32toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, int32_t v)
{
    char buffer[32];
    //sprintf(buffer, "%d", v);
    i32toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, uint32_t v)
{
    char buffer[32];
    //sprintf(buffer, "%u", v);
    u32toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, int64_t v)
{
    char buffer[32];
    //sprintf(buffer, "%lld", v);
    i64toa_branchlut(v, buffer);
    dst += buffer;
}
static void to_string(std::string& dst, uint64_t v)
{
    char buffer[32];
    //sprintf(buffer, "%llu", v);
    u64toa_branchlut(v, buffer);
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
    case Value::Type::EMPTY: dst.append("null", 4); break;
    case Value::Type::BOOL:
        if (value.get_as_bool())
        {
            dst.append("true", 4);
        }
        else
        {
            dst.append("false", 5);
        }
        break;
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
            if (nice)
            {
                dst += member_ident_str;
            }
            dst.push_back('"');
            dst += value.get_object_member_name(i);
            dst.append("\":", 2);
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
        if (nice)
        {
            dst += ident_str;
        }
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
            if (nice)
            {
                dst += element_ident_str;
            }
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
        if (nice)
        {
            dst += ident_str;
        }
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
    if (json_ptr + 4 > json_end)
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
    if (json_ptr + 5 > json_end)
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
    if (json_ptr + 4 > json_end)
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

    if (decimal_digit_count == 0 && exponent_sign > 0)
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

        int64_t value = whole * sign * s_pow[exponent];
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

    if (exponent_sign > 0)
    {
        value *= s_pow[exponent];
    }
    else
    {
        value /= s_pow[exponent];
    }

    value *= sign;

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

std::string& to_json(std::string& dst, Value const& value, bool nice)
{
    dst.clear();
    dst.reserve(32768);
    to_json(dst, value, 1, nice);
    return dst;
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

#include "Serialization.h"
#include "ts_assert.h"

namespace ts
{
namespace serialization
{

Value::~Value()
{
    destruct();
}

Value::Value(Type type) noexcept
    : type(type)
{
    construct();
}
Value::Value(bool        value) noexcept
    : type(Type::BOOL)
    , bool_value(value)
{
}
Value::Value(int8_t      value) noexcept
    : type(Type::INT8)
    , int8_value(value)
{
}
Value::Value(uint8_t     value) noexcept
    : type(Type::UINT8)
    , uint8_value(value)
{
}
Value::Value(int16_t     value) noexcept
    : type(Type::INT16)
    , int16_value(value)
{
}
Value::Value(uint16_t    value) noexcept
    : type(Type::UINT16)
    , uint16_value(value)
{
}
Value::Value(int32_t     value) noexcept
    : type(Type::INT32)
    , int32_value(value)
{
}
Value::Value(uint32_t    value) noexcept
    : type(Type::UINT32)
    , uint32_value(value)
{
}
Value::Value(int64_t     value) noexcept
    : type(Type::INT64)
    , int64_value(value)
{
}
Value::Value(uint64_t    value) noexcept
    : type(Type::UINT64)
    , uint64_value(value)
{
}
Value::Value(float       value) noexcept
    : type(Type::FLOAT)
    , float_value(value)
{
}
Value::Value(double      value) noexcept
    : type(Type::DOUBLE)
    , double_value(value)
{
}
Value::Value(std::string const& value) noexcept
    : type(Type::STRING)
    , string_value(value)
{
}
Value::Value(std::string&& value) noexcept
    : type(Type::STRING)
    , string_value(std::move(value))
{
}
Value::Value(Value const& other) noexcept
    : type(other.type)
{
    construct();
    switch (type)
    {
    case Type::EMPTY: break;
    case Type::BOOL: bool_value = other.bool_value; break;
    case Type::INT8: int8_value = other.int8_value; break;
    case Type::UINT8: uint8_value = other.uint8_value; break;
    case Type::INT16: int16_value = other.int16_value; break;
    case Type::UINT16: uint16_value = other.uint16_value; break;
    case Type::INT32: int32_value = other.int32_value; break;
    case Type::UINT32: uint32_value = other.uint32_value; break;
    case Type::INT64: int64_value = other.int64_value; break;
    case Type::UINT64: uint64_value = other.uint64_value; break;
    case Type::FLOAT: float_value = other.float_value; break;
    case Type::DOUBLE: double_value = other.double_value; break;
    case Type::STRING: string_value = other.string_value; break;
    case Type::OBJECT: object_value = other.object_value; break;
    case Type::ARRAY: array_value = other.array_value; break;
    default: TS_ASSERT(false); break;
    }
}

Value::Value(Value&& other) noexcept
    : type(other.type)
{
    construct();
    switch (type)
    {
    case Type::EMPTY: break;
    case Type::BOOL: bool_value = other.bool_value; break;
    case Type::INT8: int8_value = other.int8_value; break;
    case Type::UINT8: uint8_value = other.uint8_value; break;
    case Type::INT16: int16_value = other.int16_value; break;
    case Type::UINT16: uint16_value = other.uint16_value; break;
    case Type::INT32: int32_value = other.int32_value; break;
    case Type::UINT32: uint32_value = other.uint32_value; break;
    case Type::INT64: int64_value = other.int64_value; break;
    case Type::UINT64: uint64_value = other.uint64_value; break;
    case Type::FLOAT: float_value = other.float_value; break;
    case Type::DOUBLE: double_value = other.double_value; break;
    case Type::STRING: string_value = std::move(other.string_value); break;
    case Type::OBJECT: object_value = std::move(other.object_value); break;
    case Type::ARRAY: array_value = std::move(other.array_value); break;
    default: TS_ASSERT(false); break;
    }
}
Value& Value::operator=(Value const& other) noexcept
{
    *this = Value(other); //using the move;
    return *this;
}

Value& Value::operator=(Value&& other) noexcept
{
    destruct();

    type = other.type;
    construct();
    switch (type)
    {
    case Type::EMPTY: break;
    case Type::BOOL: bool_value = other.bool_value; break;
    case Type::INT8: int8_value = other.int8_value; break;
    case Type::UINT8: uint8_value = other.uint8_value; break;
    case Type::INT16: int16_value = other.int16_value; break;
    case Type::UINT16: uint16_value = other.uint16_value; break;
    case Type::INT32: int32_value = other.int32_value; break;
    case Type::UINT32: uint32_value = other.uint32_value; break;
    case Type::INT64: int64_value = other.int64_value; break;
    case Type::UINT64: uint64_value = other.uint64_value; break;
    case Type::FLOAT: float_value = other.float_value; break;
    case Type::DOUBLE: double_value = other.double_value; break;
    case Type::STRING: string_value = std::move(other.string_value); break;
    case Type::OBJECT: object_value = std::move(other.object_value); break;
    case Type::ARRAY: array_value = std::move(other.array_value); break;
    default: TS_ASSERT(false); break;
    }
    return *this;
}

Value::Type Value::get_type() const
{
    return type;
}

bool Value::get_as_bool() const
{
    TS_ASSERT(type == Type::BOOL);
    return bool_value;
}
int8_t Value::get_as_int8() const
{
    TS_ASSERT(type == Type::INT8);
    return int8_value;
}
uint8_t Value::get_as_uint8() const
{
    TS_ASSERT(type == Type::UINT8);
    return uint8_value;
}
int16_t Value::get_as_int16() const
{
    TS_ASSERT(type == Type::INT16);
    return int16_value;
}
uint16_t Value::get_as_uint16() const
{
    TS_ASSERT(type == Type::UINT16);
    return uint16_value;
}
int32_t Value::get_as_int32() const
{
    TS_ASSERT(type == Type::INT32);
    return int32_value;
}
uint32_t Value::get_as_uint32() const
{
    TS_ASSERT(type == Type::UINT32);
    return uint32_value;
}
int64_t Value::get_as_int64() const
{
    TS_ASSERT(type == Type::INT64);
    return int64_value;
}
uint64_t Value::get_as_uint64() const
{
    TS_ASSERT(type == Type::UINT64);
    return int64_value;
}
float Value::get_as_float() const
{
    TS_ASSERT(type == Type::FLOAT);
    return float_value;
}
double Value::get_as_double() const
{
    TS_ASSERT(type == Type::DOUBLE);
    return double_value;
}
std::string const& Value::get_as_string() const
{
    TS_ASSERT(type == Type::STRING);
    return string_value;
}
std::string&& Value::extract_as_string()
{
    TS_ASSERT(type == Type::STRING);
    return std::move(string_value);
}

void Value::add_object_member(std::string const& name, Value const& member)
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.emplace_back(name, member);
}

void Value::add_object_member(std::string const& name, Value&& member)
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.emplace_back(name, std::move(member));
}

void Value::add_object_member(std::string&& name, Value&& member)
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.emplace_back(std::move(name), std::move(member));
}

size_t Value::get_object_member_count() const
{
    TS_ASSERT(type == Type::OBJECT);
    return object_value.size();
}
std::string const& Value::get_object_member_name(size_t idx) const
{
    TS_ASSERT(type == Type::OBJECT);
    return object_value[idx].first;
}
Value const& Value::get_object_member_value(size_t idx) const
{
    TS_ASSERT(type == Type::OBJECT);
    return object_value[idx].second;
}

void Value::add_array_element(Value const& member)
{
    TS_ASSERT(type == Type::ARRAY);
    array_value.emplace_back(member);
}
void Value::add_array_element(Value&& member)
{
    TS_ASSERT(type == Type::ARRAY);
    array_value.emplace_back(std::move(member));
}

size_t Value::get_array_element_count() const
{
    TS_ASSERT(type == Type::ARRAY);
    return array_value.size();
}
Value const& Value::get_array_element_value(size_t idx) const
{
    TS_ASSERT(type == Type::ARRAY);
    return array_value[idx];
}

void Value::destruct()
{
    switch (type)
    {
    case Type::STRING:
    {
        string_value.~string_type();
        break;
    }
    case Type::OBJECT:
    {
        object_value.~object_type();
        break;
    }
    case Type::ARRAY:
    {
        array_value.~array_type();
        break;
    }
    default: break;
    }
}

void Value::construct()
{
    if (type == Type::STRING)
    {
    }
    switch (type)
    {
    case Type::STRING:
    {
        new (&string_value) string_type;
        break;
    }
    case Type::OBJECT:
    {
        new (&object_value) object_type;
        break;
    }
    case Type::ARRAY:
    {
        new (&array_value) array_type;
        break;
    }
    default: break;
    }
}

}
}

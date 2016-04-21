#include "ISerializer.h"
#include "ts_assert.h"

namespace ts
{

ISerializer::Value::~Value()
{
    destruct();
}

ISerializer::Value::Value(Type type)
    : type(type)
{
    construct();
}
ISerializer::Value::Value(bool        value)
    : type(Type::BOOL)
    , bool_value(value)
{
}
ISerializer::Value::Value(int8_t      value)
    : type(Type::INT8)
    , int8_value(value)
{
}
ISerializer::Value::Value(uint8_t     value)
    : type(Type::UINT8)
    , uint8_value(value)
{
}
ISerializer::Value::Value(int16_t     value)
    : type(Type::INT16)
    , int16_value(value)
{
}
ISerializer::Value::Value(uint16_t    value)
    : type(Type::UINT16)
    , uint16_value(value)
{
}
ISerializer::Value::Value(int32_t     value)
    : type(Type::INT32)
    , int32_value(value)
{
}
ISerializer::Value::Value(uint32_t    value)
    : type(Type::UINT32)
    , uint32_value(value)
{
}
ISerializer::Value::Value(int64_t     value)
    : type(Type::INT64)
    , int64_value(value)
{
}
ISerializer::Value::Value(uint64_t    value)
    : type(Type::UINT64)
    , uint64_value(value)
{
}
ISerializer::Value::Value(float       value)
    : type(Type::FLOAT)
    , float_value(value)
{
}
ISerializer::Value::Value(double      value)
    : type(Type::DOUBLE)
    , double_value(value)
{
}
ISerializer::Value::Value(std::string const& value)
    : type(Type::STRING)
    , string_value(value)
{
}

ISerializer::Value::Value(Value const& other)
    : type(other.type)
{
    construct();
    switch (type)
    {
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

ISerializer::Value::Value(Value&& other)
    : type(other.type)
{
    construct();
    switch (type)
    {
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
ISerializer::Value& ISerializer::Value::operator=(Value const& other)
{
    *this = Value(other); //using the move;
    return *this;
}

ISerializer::Value& ISerializer::Value::operator=(Value&& other)
{
    destruct();

    type = other.type;
    construct();
    switch (type)
    {
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

bool ISerializer::Value::get_as_bool() const
{
    TS_ASSERT(type == Type::BOOL);
    return bool_value;
}
int8_t ISerializer::Value::get_as_int8() const
{
    TS_ASSERT(type == Type::INT8);
    return int8_value;
}
uint8_t ISerializer::Value::get_as_uint8() const
{
    TS_ASSERT(type == Type::UINT8);
    return uint8_value;
}
int16_t ISerializer::Value::get_as_int16() const
{
    TS_ASSERT(type == Type::INT16);
    return int16_value;
}
uint16_t ISerializer::Value::get_as_uint16() const
{
    TS_ASSERT(type == Type::UINT16);
    return uint16_value;
}
int32_t ISerializer::Value::get_as_int32() const
{
    TS_ASSERT(type == Type::INT32);
    return int32_value;
}
uint32_t ISerializer::Value::get_as_uint32() const
{
    TS_ASSERT(type == Type::UINT32);
    return uint32_value;
}
int64_t ISerializer::Value::get_as_int64() const
{
    TS_ASSERT(type == Type::INT64);
    return int64_value;
}
uint64_t ISerializer::Value::get_as_uint64() const
{
    TS_ASSERT(type == Type::UINT64);
    return int64_value;
}
float ISerializer::Value::get_as_float() const
{
    TS_ASSERT(type == Type::FLOAT);
    return float_value;
}
double ISerializer::Value::get_as_double() const
{
    TS_ASSERT(type == Type::DOUBLE);
    return double_value;
}
std::string const& ISerializer::Value::get_as_string() const
{
    TS_ASSERT(type == Type::STRING);
    return string_value;
}


void ISerializer::Value::add_object_member(std::string const& name, Value const& member)
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.insert(std::make_pair(name, member));
}

void ISerializer::Value::add_object_member(std::string const& name, Value&& member)
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.insert(std::make_pair(name, std::move(member)));
}

void ISerializer::Value::add_array_element(Value const& member)
{
    TS_ASSERT(type == Type::ARRAY);
    array_value.push_back(member);
}
void ISerializer::Value::add_array_element(Value&& member)
{
    TS_ASSERT(type == Type::ARRAY);
    array_value.push_back(std::move(member));
}

void ISerializer::Value::destruct()
{
    switch (type)
    {
    case Type::STRING:
    {
        typedef std::string type;
        string_value.~type();
        break;
    }
    case Type::OBJECT:
    {
        typedef std::map<std::string, Value> type;
        object_value.~type();
        break;
    }
    case Type::ARRAY:
    {
        typedef std::vector<Value> type;
        array_value.~type();
        break;
    }
    default: break;
    }
}

void ISerializer::Value::construct()
{
    if (type == Type::STRING)
    {
    }
    switch (type)
    {
    case Type::STRING:
    {
        typedef std::string type;
        new (&string_value) type;
        break;
    }
    case Type::OBJECT:
    {
        typedef std::map<std::string, Value> type;
        new (&object_value) type;
        break;
    }
    case Type::ARRAY:
    {
        typedef std::vector<Value> type;
        new (&array_value) type;
        break;
    }
    default: break;
    }
}

std::string ISerializer::Value::to_string() const
{
    switch (type)
    {
    case Type::BOOL: return bool_value ? "true" : "false"; break;
    case Type::INT8: return std::to_string(int8_value); break;
    case Type::UINT8: return std::to_string(uint8_value); break;
    case Type::INT16: return std::to_string(int16_value); break;
    case Type::UINT16: return std::to_string(uint16_value); break;
    case Type::INT32: return std::to_string(int32_value); break;
    case Type::UINT32: return std::to_string(uint32_value); break;
    case Type::INT64: return std::to_string(int64_value); break;
    case Type::UINT64: return std::to_string(uint64_value); break;
    case Type::FLOAT: return std::to_string(float_value); break;
    case Type::DOUBLE: return std::to_string(double_value); break;
    case Type::STRING: return "\"" + string_value + "\""; break;
    default: TS_ASSERT(false); break;
    }
}

}

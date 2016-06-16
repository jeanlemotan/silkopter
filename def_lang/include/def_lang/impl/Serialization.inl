namespace ts
{
namespace sz
{

inline Value::Value() noexcept
    : type(Type::EMPTY)
{
    construct();
}

inline Value::~Value()
{
    destruct();
}

inline Value::Value(Type type) noexcept
    : type(type)
{
    construct();
}
inline Value::Value(bool        value) noexcept
    : type(Type::BOOL)
    , bool_value(value)
{
}
inline Value::Value(int8_t      value) noexcept
    : type(Type::INT8)
    , int8_value(value)
{
}
inline Value::Value(uint8_t     value) noexcept
    : type(Type::UINT8)
    , uint8_value(value)
{
}
inline Value::Value(int16_t     value) noexcept
    : type(Type::INT16)
    , int16_value(value)
{
}
inline Value::Value(uint16_t    value) noexcept
    : type(Type::UINT16)
    , uint16_value(value)
{
}
inline Value::Value(int32_t     value) noexcept
    : type(Type::INT32)
    , int32_value(value)
{
}
inline Value::Value(uint32_t    value) noexcept
    : type(Type::UINT32)
    , uint32_value(value)
{
}
inline Value::Value(int64_t     value) noexcept
    : type(Type::INT64)
    , int64_value(value)
{
}
inline Value::Value(uint64_t    value) noexcept
    : type(Type::UINT64)
    , uint64_value(value)
{
}
inline Value::Value(float       value) noexcept
    : type(Type::FLOAT)
    , float_value(value)
{
}
inline Value::Value(double      value) noexcept
    : type(Type::DOUBLE)
    , double_value(value)
{
}
inline Value::Value(std::string const& value) noexcept
    : type(Type::STRING)
    , string_value(value)
{
}
inline Value::Value(char const* value) noexcept
    : type(Type::STRING)
    , string_value(value ? value : "")
{
}
inline Value::Value(std::string&& value) noexcept
    : type(Type::STRING)
    , string_value(std::move(value))
{
}
inline Value::Value(Value const& other) noexcept
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

inline Value::Value(Value&& other) noexcept
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
    case Type::STRING:
        string_value = std::move(other.string_value);
        other.string_value.~string_type();
        other.type = Type::EMPTY;
        break;
    case Type::OBJECT:
        object_value = std::move(other.object_value);
        other.object_value.~object_type();
        other.type = Type::EMPTY;
        break;
    case Type::ARRAY:
        array_value = std::move(other.array_value);
        other.array_value.~array_type();
        other.type = Type::EMPTY;
        break;
    default: TS_ASSERT(false); break;
    }
}
inline Value& Value::operator=(Value const& other) noexcept
{
    *this = Value(other); //using the move;
    return *this;
}

inline Value& Value::operator=(Value&& other) noexcept
{
    if (type != other.type)
    {
        destruct();
        type = other.type;
        construct();
    }
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
    case Type::STRING:
        string_value = std::move(other.string_value);
        other.string_value.~string_type();
        other.type = Type::EMPTY;
        break;
    case Type::OBJECT:
        object_value = std::move(other.object_value);
        other.object_value.~object_type();
        other.type = Type::EMPTY;
        break;
    case Type::ARRAY:
        array_value = std::move(other.array_value);
        other.array_value.~array_type();
        other.type = Type::EMPTY;
        break;
    default: TS_ASSERT(false); break;
    }
    return *this;
}

inline Value::Type Value::get_type() const noexcept
{
    return type;
}

inline bool Value::get_as_bool() const noexcept
{
    TS_ASSERT(type == Type::BOOL);
    return bool_value;
}
inline int8_t Value::get_as_int8() const noexcept
{
    TS_ASSERT(type == Type::INT8);
    return int8_value;
}
inline uint8_t Value::get_as_uint8() const noexcept
{
    TS_ASSERT(type == Type::UINT8);
    return uint8_value;
}
inline int16_t Value::get_as_int16() const noexcept
{
    TS_ASSERT(type == Type::INT16);
    return int16_value;
}
inline uint16_t Value::get_as_uint16() const noexcept
{
    TS_ASSERT(type == Type::UINT16);
    return uint16_value;
}
inline int32_t Value::get_as_int32() const noexcept
{
    TS_ASSERT(type == Type::INT32);
    return int32_value;
}
inline uint32_t Value::get_as_uint32() const noexcept
{
    TS_ASSERT(type == Type::UINT32);
    return uint32_value;
}
inline int64_t Value::get_as_int64() const noexcept
{
    TS_ASSERT(type == Type::INT64);
    return int64_value;
}
inline uint64_t Value::get_as_uint64() const noexcept
{
    TS_ASSERT(type == Type::UINT64);
    return uint64_value;
}
inline float Value::get_as_float() const noexcept
{
    TS_ASSERT(type == Type::FLOAT);
    return float_value;
}
inline double Value::get_as_double() const noexcept
{
    TS_ASSERT(type == Type::DOUBLE);
    return double_value;
}
inline double Value::get_as_number() const noexcept
{
    switch (type)
    {
    case Type::INT8: return static_cast<double>(get_as_int8());
    case Type::UINT8: return static_cast<double>(get_as_uint8());
    case Type::INT16: return static_cast<double>(get_as_int16());
    case Type::UINT16: return static_cast<double>(get_as_uint16());
    case Type::INT32: return static_cast<double>(get_as_int32());
    case Type::UINT32: return static_cast<double>(get_as_uint32());
    case Type::INT64: return static_cast<double>(get_as_int64());
    case Type::UINT64: return static_cast<double>(get_as_uint64());
    case Type::FLOAT: return static_cast<double>(get_as_float());
    default: return get_as_double();
    }
}
inline double Value::get_as_real_number() const noexcept
{
    switch (type)
    {
    case Type::FLOAT: return get_as_float();
    default: return get_as_double();
    }
}
inline int64_t Value::get_as_integral_number() const noexcept
{
    switch (type)
    {
    case Type::INT8: return get_as_int8();
    case Type::UINT8: return get_as_uint8();
    case Type::INT16: return get_as_int16();
    case Type::UINT16: return get_as_uint16();
    case Type::INT32: return get_as_int32();
    case Type::UINT32: return get_as_uint32();
    case Type::INT64: return get_as_int64();
    default: return get_as_uint64();
    }
}
inline std::string const& Value::get_as_string() const noexcept
{
    TS_ASSERT(type == Type::STRING);
    return string_value;
}
inline std::string&& Value::extract_as_string() noexcept
{
    TS_ASSERT(type == Type::STRING);
    return std::move(string_value);
}

inline void Value::add_object_member(std::string const& name, Value const& member) noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.emplace_back(name, member);
}

inline void Value::add_object_member(std::string const& name, Value&& member) noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.emplace_back(name, std::move(member));
}

inline void Value::add_object_member(std::string&& name, Value&& member) noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    object_value.emplace_back(std::move(name), std::move(member));
}

inline size_t Value::get_object_member_count() const noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    return object_value.size();
}
inline std::string const& Value::get_object_member_name(size_t idx) const noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    return object_value[idx].first;
}
inline Value const& Value::get_object_member_value(size_t idx) const noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    return object_value[idx].second;
}
inline Value const* Value::find_object_member_by_name(std::string const& name) const noexcept
{
    TS_ASSERT(type == Type::OBJECT);
    auto it = std::find_if(object_value.begin(), object_value.end(), [&name](std::pair<string_type, Value> const& p) { return p.first == name; });
    return it != object_value.end() ? &it->second : nullptr;
}

inline void Value::add_array_element(Value const& member) noexcept
{
    TS_ASSERT(type == Type::ARRAY);
    array_value.emplace_back(member);
}
inline void Value::add_array_element(Value&& member) noexcept
{
    TS_ASSERT(type == Type::ARRAY);
    array_value.emplace_back(std::move(member));
}

inline size_t Value::get_array_element_count() const noexcept
{
    TS_ASSERT(type == Type::ARRAY);
    return array_value.size();
}
inline Value const& Value::get_array_element_value(size_t idx) const noexcept
{
    TS_ASSERT(type == Type::ARRAY);
    return array_value[idx];
}

inline bool Value::is_empty() const noexcept
{
    return type == Type::EMPTY;
}
inline bool Value::is_bool() const noexcept
{
    return type == Type::BOOL;
}
inline bool Value::is_number() const noexcept
{
    return is_integral_number() || is_real_number();
}
inline bool Value::is_integral_number() const noexcept
{
    return type == Type::INT8 || type == Type::UINT8 ||
            type == Type::INT16 || type == Type::UINT16 ||
            type == Type::INT32 || type == Type::UINT32 ||
            type == Type::INT64 || type == Type::UINT64;
}
inline bool Value::is_real_number() const noexcept
{
    return type == Type::FLOAT || type == Type::DOUBLE;
}
inline bool Value::is_string() const noexcept
{
    return type == Type::STRING;
}
inline bool Value::is_object() const noexcept
{
    return type == Type::OBJECT;
}
inline bool Value::is_array() const noexcept
{
    return type == Type::ARRAY;
}

inline void Value::destruct() noexcept
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

inline void Value::construct() noexcept
{
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
        object_value.reserve(32);
        break;
    }
    case Type::ARRAY:
    {
        new (&array_value) array_type;
        array_value.reserve(32);
        break;
    }
    default: break;
    }
}

}
}

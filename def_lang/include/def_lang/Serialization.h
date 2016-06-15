#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include "ts_assert.h"

namespace ts
{
namespace sz
{
struct Value final
{
    //----------------------

    enum class Type : uint8_t
    {
        BOOL,
        EMPTY,
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        INT64,
        UINT64,
        FLOAT,
        DOUBLE,
        STRING,
        OBJECT,
        ARRAY
    };

    ~Value();

    Value() noexcept; //EMPTY
    Value(Type      type) noexcept;
    Value(bool      value) noexcept;
    Value(int8_t    value) noexcept;
    Value(uint8_t   value) noexcept;
    Value(int16_t   value) noexcept;
    Value(uint16_t  value) noexcept;
    Value(int32_t   value) noexcept;
    Value(uint32_t  value) noexcept;
    Value(int64_t   value) noexcept;
    Value(uint64_t  value) noexcept;
    Value(float     value) noexcept;
    Value(double    value) noexcept;
    Value(char const* value) noexcept;
    Value(std::string const& value) noexcept;
    Value(std::string&& value) noexcept;

    Value(Value const& other) noexcept;
    Value(Value&& other) noexcept;
    Value& operator=(Value const& other) noexcept;
    Value& operator=(Value&& other) noexcept;

    //----------------------

    Type    get_type() const noexcept;

    bool    is_empty() const noexcept;
    bool    is_bool() const noexcept;
    bool    is_number() const noexcept;
    bool    is_integral_number() const noexcept;
    bool    is_real_number() const noexcept;
    bool    is_string() const noexcept;
    bool    is_object() const noexcept;
    bool    is_array() const noexcept;

    bool        get_as_bool() const noexcept;
    int8_t      get_as_int8() const noexcept;
    uint8_t     get_as_uint8() const noexcept;
    int16_t     get_as_int16() const noexcept;
    uint16_t    get_as_uint16() const noexcept;
    int32_t     get_as_int32() const noexcept;
    uint32_t    get_as_uint32() const noexcept;
    int64_t     get_as_int64() const noexcept;
    uint64_t    get_as_uint64() const noexcept;
    float       get_as_float() const noexcept;
    double      get_as_double() const noexcept;
    double      get_as_number() const noexcept;
    double      get_as_real_number() const noexcept;
    int64_t     get_as_integral_number() const noexcept;
    std::string const& get_as_string() const noexcept;
    std::string&& extract_as_string() noexcept;

    void add_object_member(std::string const& name, Value const& member) noexcept;
    void add_object_member(std::string const& name, Value&& member) noexcept;
    void add_object_member(std::string&& name, Value&& member) noexcept;
    Value const* find_object_member_by_name(std::string const& name) const noexcept;

    size_t get_object_member_count() const noexcept;
    std::string const& get_object_member_name(size_t idx) const noexcept;
    Value const& get_object_member_value(size_t idx) const noexcept;

    void add_array_element(Value const& member) noexcept;
    void add_array_element(Value&& member) noexcept;

    size_t get_array_element_count() const noexcept;
    Value const& get_array_element_value(size_t idx) const noexcept;

private:
    typedef std::string string_type;
    typedef std::vector<std::pair<string_type, Value>> object_type;
    typedef std::vector<Value> array_type;

    Type type = Type::BOOL;
    union
    {
        bool        bool_value;
        int8_t      int8_value;
        uint8_t     uint8_value;
        int16_t     int16_value;
        uint16_t    uint16_value;
        int32_t     int32_value;
        uint32_t    uint32_value;
        int64_t     int64_value;
        uint64_t    uint64_value;
        float       float_value;
        double      double_value;
        string_type string_value;
        object_type object_value;
        array_type  array_value;
    };

    void construct() noexcept;
    void destruct() noexcept;
};

}
}

#include "impl/Serialization.inl"

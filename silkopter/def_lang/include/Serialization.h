#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>

namespace ts
{
namespace serialization
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
    Value(std::string const& value) noexcept;
    Value(std::string&& value) noexcept;

    Value(Value const& other) noexcept;
    Value(Value&& other) noexcept;
    Value& operator=(Value const& other) noexcept;
    Value& operator=(Value&& other) noexcept;

    //----------------------

    Type    get_type() const;

    bool        get_as_bool() const;
    int8_t      get_as_int8() const;
    uint8_t     get_as_uint8() const;
    int16_t     get_as_int16() const;
    uint16_t    get_as_uint16() const;
    int32_t     get_as_int32() const;
    uint32_t    get_as_uint32() const;
    int64_t     get_as_int64() const;
    uint64_t    get_as_uint64() const;
    float       get_as_float() const;
    double      get_as_double() const;
    std::string const& get_as_string() const;
    std::string&& extract_as_string();

    void add_object_member(std::string const& name, Value const& member);
    void add_object_member(std::string const& name, Value&& member);
    void add_object_member(std::string&& name, Value&& member);

    size_t get_object_member_count() const;
    std::string const& get_object_member_name(size_t idx) const;
    Value const& get_object_member_value(size_t idx) const;

    void add_array_element(Value const& member);
    void add_array_element(Value&& member);

    size_t get_array_element_count() const;
    Value const& get_array_element_value(size_t idx) const;

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

    void construct();
    void destruct();
};

}
}

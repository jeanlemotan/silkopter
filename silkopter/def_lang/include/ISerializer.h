#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include "Result.h"

namespace ts
{

class ISerializer
{
public:
    struct Value final
    {
        //----------------------

        enum class Type : uint8_t
        {
            BOOL,
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

        Value(Type      type);
        Value(bool      value);
        Value(int8_t    value);
        Value(uint8_t   value);
        Value(int16_t   value);
        Value(uint16_t  value);
        Value(int32_t   value);
        Value(uint32_t  value);
        Value(int64_t   value);
        Value(uint64_t  value);
        Value(float     value);
        Value(double    value);
        Value(std::string const& value);

        Value(Value const& other);
        Value(Value&& other);
        Value& operator=(Value const& other);
        Value& operator=(Value&& other);

        //----------------------

        bool get_as_bool() const;
        int8_t get_as_int8() const;
        uint8_t get_as_uint8() const;
        int16_t get_as_int16() const;
        uint16_t get_as_uint16() const;
        int32_t get_as_int32() const;
        uint32_t get_as_uint32() const;
        int64_t get_as_int64() const;
        uint64_t get_as_uint64() const;
        float get_as_float() const;
        double get_as_double() const;
        std::string const& get_as_string() const;

        void add_object_member(std::string const& name, Value const& member);
        void add_object_member(std::string const& name, Value&& member);

        void add_array_element(Value const& member);
        void add_array_element(Value&& member);

        std::string to_string() const;

    private:
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
            std::string string_value;
            std::map<std::string, Value>    object_value;
            std::vector<Value>              array_value;
        };

        void construct();
        void destruct();
    };

    virtual ~ISerializer() = default;

    virtual Result<void> begin_object(std::string const& name, size_t reserve = 0) = 0;
    virtual Result<void> end_object() = 0;

    virtual Result<void> add_member(std::string const& name, Value const& value) = 0;

    virtual Result<void> begin_array(size_t reserve = 0) = 0;
    virtual Result<void> end_array() = 0;

    virtual Result<void> add_element(Value const& value) = 0;
};

}

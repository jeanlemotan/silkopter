#pragma once

#include "IStream.h"
#include "autojsoncxx/boost_types.hpp"
#include "sz_math.hpp"
#include "utils/Json_Util.h"

namespace silk
{
namespace node
{
namespace stream
{



template<class T> struct Bias_Scale_Calibration_Data
{
    T bias = T(0);
    T scale = T(1);
};
struct No_Calibration_Data
{
};

template<class T, class Value>
Value apply(Bias_Scale_Calibration_Data<T> const& calibration_data, Value const& value)
{
    return value * calibration_data.scale + calibration_data.bias;
}
template<class T>
auto deserialize(Bias_Scale_Calibration_Data<T>& calibration_data, rapidjson::Value const& json) -> bool
{
    auto* biasj = jsonutil::find_value(json, std::string("bias"));
    if (!biasj)
    {
        QLOGE("No bias data found");
        return false;
    }
    auto* scalej = jsonutil::find_value(json, std::string("scale"));
    if (!scalej)
    {
        QLOGE("No scale data found");
        return false;
    }

    Bias_Scale_Calibration_Data<T> sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz.bias, *biasj, result) ||
        !autojsoncxx::from_value(sz.scale, *scalej, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize calibration data: {}", ss.str());
        return false;
    }
    calibration_data = sz;
    return true;
}
template<class T>
auto serialize(Bias_Scale_Calibration_Data<T> const& calibration_data, rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) -> bool
{
    json.SetObject();
    auto* biasj = jsonutil::get_or_add_value(json, std::string("bias"), rapidjson::kObjectType, allocator);
    if (!biasj)
    {
        QLOGE("Cannot create bias data");
        return false;
    }
    auto* scalej = jsonutil::get_or_add_value(json, std::string("scale"), rapidjson::kObjectType, allocator);
    if (!scalej)
    {
        QLOGE("Cannot create scale data");
        return false;
    }

    rapidjson::Document d;
    autojsoncxx::to_document(calibration_data.bias, d);
    jsonutil::clone_value(*biasj, d, allocator);
    autojsoncxx::to_document(calibration_data.scale, d);
    jsonutil::clone_value(*scalej, d, allocator);
    return true;
}
template<class T>
auto deserialize(Bias_Scale_Calibration_Data<T>& calibration_data, std::vector<uint8_t> const& binary) -> bool
{
    if (binary.size() < sizeof(calibration_data))
    {
        QLOGE("not enough binary data to deserialize");
        return false;
    }
    calibration_data = *reinterpret_cast<Bias_Scale_Calibration_Data<T> const*>(binary.data());
    return true;
}
template<class T>
auto serialize(Bias_Scale_Calibration_Data<T> const& calibration_data, std::vector<uint8_t>& binary) -> bool
{
    binary.resize(sizeof(calibration_data));
    *reinterpret_cast<Bias_Scale_Calibration_Data<T>*>(binary.data()) = calibration_data;
    return true;
}

template<class Value>
Value apply(No_Calibration_Data const& calibration_data, Value const& value)
{
    return value;
}
inline auto deserialize(No_Calibration_Data& calibration_data, rapidjson::Value const& json) -> bool
{
    return true;
}
inline auto serialize(No_Calibration_Data const& calibration_data, rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) -> bool
{
    json.SetObject();
    return true;
}
inline auto deserialize(No_Calibration_Data& calibration_data, std::vector<uint8_t> const& binary) -> bool
{
    return true;
}
inline auto serialize(No_Calibration_Data const& calibration_data, std::vector<uint8_t>& binary) -> bool
{
    return true;
}


//for streams of data with a fixed sample rate
template <Type TYPE_VALUE, class CALIBRATION_DATA = No_Calibration_Data>
class IScalar_Stream : public IStream
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }

    typedef CALIBRATION_DATA Calibration_Data;
    Calibration_Data calibration_data;

    auto deserialize_calibration_data(rapidjson::Value const& json) -> bool { return deserialize(calibration_data, json); }
    auto serialize_calibration_data(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const -> bool  { return serialize(calibration_data, json, allocator); }
    auto deserialize_calibration_data(std::vector<uint8_t> const& binary) -> bool  { return deserialize(calibration_data, binary); }
    auto serialize_calibration_data(std::vector<uint8_t>& binary) const -> bool  { return serialize(calibration_data, binary); }
};

//for vectorial streams of data with a fixed sample rate
template <Type TYPE_VALUE, Space SPACE_VALUE, class CALIBRATION_DATA = No_Calibration_Data>
class ISpatial_Stream : public IStream
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    static constexpr Space SPACE = SPACE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }

    typedef CALIBRATION_DATA Calibration_Data;
    Calibration_Data calibration_data;

    auto deserialize_calibration_data(rapidjson::Value const& json) -> bool { return deserialize(calibration_data, json); }
    auto serialize_calibration_data(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const -> bool  { return serialize(calibration_data, json, allocator); }
    auto deserialize_calibration_data(std::vector<uint8_t> const& binary) -> bool  { return deserialize(calibration_data, binary); }
    auto serialize_calibration_data(std::vector<uint8_t>& binary) const -> bool  { return serialize(calibration_data, binary); }
};



}
}
}

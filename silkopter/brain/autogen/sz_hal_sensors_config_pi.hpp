#pragma once

// The MIT License (MIT)
//
// Copyright (c) 2014 Siyuan Ren (netheril96@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <autojsoncxx/autojsoncxx.hpp>

// The comments are reserved for replacement
// such syntax is chosen so that the template file looks like valid C++

//struct Config;

namespace autojsoncxx {

template <>
class SAXEventHandler< ::silk::HAL_Sensors_Pi::Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< math::vec3f > handler_0;
SAXEventHandler< math::vec3f > handler_1;
SAXEventHandler< math::vec3f > handler_2;
SAXEventHandler< math::vec3f > handler_3;
SAXEventHandler< std::string > handler_4;
SAXEventHandler< std::string > handler_5;
SAXEventHandler< size_t > handler_6;
SAXEventHandler< size_t > handler_7;bool has_accelerometer_bias;
bool has_accelerometer_scale;
bool has_gyroscope_bias;
bool has_compass_bias;

    bool check_depth(const char* type)
    {
        if (depth <= 0) {
            the_error.reset(new error::TypeMismatchError("object", type));
            return false;
        }
        return true;
    }

    const char* current_member_name() const
    {
        switch (state) {
            case 0:
    return "accelerometer_bias";
case 1:
    return "accelerometer_scale";
case 2:
    return "gyroscope_bias";
case 3:
    return "compass_bias";
case 4:
    return "mpu_i2c_device";
case 5:
    return "barometer_i2c_device";
case 6:
    return "gyroscope_range";
case 7:
    return "accelerometer_range";
        default:
            break;
        }
        return "<UNKNOWN>";
    }

    bool checked_event_forwarding(bool success)
    {
        if (!success)
            the_error.reset(new error::ObjectMemberError(current_member_name()));
        return success;
    }

    void set_missing_required(const char* name)
    {
        if (the_error.empty() || the_error->type() != error::MISSING_REQUIRED)
            the_error.reset(new error::RequiredFieldMissingError());

        std::vector<std::string>& missing =
            static_cast<error::RequiredFieldMissingError*>(the_error.get())->missing_members();

        missing.push_back(name);
    }

    void reset_flags()
    {
        has_accelerometer_bias = false;
has_accelerometer_scale = false;
has_gyroscope_bias = false;
has_compass_bias = false;




    }

public:
    explicit SAXEventHandler( ::silk::HAL_Sensors_Pi::Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->accelerometer_bias)
, handler_1(&obj->accelerometer_scale)
, handler_2(&obj->gyroscope_bias)
, handler_3(&obj->compass_bias)
, handler_4(&obj->mpu_i2c_device)
, handler_5(&obj->barometer_i2c_device)
, handler_6(&obj->gyroscope_range)
, handler_7(&obj->accelerometer_range)
    {
        reset_flags();
    }

    bool Null()
    {
        if (!check_depth("null"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Null());

case 1:
    return checked_event_forwarding(handler_1.Null());

case 2:
    return checked_event_forwarding(handler_2.Null());

case 3:
    return checked_event_forwarding(handler_3.Null());

case 4:
    return checked_event_forwarding(handler_4.Null());

case 5:
    return checked_event_forwarding(handler_5.Null());

case 6:
    return checked_event_forwarding(handler_6.Null());

case 7:
    return checked_event_forwarding(handler_7.Null());

        default:
            break;
        }
        return true;
    }

    bool Bool(bool b)
    {
        if (!check_depth("bool"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Bool(b));

case 1:
    return checked_event_forwarding(handler_1.Bool(b));

case 2:
    return checked_event_forwarding(handler_2.Bool(b));

case 3:
    return checked_event_forwarding(handler_3.Bool(b));

case 4:
    return checked_event_forwarding(handler_4.Bool(b));

case 5:
    return checked_event_forwarding(handler_5.Bool(b));

case 6:
    return checked_event_forwarding(handler_6.Bool(b));

case 7:
    return checked_event_forwarding(handler_7.Bool(b));

        default:
            break;
        }
        return true;
    }

    bool Int(int i)
    {
        if (!check_depth("int"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int(i));

case 1:
    return checked_event_forwarding(handler_1.Int(i));

case 2:
    return checked_event_forwarding(handler_2.Int(i));

case 3:
    return checked_event_forwarding(handler_3.Int(i));

case 4:
    return checked_event_forwarding(handler_4.Int(i));

case 5:
    return checked_event_forwarding(handler_5.Int(i));

case 6:
    return checked_event_forwarding(handler_6.Int(i));

case 7:
    return checked_event_forwarding(handler_7.Int(i));

        default:
            break;
        }
        return true;
    }

    bool Uint(unsigned i)
    {
        if (!check_depth("unsigned"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint(i));

case 1:
    return checked_event_forwarding(handler_1.Uint(i));

case 2:
    return checked_event_forwarding(handler_2.Uint(i));

case 3:
    return checked_event_forwarding(handler_3.Uint(i));

case 4:
    return checked_event_forwarding(handler_4.Uint(i));

case 5:
    return checked_event_forwarding(handler_5.Uint(i));

case 6:
    return checked_event_forwarding(handler_6.Uint(i));

case 7:
    return checked_event_forwarding(handler_7.Uint(i));

        default:
            break;
        }
        return true;
    }

    bool Int64(utility::int64_t i)
    {
        if (!check_depth("int64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Int64(i));

case 1:
    return checked_event_forwarding(handler_1.Int64(i));

case 2:
    return checked_event_forwarding(handler_2.Int64(i));

case 3:
    return checked_event_forwarding(handler_3.Int64(i));

case 4:
    return checked_event_forwarding(handler_4.Int64(i));

case 5:
    return checked_event_forwarding(handler_5.Int64(i));

case 6:
    return checked_event_forwarding(handler_6.Int64(i));

case 7:
    return checked_event_forwarding(handler_7.Int64(i));

        default:
            break;
        }
        return true;
    }

    bool Uint64(utility::uint64_t i)
    {
        if (!check_depth("uint64_t"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Uint64(i));

case 1:
    return checked_event_forwarding(handler_1.Uint64(i));

case 2:
    return checked_event_forwarding(handler_2.Uint64(i));

case 3:
    return checked_event_forwarding(handler_3.Uint64(i));

case 4:
    return checked_event_forwarding(handler_4.Uint64(i));

case 5:
    return checked_event_forwarding(handler_5.Uint64(i));

case 6:
    return checked_event_forwarding(handler_6.Uint64(i));

case 7:
    return checked_event_forwarding(handler_7.Uint64(i));

        default:
            break;
        }
        return true;
    }

    bool Double(double d)
    {
        if (!check_depth("double"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.Double(d));

case 1:
    return checked_event_forwarding(handler_1.Double(d));

case 2:
    return checked_event_forwarding(handler_2.Double(d));

case 3:
    return checked_event_forwarding(handler_3.Double(d));

case 4:
    return checked_event_forwarding(handler_4.Double(d));

case 5:
    return checked_event_forwarding(handler_5.Double(d));

case 6:
    return checked_event_forwarding(handler_6.Double(d));

case 7:
    return checked_event_forwarding(handler_7.Double(d));

        default:
            break;
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("string"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.String(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.String(str, length, copy));

case 2:
    return checked_event_forwarding(handler_2.String(str, length, copy));

case 3:
    return checked_event_forwarding(handler_3.String(str, length, copy));

case 4:
    return checked_event_forwarding(handler_4.String(str, length, copy));

case 5:
    return checked_event_forwarding(handler_5.String(str, length, copy));

case 6:
    return checked_event_forwarding(handler_6.String(str, length, copy));

case 7:
    return checked_event_forwarding(handler_7.String(str, length, copy));

        default:
            break;
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy)
    {
        if (!check_depth("object"))
            return false;

        if (depth == 1) {
            if (0) {
            }
            else if (utility::string_equal(str, length, "\x61\x63\x63\x65\x6c\x65\x72\x6f\x6d\x65\x74\x65\x72\x5f\x62\x69\x61\x73", 18))
						 { state=0; has_accelerometer_bias = true; }
else if (utility::string_equal(str, length, "\x61\x63\x63\x65\x6c\x65\x72\x6f\x6d\x65\x74\x65\x72\x5f\x73\x63\x61\x6c\x65", 19))
						 { state=1; has_accelerometer_scale = true; }
else if (utility::string_equal(str, length, "\x67\x79\x72\x6f\x73\x63\x6f\x70\x65\x5f\x62\x69\x61\x73", 14))
						 { state=2; has_gyroscope_bias = true; }
else if (utility::string_equal(str, length, "\x63\x6f\x6d\x70\x61\x73\x73\x5f\x62\x69\x61\x73", 12))
						 { state=3; has_compass_bias = true; }
else if (utility::string_equal(str, length, "\x6d\x70\x75\x5f\x69\x32\x63\x5f\x64\x65\x76\x69\x63\x65", 14))
						 { state=4;  }
else if (utility::string_equal(str, length, "\x62\x61\x72\x6f\x6d\x65\x74\x65\x72\x5f\x69\x32\x63\x5f\x64\x65\x76\x69\x63\x65", 20))
						 { state=5;  }
else if (utility::string_equal(str, length, "\x67\x79\x72\x6f\x73\x63\x6f\x70\x65\x5f\x72\x61\x6e\x67\x65", 15))
						 { state=6;  }
else if (utility::string_equal(str, length, "\x61\x63\x63\x65\x6c\x65\x72\x6f\x6d\x65\x74\x65\x72\x5f\x72\x61\x6e\x67\x65", 19))
						 { state=7;  }
            else {
                state = -1;
                return true;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

case 1:
    return checked_event_forwarding(handler_1.Key(str, length, copy));

case 2:
    return checked_event_forwarding(handler_2.Key(str, length, copy));

case 3:
    return checked_event_forwarding(handler_3.Key(str, length, copy));

case 4:
    return checked_event_forwarding(handler_4.Key(str, length, copy));

case 5:
    return checked_event_forwarding(handler_5.Key(str, length, copy));

case 6:
    return checked_event_forwarding(handler_6.Key(str, length, copy));

case 7:
    return checked_event_forwarding(handler_7.Key(str, length, copy));

            default:
                break;
            }
        }
        return true;
    }

    bool StartArray()
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.StartArray());

case 1:
    return checked_event_forwarding(handler_1.StartArray());

case 2:
    return checked_event_forwarding(handler_2.StartArray());

case 3:
    return checked_event_forwarding(handler_3.StartArray());

case 4:
    return checked_event_forwarding(handler_4.StartArray());

case 5:
    return checked_event_forwarding(handler_5.StartArray());

case 6:
    return checked_event_forwarding(handler_6.StartArray());

case 7:
    return checked_event_forwarding(handler_7.StartArray());

        default:
            break;
        }
        return true;
    }

    bool EndArray(SizeType length)
    {
        if (!check_depth("array"))
            return false;

        switch (state) {

        case 0:
    return checked_event_forwarding(handler_0.EndArray(length));

case 1:
    return checked_event_forwarding(handler_1.EndArray(length));

case 2:
    return checked_event_forwarding(handler_2.EndArray(length));

case 3:
    return checked_event_forwarding(handler_3.EndArray(length));

case 4:
    return checked_event_forwarding(handler_4.EndArray(length));

case 5:
    return checked_event_forwarding(handler_5.EndArray(length));

case 6:
    return checked_event_forwarding(handler_6.EndArray(length));

case 7:
    return checked_event_forwarding(handler_7.EndArray(length));

        default:
            break;
        }
        return true;
    }

    bool StartObject()
    {
        ++depth;
        if (depth > 1) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.StartObject());

case 1:
    return checked_event_forwarding(handler_1.StartObject());

case 2:
    return checked_event_forwarding(handler_2.StartObject());

case 3:
    return checked_event_forwarding(handler_3.StartObject());

case 4:
    return checked_event_forwarding(handler_4.StartObject());

case 5:
    return checked_event_forwarding(handler_5.StartObject());

case 6:
    return checked_event_forwarding(handler_6.StartObject());

case 7:
    return checked_event_forwarding(handler_7.StartObject());

            default:
                break;
            }
        }
        return true;
    }

    bool EndObject(SizeType length)
    {
        --depth;
        if (depth > 0) {

            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.EndObject(length));

case 1:
    return checked_event_forwarding(handler_1.EndObject(length));

case 2:
    return checked_event_forwarding(handler_2.EndObject(length));

case 3:
    return checked_event_forwarding(handler_3.EndObject(length));

case 4:
    return checked_event_forwarding(handler_4.EndObject(length));

case 5:
    return checked_event_forwarding(handler_5.EndObject(length));

case 6:
    return checked_event_forwarding(handler_6.EndObject(length));

case 7:
    return checked_event_forwarding(handler_7.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_accelerometer_bias) set_missing_required("accelerometer_bias");
if (!has_accelerometer_scale) set_missing_required("accelerometer_scale");
if (!has_gyroscope_bias) set_missing_required("gyroscope_bias");
if (!has_compass_bias) set_missing_required("compass_bias");
        }
        return the_error.empty();
    }

    bool HasError() const
    {
        return !this->the_error.empty();
    }

    bool ReapError(error::ErrorStack& errs)
    {
        if (this->the_error.empty())
            return false;

        errs.push(this->the_error.release());

        switch (state) {

        case 0:
     handler_0.ReapError(errs); break;
case 1:
     handler_1.ReapError(errs); break;
case 2:
     handler_2.ReapError(errs); break;
case 3:
     handler_3.ReapError(errs); break;
case 4:
     handler_4.ReapError(errs); break;
case 5:
     handler_5.ReapError(errs); break;
case 6:
     handler_6.ReapError(errs); break;
case 7:
     handler_7.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        reset_flags();
    }
};

template < class Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579 >
struct Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, ::silk::HAL_Sensors_Pi::Config > {

    void operator()( Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579& w, const ::silk::HAL_Sensors_Pi::Config& value) const
    {
        w.StartObject();

        w.Key("\x61\x63\x63\x65\x6c\x65\x72\x6f\x6d\x65\x74\x65\x72\x5f\x62\x69\x61\x73"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, math::vec3f >()(w, value.accelerometer_bias);
w.Key("\x61\x63\x63\x65\x6c\x65\x72\x6f\x6d\x65\x74\x65\x72\x5f\x73\x63\x61\x6c\x65"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, math::vec3f >()(w, value.accelerometer_scale);
w.Key("\x67\x79\x72\x6f\x73\x63\x6f\x70\x65\x5f\x62\x69\x61\x73"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, math::vec3f >()(w, value.gyroscope_bias);
w.Key("\x63\x6f\x6d\x70\x61\x73\x73\x5f\x62\x69\x61\x73"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, math::vec3f >()(w, value.compass_bias);
w.Key("\x6d\x70\x75\x5f\x69\x32\x63\x5f\x64\x65\x76\x69\x63\x65"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, std::string >()(w, value.mpu_i2c_device);
w.Key("\x62\x61\x72\x6f\x6d\x65\x74\x65\x72\x5f\x69\x32\x63\x5f\x64\x65\x76\x69\x63\x65"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, std::string >()(w, value.barometer_i2c_device);
w.Key("\x67\x79\x72\x6f\x73\x63\x6f\x70\x65\x5f\x72\x61\x6e\x67\x65"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, size_t >()(w, value.gyroscope_range);
w.Key("\x61\x63\x63\x65\x6c\x65\x72\x6f\x6d\x65\x74\x65\x72\x5f\x72\x61\x6e\x67\x65"); Serializer< Writera9543170a7842b30ba5eeca4186896221a7f674ca43ceebbd0aa064c3e402579, size_t >()(w, value.accelerometer_range);

        w.EndObject();
    }

};
}



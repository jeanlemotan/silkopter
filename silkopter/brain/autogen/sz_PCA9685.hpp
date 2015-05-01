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

namespace sz { namespace PCA9685 { struct PWM_Channel {
 uint32_t min;
uint32_t max;
uint32_t range;
uint32_t rate;

explicit PWM_Channel():min(1000), max(2000), range(2500), rate(400) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PCA9685::PWM_Channel > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< uint32_t > handler_0;
SAXEventHandler< uint32_t > handler_1;
SAXEventHandler< uint32_t > handler_2;
SAXEventHandler< uint32_t > handler_3;bool has_min;
bool has_max;
bool has_range;
bool has_rate;

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
    return "min";
case 1:
    return "max";
case 2:
    return "range";
case 3:
    return "rate";
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
        has_min = false;
has_max = false;
has_range = false;
has_rate = false;
    }

public:
    explicit SAXEventHandler( ::sz::PCA9685::PWM_Channel * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->min)
, handler_1(&obj->max)
, handler_2(&obj->range)
, handler_3(&obj->rate)
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
            else if (utility::string_equal(str, length, "\x4d\x69\x6e\x20\x28\x75\x73\x29", 8))
						 { state=0; has_min = true; }
else if (utility::string_equal(str, length, "\x4d\x61\x78\x20\x28\x75\x73\x29", 8))
						 { state=1; has_max = true; }
else if (utility::string_equal(str, length, "\x52\x61\x6e\x67\x65\x20\x28\x75\x73\x29", 10))
						 { state=2; has_range = true; }
else if (utility::string_equal(str, length, "\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 9))
						 { state=3; has_rate = true; }
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

            default:
                break;
            }
        } else {
            if (!has_min) set_missing_required("min");
if (!has_max) set_missing_required("max");
if (!has_range) set_missing_required("range");
if (!has_rate) set_missing_required("rate");
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

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();
handler_1.PrepareForReuse();
handler_2.PrepareForReuse();
handler_3.PrepareForReuse();

    }
};

template < class Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f >
struct Serializer< Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f, ::sz::PCA9685::PWM_Channel > {

    void operator()( Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f& w, const ::sz::PCA9685::PWM_Channel& value) const
    {
        w.StartObject();

        w.Key("\x4d\x69\x6e\x20\x28\x75\x73\x29", 8, false); Serializer< Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f, uint32_t >()(w, value.min);
w.Key("\x4d\x61\x78\x20\x28\x75\x73\x29", 8, false); Serializer< Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f, uint32_t >()(w, value.max);
w.Key("\x52\x61\x6e\x67\x65\x20\x28\x75\x73\x29", 10, false); Serializer< Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f, uint32_t >()(w, value.range);
w.Key("\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 9, false); Serializer< Writer601111eadf11165f127ec047ead24eca14d4adb1d811f80872a79db2b46e225f, uint32_t >()(w, value.rate);

        w.EndObject(4);
    }

};
}


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

namespace sz { namespace PCA9685 { struct Init_Params {
 uint32_t period_micro;
sz::PCA9685::PWM_Channel channel_1;
sz::PCA9685::PWM_Channel channel_2;
sz::PCA9685::PWM_Channel channel_3;
sz::PCA9685::PWM_Channel channel_4;
sz::PCA9685::PWM_Channel channel_5;
sz::PCA9685::PWM_Channel channel_6;
sz::PCA9685::PWM_Channel channel_7;
sz::PCA9685::PWM_Channel channel_8;
sz::PCA9685::PWM_Channel channel_9;
sz::PCA9685::PWM_Channel channel_10;
sz::PCA9685::PWM_Channel channel_11;
sz::PCA9685::PWM_Channel channel_12;
sz::PCA9685::PWM_Channel channel_13;
sz::PCA9685::PWM_Channel channel_14;
sz::PCA9685::PWM_Channel channel_15;
sz::PCA9685::PWM_Channel channel_16;

explicit Init_Params():period_micro(5), channel_1(), channel_2(), channel_3(), channel_4(), channel_5(), channel_6(), channel_7(), channel_8(), channel_9(), channel_10(), channel_11(), channel_12(), channel_13(), channel_14(), channel_15(), channel_16() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PCA9685::Init_Params > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< uint32_t > handler_0;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_1;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_2;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_3;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_4;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_5;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_6;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_7;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_8;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_9;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_10;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_11;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_12;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_13;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_14;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_15;
SAXEventHandler< sz::PCA9685::PWM_Channel > handler_16;bool has_period_micro;

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
    return "period_micro";
case 1:
    return "channel_1";
case 2:
    return "channel_2";
case 3:
    return "channel_3";
case 4:
    return "channel_4";
case 5:
    return "channel_5";
case 6:
    return "channel_6";
case 7:
    return "channel_7";
case 8:
    return "channel_8";
case 9:
    return "channel_9";
case 10:
    return "channel_10";
case 11:
    return "channel_11";
case 12:
    return "channel_12";
case 13:
    return "channel_13";
case 14:
    return "channel_14";
case 15:
    return "channel_15";
case 16:
    return "channel_16";
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
        has_period_micro = false;
















    }

public:
    explicit SAXEventHandler( ::sz::PCA9685::Init_Params * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->period_micro)
, handler_1(&obj->channel_1)
, handler_2(&obj->channel_2)
, handler_3(&obj->channel_3)
, handler_4(&obj->channel_4)
, handler_5(&obj->channel_5)
, handler_6(&obj->channel_6)
, handler_7(&obj->channel_7)
, handler_8(&obj->channel_8)
, handler_9(&obj->channel_9)
, handler_10(&obj->channel_10)
, handler_11(&obj->channel_11)
, handler_12(&obj->channel_12)
, handler_13(&obj->channel_13)
, handler_14(&obj->channel_14)
, handler_15(&obj->channel_15)
, handler_16(&obj->channel_16)
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

case 8:
    return checked_event_forwarding(handler_8.Null());

case 9:
    return checked_event_forwarding(handler_9.Null());

case 10:
    return checked_event_forwarding(handler_10.Null());

case 11:
    return checked_event_forwarding(handler_11.Null());

case 12:
    return checked_event_forwarding(handler_12.Null());

case 13:
    return checked_event_forwarding(handler_13.Null());

case 14:
    return checked_event_forwarding(handler_14.Null());

case 15:
    return checked_event_forwarding(handler_15.Null());

case 16:
    return checked_event_forwarding(handler_16.Null());

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

case 8:
    return checked_event_forwarding(handler_8.Bool(b));

case 9:
    return checked_event_forwarding(handler_9.Bool(b));

case 10:
    return checked_event_forwarding(handler_10.Bool(b));

case 11:
    return checked_event_forwarding(handler_11.Bool(b));

case 12:
    return checked_event_forwarding(handler_12.Bool(b));

case 13:
    return checked_event_forwarding(handler_13.Bool(b));

case 14:
    return checked_event_forwarding(handler_14.Bool(b));

case 15:
    return checked_event_forwarding(handler_15.Bool(b));

case 16:
    return checked_event_forwarding(handler_16.Bool(b));

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

case 8:
    return checked_event_forwarding(handler_8.Int(i));

case 9:
    return checked_event_forwarding(handler_9.Int(i));

case 10:
    return checked_event_forwarding(handler_10.Int(i));

case 11:
    return checked_event_forwarding(handler_11.Int(i));

case 12:
    return checked_event_forwarding(handler_12.Int(i));

case 13:
    return checked_event_forwarding(handler_13.Int(i));

case 14:
    return checked_event_forwarding(handler_14.Int(i));

case 15:
    return checked_event_forwarding(handler_15.Int(i));

case 16:
    return checked_event_forwarding(handler_16.Int(i));

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

case 8:
    return checked_event_forwarding(handler_8.Uint(i));

case 9:
    return checked_event_forwarding(handler_9.Uint(i));

case 10:
    return checked_event_forwarding(handler_10.Uint(i));

case 11:
    return checked_event_forwarding(handler_11.Uint(i));

case 12:
    return checked_event_forwarding(handler_12.Uint(i));

case 13:
    return checked_event_forwarding(handler_13.Uint(i));

case 14:
    return checked_event_forwarding(handler_14.Uint(i));

case 15:
    return checked_event_forwarding(handler_15.Uint(i));

case 16:
    return checked_event_forwarding(handler_16.Uint(i));

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

case 8:
    return checked_event_forwarding(handler_8.Int64(i));

case 9:
    return checked_event_forwarding(handler_9.Int64(i));

case 10:
    return checked_event_forwarding(handler_10.Int64(i));

case 11:
    return checked_event_forwarding(handler_11.Int64(i));

case 12:
    return checked_event_forwarding(handler_12.Int64(i));

case 13:
    return checked_event_forwarding(handler_13.Int64(i));

case 14:
    return checked_event_forwarding(handler_14.Int64(i));

case 15:
    return checked_event_forwarding(handler_15.Int64(i));

case 16:
    return checked_event_forwarding(handler_16.Int64(i));

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

case 8:
    return checked_event_forwarding(handler_8.Uint64(i));

case 9:
    return checked_event_forwarding(handler_9.Uint64(i));

case 10:
    return checked_event_forwarding(handler_10.Uint64(i));

case 11:
    return checked_event_forwarding(handler_11.Uint64(i));

case 12:
    return checked_event_forwarding(handler_12.Uint64(i));

case 13:
    return checked_event_forwarding(handler_13.Uint64(i));

case 14:
    return checked_event_forwarding(handler_14.Uint64(i));

case 15:
    return checked_event_forwarding(handler_15.Uint64(i));

case 16:
    return checked_event_forwarding(handler_16.Uint64(i));

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

case 8:
    return checked_event_forwarding(handler_8.Double(d));

case 9:
    return checked_event_forwarding(handler_9.Double(d));

case 10:
    return checked_event_forwarding(handler_10.Double(d));

case 11:
    return checked_event_forwarding(handler_11.Double(d));

case 12:
    return checked_event_forwarding(handler_12.Double(d));

case 13:
    return checked_event_forwarding(handler_13.Double(d));

case 14:
    return checked_event_forwarding(handler_14.Double(d));

case 15:
    return checked_event_forwarding(handler_15.Double(d));

case 16:
    return checked_event_forwarding(handler_16.Double(d));

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

case 8:
    return checked_event_forwarding(handler_8.String(str, length, copy));

case 9:
    return checked_event_forwarding(handler_9.String(str, length, copy));

case 10:
    return checked_event_forwarding(handler_10.String(str, length, copy));

case 11:
    return checked_event_forwarding(handler_11.String(str, length, copy));

case 12:
    return checked_event_forwarding(handler_12.String(str, length, copy));

case 13:
    return checked_event_forwarding(handler_13.String(str, length, copy));

case 14:
    return checked_event_forwarding(handler_14.String(str, length, copy));

case 15:
    return checked_event_forwarding(handler_15.String(str, length, copy));

case 16:
    return checked_event_forwarding(handler_16.String(str, length, copy));

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
            else if (utility::string_equal(str, length, "\x50\x65\x72\x69\x6f\x64\x20\x28\x75\x73\x29", 11))
						 { state=0; has_period_micro = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31", 9))
						 { state=1;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x32", 9))
						 { state=2;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x33", 9))
						 { state=3;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x34", 9))
						 { state=4;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x35", 9))
						 { state=5;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x36", 9))
						 { state=6;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x37", 9))
						 { state=7;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x38", 9))
						 { state=8;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x39", 9))
						 { state=9;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x30", 10))
						 { state=10;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x31", 10))
						 { state=11;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x32", 10))
						 { state=12;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x33", 10))
						 { state=13;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x34", 10))
						 { state=14;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x35", 10))
						 { state=15;  }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x36", 10))
						 { state=16;  }
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

case 8:
    return checked_event_forwarding(handler_8.Key(str, length, copy));

case 9:
    return checked_event_forwarding(handler_9.Key(str, length, copy));

case 10:
    return checked_event_forwarding(handler_10.Key(str, length, copy));

case 11:
    return checked_event_forwarding(handler_11.Key(str, length, copy));

case 12:
    return checked_event_forwarding(handler_12.Key(str, length, copy));

case 13:
    return checked_event_forwarding(handler_13.Key(str, length, copy));

case 14:
    return checked_event_forwarding(handler_14.Key(str, length, copy));

case 15:
    return checked_event_forwarding(handler_15.Key(str, length, copy));

case 16:
    return checked_event_forwarding(handler_16.Key(str, length, copy));

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

case 8:
    return checked_event_forwarding(handler_8.StartArray());

case 9:
    return checked_event_forwarding(handler_9.StartArray());

case 10:
    return checked_event_forwarding(handler_10.StartArray());

case 11:
    return checked_event_forwarding(handler_11.StartArray());

case 12:
    return checked_event_forwarding(handler_12.StartArray());

case 13:
    return checked_event_forwarding(handler_13.StartArray());

case 14:
    return checked_event_forwarding(handler_14.StartArray());

case 15:
    return checked_event_forwarding(handler_15.StartArray());

case 16:
    return checked_event_forwarding(handler_16.StartArray());

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

case 8:
    return checked_event_forwarding(handler_8.EndArray(length));

case 9:
    return checked_event_forwarding(handler_9.EndArray(length));

case 10:
    return checked_event_forwarding(handler_10.EndArray(length));

case 11:
    return checked_event_forwarding(handler_11.EndArray(length));

case 12:
    return checked_event_forwarding(handler_12.EndArray(length));

case 13:
    return checked_event_forwarding(handler_13.EndArray(length));

case 14:
    return checked_event_forwarding(handler_14.EndArray(length));

case 15:
    return checked_event_forwarding(handler_15.EndArray(length));

case 16:
    return checked_event_forwarding(handler_16.EndArray(length));

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

case 8:
    return checked_event_forwarding(handler_8.StartObject());

case 9:
    return checked_event_forwarding(handler_9.StartObject());

case 10:
    return checked_event_forwarding(handler_10.StartObject());

case 11:
    return checked_event_forwarding(handler_11.StartObject());

case 12:
    return checked_event_forwarding(handler_12.StartObject());

case 13:
    return checked_event_forwarding(handler_13.StartObject());

case 14:
    return checked_event_forwarding(handler_14.StartObject());

case 15:
    return checked_event_forwarding(handler_15.StartObject());

case 16:
    return checked_event_forwarding(handler_16.StartObject());

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

case 8:
    return checked_event_forwarding(handler_8.EndObject(length));

case 9:
    return checked_event_forwarding(handler_9.EndObject(length));

case 10:
    return checked_event_forwarding(handler_10.EndObject(length));

case 11:
    return checked_event_forwarding(handler_11.EndObject(length));

case 12:
    return checked_event_forwarding(handler_12.EndObject(length));

case 13:
    return checked_event_forwarding(handler_13.EndObject(length));

case 14:
    return checked_event_forwarding(handler_14.EndObject(length));

case 15:
    return checked_event_forwarding(handler_15.EndObject(length));

case 16:
    return checked_event_forwarding(handler_16.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_period_micro) set_missing_required("period_micro");
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
case 8:
     handler_8.ReapError(errs); break;
case 9:
     handler_9.ReapError(errs); break;
case 10:
     handler_10.ReapError(errs); break;
case 11:
     handler_11.ReapError(errs); break;
case 12:
     handler_12.ReapError(errs); break;
case 13:
     handler_13.ReapError(errs); break;
case 14:
     handler_14.ReapError(errs); break;
case 15:
     handler_15.ReapError(errs); break;
case 16:
     handler_16.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();
handler_1.PrepareForReuse();
handler_2.PrepareForReuse();
handler_3.PrepareForReuse();
handler_4.PrepareForReuse();
handler_5.PrepareForReuse();
handler_6.PrepareForReuse();
handler_7.PrepareForReuse();
handler_8.PrepareForReuse();
handler_9.PrepareForReuse();
handler_10.PrepareForReuse();
handler_11.PrepareForReuse();
handler_12.PrepareForReuse();
handler_13.PrepareForReuse();
handler_14.PrepareForReuse();
handler_15.PrepareForReuse();
handler_16.PrepareForReuse();

    }
};

template < class Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140 >
struct Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, ::sz::PCA9685::Init_Params > {

    void operator()( Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140& w, const ::sz::PCA9685::Init_Params& value) const
    {
        w.StartObject();

        w.Key("\x50\x65\x72\x69\x6f\x64\x20\x28\x75\x73\x29", 11, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, uint32_t >()(w, value.period_micro);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_1);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x32", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_2);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x33", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_3);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x34", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_4);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x35", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_5);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x36", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_6);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x37", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_7);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x38", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_8);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x39", 9, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_9);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x30", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_10);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x31", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_11);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x32", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_12);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x33", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_13);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x34", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_14);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x35", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_15);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x36", 10, false); Serializer< Writerbb7c192d993aa227c86efd2f4dbc1aa7e55bb48f1928dad96277e9fb2ec07140, sz::PCA9685::PWM_Channel >()(w, value.channel_16);

        w.EndObject(17);
    }

};
}


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

namespace sz { namespace PCA9685 { struct Inputs {
 std::string channel_1;
std::string channel_2;
std::string channel_3;
std::string channel_4;
std::string channel_5;
std::string channel_6;
std::string channel_7;
std::string channel_8;
std::string channel_9;
std::string channel_10;
std::string channel_11;
std::string channel_12;
std::string channel_13;
std::string channel_14;
std::string channel_15;
std::string channel_16;

explicit Inputs():channel_1(), channel_2(), channel_3(), channel_4(), channel_5(), channel_6(), channel_7(), channel_8(), channel_9(), channel_10(), channel_11(), channel_12(), channel_13(), channel_14(), channel_15(), channel_16() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PCA9685::Inputs > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::string > handler_0;
SAXEventHandler< std::string > handler_1;
SAXEventHandler< std::string > handler_2;
SAXEventHandler< std::string > handler_3;
SAXEventHandler< std::string > handler_4;
SAXEventHandler< std::string > handler_5;
SAXEventHandler< std::string > handler_6;
SAXEventHandler< std::string > handler_7;
SAXEventHandler< std::string > handler_8;
SAXEventHandler< std::string > handler_9;
SAXEventHandler< std::string > handler_10;
SAXEventHandler< std::string > handler_11;
SAXEventHandler< std::string > handler_12;
SAXEventHandler< std::string > handler_13;
SAXEventHandler< std::string > handler_14;
SAXEventHandler< std::string > handler_15;bool has_channel_1;
bool has_channel_2;
bool has_channel_3;
bool has_channel_4;
bool has_channel_5;
bool has_channel_6;
bool has_channel_7;
bool has_channel_8;
bool has_channel_9;
bool has_channel_10;
bool has_channel_11;
bool has_channel_12;
bool has_channel_13;
bool has_channel_14;
bool has_channel_15;
bool has_channel_16;

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
    return "channel_1";
case 1:
    return "channel_2";
case 2:
    return "channel_3";
case 3:
    return "channel_4";
case 4:
    return "channel_5";
case 5:
    return "channel_6";
case 6:
    return "channel_7";
case 7:
    return "channel_8";
case 8:
    return "channel_9";
case 9:
    return "channel_10";
case 10:
    return "channel_11";
case 11:
    return "channel_12";
case 12:
    return "channel_13";
case 13:
    return "channel_14";
case 14:
    return "channel_15";
case 15:
    return "channel_16";
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
        has_channel_1 = false;
has_channel_2 = false;
has_channel_3 = false;
has_channel_4 = false;
has_channel_5 = false;
has_channel_6 = false;
has_channel_7 = false;
has_channel_8 = false;
has_channel_9 = false;
has_channel_10 = false;
has_channel_11 = false;
has_channel_12 = false;
has_channel_13 = false;
has_channel_14 = false;
has_channel_15 = false;
has_channel_16 = false;
    }

public:
    explicit SAXEventHandler( ::sz::PCA9685::Inputs * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->channel_1)
, handler_1(&obj->channel_2)
, handler_2(&obj->channel_3)
, handler_3(&obj->channel_4)
, handler_4(&obj->channel_5)
, handler_5(&obj->channel_6)
, handler_6(&obj->channel_7)
, handler_7(&obj->channel_8)
, handler_8(&obj->channel_9)
, handler_9(&obj->channel_10)
, handler_10(&obj->channel_11)
, handler_11(&obj->channel_12)
, handler_12(&obj->channel_13)
, handler_13(&obj->channel_14)
, handler_14(&obj->channel_15)
, handler_15(&obj->channel_16)
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

case 8:
    return checked_event_forwarding(handler_8.Null());

case 9:
    return checked_event_forwarding(handler_9.Null());

case 10:
    return checked_event_forwarding(handler_10.Null());

case 11:
    return checked_event_forwarding(handler_11.Null());

case 12:
    return checked_event_forwarding(handler_12.Null());

case 13:
    return checked_event_forwarding(handler_13.Null());

case 14:
    return checked_event_forwarding(handler_14.Null());

case 15:
    return checked_event_forwarding(handler_15.Null());

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

case 8:
    return checked_event_forwarding(handler_8.Bool(b));

case 9:
    return checked_event_forwarding(handler_9.Bool(b));

case 10:
    return checked_event_forwarding(handler_10.Bool(b));

case 11:
    return checked_event_forwarding(handler_11.Bool(b));

case 12:
    return checked_event_forwarding(handler_12.Bool(b));

case 13:
    return checked_event_forwarding(handler_13.Bool(b));

case 14:
    return checked_event_forwarding(handler_14.Bool(b));

case 15:
    return checked_event_forwarding(handler_15.Bool(b));

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

case 8:
    return checked_event_forwarding(handler_8.Int(i));

case 9:
    return checked_event_forwarding(handler_9.Int(i));

case 10:
    return checked_event_forwarding(handler_10.Int(i));

case 11:
    return checked_event_forwarding(handler_11.Int(i));

case 12:
    return checked_event_forwarding(handler_12.Int(i));

case 13:
    return checked_event_forwarding(handler_13.Int(i));

case 14:
    return checked_event_forwarding(handler_14.Int(i));

case 15:
    return checked_event_forwarding(handler_15.Int(i));

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

case 8:
    return checked_event_forwarding(handler_8.Uint(i));

case 9:
    return checked_event_forwarding(handler_9.Uint(i));

case 10:
    return checked_event_forwarding(handler_10.Uint(i));

case 11:
    return checked_event_forwarding(handler_11.Uint(i));

case 12:
    return checked_event_forwarding(handler_12.Uint(i));

case 13:
    return checked_event_forwarding(handler_13.Uint(i));

case 14:
    return checked_event_forwarding(handler_14.Uint(i));

case 15:
    return checked_event_forwarding(handler_15.Uint(i));

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

case 8:
    return checked_event_forwarding(handler_8.Int64(i));

case 9:
    return checked_event_forwarding(handler_9.Int64(i));

case 10:
    return checked_event_forwarding(handler_10.Int64(i));

case 11:
    return checked_event_forwarding(handler_11.Int64(i));

case 12:
    return checked_event_forwarding(handler_12.Int64(i));

case 13:
    return checked_event_forwarding(handler_13.Int64(i));

case 14:
    return checked_event_forwarding(handler_14.Int64(i));

case 15:
    return checked_event_forwarding(handler_15.Int64(i));

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

case 8:
    return checked_event_forwarding(handler_8.Uint64(i));

case 9:
    return checked_event_forwarding(handler_9.Uint64(i));

case 10:
    return checked_event_forwarding(handler_10.Uint64(i));

case 11:
    return checked_event_forwarding(handler_11.Uint64(i));

case 12:
    return checked_event_forwarding(handler_12.Uint64(i));

case 13:
    return checked_event_forwarding(handler_13.Uint64(i));

case 14:
    return checked_event_forwarding(handler_14.Uint64(i));

case 15:
    return checked_event_forwarding(handler_15.Uint64(i));

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

case 8:
    return checked_event_forwarding(handler_8.Double(d));

case 9:
    return checked_event_forwarding(handler_9.Double(d));

case 10:
    return checked_event_forwarding(handler_10.Double(d));

case 11:
    return checked_event_forwarding(handler_11.Double(d));

case 12:
    return checked_event_forwarding(handler_12.Double(d));

case 13:
    return checked_event_forwarding(handler_13.Double(d));

case 14:
    return checked_event_forwarding(handler_14.Double(d));

case 15:
    return checked_event_forwarding(handler_15.Double(d));

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

case 8:
    return checked_event_forwarding(handler_8.String(str, length, copy));

case 9:
    return checked_event_forwarding(handler_9.String(str, length, copy));

case 10:
    return checked_event_forwarding(handler_10.String(str, length, copy));

case 11:
    return checked_event_forwarding(handler_11.String(str, length, copy));

case 12:
    return checked_event_forwarding(handler_12.String(str, length, copy));

case 13:
    return checked_event_forwarding(handler_13.String(str, length, copy));

case 14:
    return checked_event_forwarding(handler_14.String(str, length, copy));

case 15:
    return checked_event_forwarding(handler_15.String(str, length, copy));

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
            else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31", 9))
						 { state=0; has_channel_1 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x32", 9))
						 { state=1; has_channel_2 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x33", 9))
						 { state=2; has_channel_3 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x34", 9))
						 { state=3; has_channel_4 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x35", 9))
						 { state=4; has_channel_5 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x36", 9))
						 { state=5; has_channel_6 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x37", 9))
						 { state=6; has_channel_7 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x38", 9))
						 { state=7; has_channel_8 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x39", 9))
						 { state=8; has_channel_9 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x30", 10))
						 { state=9; has_channel_10 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x31", 10))
						 { state=10; has_channel_11 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x32", 10))
						 { state=11; has_channel_12 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x33", 10))
						 { state=12; has_channel_13 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x34", 10))
						 { state=13; has_channel_14 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x35", 10))
						 { state=14; has_channel_15 = true; }
else if (utility::string_equal(str, length, "\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x36", 10))
						 { state=15; has_channel_16 = true; }
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

case 8:
    return checked_event_forwarding(handler_8.Key(str, length, copy));

case 9:
    return checked_event_forwarding(handler_9.Key(str, length, copy));

case 10:
    return checked_event_forwarding(handler_10.Key(str, length, copy));

case 11:
    return checked_event_forwarding(handler_11.Key(str, length, copy));

case 12:
    return checked_event_forwarding(handler_12.Key(str, length, copy));

case 13:
    return checked_event_forwarding(handler_13.Key(str, length, copy));

case 14:
    return checked_event_forwarding(handler_14.Key(str, length, copy));

case 15:
    return checked_event_forwarding(handler_15.Key(str, length, copy));

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

case 8:
    return checked_event_forwarding(handler_8.StartArray());

case 9:
    return checked_event_forwarding(handler_9.StartArray());

case 10:
    return checked_event_forwarding(handler_10.StartArray());

case 11:
    return checked_event_forwarding(handler_11.StartArray());

case 12:
    return checked_event_forwarding(handler_12.StartArray());

case 13:
    return checked_event_forwarding(handler_13.StartArray());

case 14:
    return checked_event_forwarding(handler_14.StartArray());

case 15:
    return checked_event_forwarding(handler_15.StartArray());

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

case 8:
    return checked_event_forwarding(handler_8.EndArray(length));

case 9:
    return checked_event_forwarding(handler_9.EndArray(length));

case 10:
    return checked_event_forwarding(handler_10.EndArray(length));

case 11:
    return checked_event_forwarding(handler_11.EndArray(length));

case 12:
    return checked_event_forwarding(handler_12.EndArray(length));

case 13:
    return checked_event_forwarding(handler_13.EndArray(length));

case 14:
    return checked_event_forwarding(handler_14.EndArray(length));

case 15:
    return checked_event_forwarding(handler_15.EndArray(length));

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

case 8:
    return checked_event_forwarding(handler_8.StartObject());

case 9:
    return checked_event_forwarding(handler_9.StartObject());

case 10:
    return checked_event_forwarding(handler_10.StartObject());

case 11:
    return checked_event_forwarding(handler_11.StartObject());

case 12:
    return checked_event_forwarding(handler_12.StartObject());

case 13:
    return checked_event_forwarding(handler_13.StartObject());

case 14:
    return checked_event_forwarding(handler_14.StartObject());

case 15:
    return checked_event_forwarding(handler_15.StartObject());

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

case 8:
    return checked_event_forwarding(handler_8.EndObject(length));

case 9:
    return checked_event_forwarding(handler_9.EndObject(length));

case 10:
    return checked_event_forwarding(handler_10.EndObject(length));

case 11:
    return checked_event_forwarding(handler_11.EndObject(length));

case 12:
    return checked_event_forwarding(handler_12.EndObject(length));

case 13:
    return checked_event_forwarding(handler_13.EndObject(length));

case 14:
    return checked_event_forwarding(handler_14.EndObject(length));

case 15:
    return checked_event_forwarding(handler_15.EndObject(length));

            default:
                break;
            }
        } else {
            if (!has_channel_1) set_missing_required("channel_1");
if (!has_channel_2) set_missing_required("channel_2");
if (!has_channel_3) set_missing_required("channel_3");
if (!has_channel_4) set_missing_required("channel_4");
if (!has_channel_5) set_missing_required("channel_5");
if (!has_channel_6) set_missing_required("channel_6");
if (!has_channel_7) set_missing_required("channel_7");
if (!has_channel_8) set_missing_required("channel_8");
if (!has_channel_9) set_missing_required("channel_9");
if (!has_channel_10) set_missing_required("channel_10");
if (!has_channel_11) set_missing_required("channel_11");
if (!has_channel_12) set_missing_required("channel_12");
if (!has_channel_13) set_missing_required("channel_13");
if (!has_channel_14) set_missing_required("channel_14");
if (!has_channel_15) set_missing_required("channel_15");
if (!has_channel_16) set_missing_required("channel_16");
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
case 8:
     handler_8.ReapError(errs); break;
case 9:
     handler_9.ReapError(errs); break;
case 10:
     handler_10.ReapError(errs); break;
case 11:
     handler_11.ReapError(errs); break;
case 12:
     handler_12.ReapError(errs); break;
case 13:
     handler_13.ReapError(errs); break;
case 14:
     handler_14.ReapError(errs); break;
case 15:
     handler_15.ReapError(errs); break;

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();
handler_1.PrepareForReuse();
handler_2.PrepareForReuse();
handler_3.PrepareForReuse();
handler_4.PrepareForReuse();
handler_5.PrepareForReuse();
handler_6.PrepareForReuse();
handler_7.PrepareForReuse();
handler_8.PrepareForReuse();
handler_9.PrepareForReuse();
handler_10.PrepareForReuse();
handler_11.PrepareForReuse();
handler_12.PrepareForReuse();
handler_13.PrepareForReuse();
handler_14.PrepareForReuse();
handler_15.PrepareForReuse();

    }
};

template < class Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0 >
struct Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, ::sz::PCA9685::Inputs > {

    void operator()( Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0& w, const ::sz::PCA9685::Inputs& value) const
    {
        w.StartObject();

        w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_1);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x32", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_2);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x33", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_3);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x34", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_4);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x35", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_5);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x36", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_6);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x37", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_7);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x38", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_8);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x39", 9, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_9);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x30", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_10);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x31", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_11);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x32", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_12);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x33", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_13);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x34", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_14);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x35", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_15);
w.Key("\x43\x68\x61\x6e\x6e\x65\x6c\x20\x31\x36", 10, false); Serializer< Writer9fa7f8f62678a3b9203cc156f52c1cc34b3ad0a3fe81fc2b9ea640ebbb25ebb0, std::string >()(w, value.channel_16);

        w.EndObject(16);
    }

};
}


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

namespace sz { namespace PCA9685 { struct Config {
 sz::PCA9685::Inputs inputs;

explicit Config():inputs() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PCA9685::Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< sz::PCA9685::Inputs > handler_0;bool has_inputs;

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
    return "inputs";
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
        has_inputs = false;
    }

public:
    explicit SAXEventHandler( ::sz::PCA9685::Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->inputs)
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
            else if (utility::string_equal(str, length, "\x49\x6e\x70\x75\x74\x73", 6))
						 { state=0; has_inputs = true; }
            else {
                state = -1;
                return true;
            }

        } else {
            switch (state) {

            case 0:
    return checked_event_forwarding(handler_0.Key(str, length, copy));

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

            default:
                break;
            }
        } else {
            if (!has_inputs) set_missing_required("inputs");
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

        default:
            break;
        }

        return true;
    }

    void PrepareForReuse()
    {
        depth = 0;
        state = -1;
        the_error.reset();
        reset_flags();
        handler_0.PrepareForReuse();

    }
};

template < class Writerc1e28de29ada41a692a3af3a61964af4e7edfba6565ab979ee4fbd1a0763fce9 >
struct Serializer< Writerc1e28de29ada41a692a3af3a61964af4e7edfba6565ab979ee4fbd1a0763fce9, ::sz::PCA9685::Config > {

    void operator()( Writerc1e28de29ada41a692a3af3a61964af4e7edfba6565ab979ee4fbd1a0763fce9& w, const ::sz::PCA9685::Config& value) const
    {
        w.StartObject();

        w.Key("\x49\x6e\x70\x75\x74\x73", 6, false); Serializer< Writerc1e28de29ada41a692a3af3a61964af4e7edfba6565ab979ee4fbd1a0763fce9, sz::PCA9685::Inputs >()(w, value.inputs);

        w.EndObject(1);
    }

};
}



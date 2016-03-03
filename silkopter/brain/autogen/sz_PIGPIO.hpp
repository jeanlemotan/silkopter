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

namespace sz { namespace PIGPIO { struct PWM_Channel {
 bool enabled;
bool servo;
uint32_t rate;

explicit PWM_Channel():enabled(false), servo(false), rate(400) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PIGPIO::PWM_Channel > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< bool > handler_0;
SAXEventHandler< bool > handler_1;
SAXEventHandler< uint32_t > handler_2;bool has_rate;

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
    return "enabled";
case 1:
    return "servo";
case 2:
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
        

has_rate = false;
    }

public:
    explicit SAXEventHandler( ::sz::PIGPIO::PWM_Channel * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->enabled)
, handler_1(&obj->servo)
, handler_2(&obj->rate)
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
            else if (utility::string_equal(str, length, "\x45\x6e\x61\x62\x6c\x65\x64", 7))
						 { state=0;  }
else if (utility::string_equal(str, length, "\x53\x65\x72\x76\x6f", 5))
						 { state=1;  }
else if (utility::string_equal(str, length, "\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 9))
						 { state=2; has_rate = true; }
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

            default:
                break;
            }
        } else {
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

    }
};

template < class Writer4712655c439cd1bb07e4910104a72f8ef3681447509a8ddffc387168f79ad59c >
struct Serializer< Writer4712655c439cd1bb07e4910104a72f8ef3681447509a8ddffc387168f79ad59c, ::sz::PIGPIO::PWM_Channel > {

    void operator()( Writer4712655c439cd1bb07e4910104a72f8ef3681447509a8ddffc387168f79ad59c& w, const ::sz::PIGPIO::PWM_Channel& value) const
    {
        w.StartObject();

        w.Key("\x45\x6e\x61\x62\x6c\x65\x64", 7, false); Serializer< Writer4712655c439cd1bb07e4910104a72f8ef3681447509a8ddffc387168f79ad59c, bool >()(w, value.enabled);
w.Key("\x53\x65\x72\x76\x6f", 5, false); Serializer< Writer4712655c439cd1bb07e4910104a72f8ef3681447509a8ddffc387168f79ad59c, bool >()(w, value.servo);
w.Key("\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 9, false); Serializer< Writer4712655c439cd1bb07e4910104a72f8ef3681447509a8ddffc387168f79ad59c, uint32_t >()(w, value.rate);

        w.EndObject(3);
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

namespace sz { namespace PIGPIO { struct PWM_Config {
 float min_servo;
float max_servo;
float min_pwm;
float max_pwm;

explicit PWM_Config():min_servo(1), max_servo(2), min_pwm(0), max_pwm(1) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PIGPIO::PWM_Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< float > handler_0;
SAXEventHandler< float > handler_1;
SAXEventHandler< float > handler_2;
SAXEventHandler< float > handler_3;

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
    return "min_servo";
case 1:
    return "max_servo";
case 2:
    return "min_pwm";
case 3:
    return "max_pwm";
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
        



    }

public:
    explicit SAXEventHandler( ::sz::PIGPIO::PWM_Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->min_servo)
, handler_1(&obj->max_servo)
, handler_2(&obj->min_pwm)
, handler_3(&obj->max_pwm)
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
            else if (utility::string_equal(str, length, "\x4d\x69\x6e\x20\x53\x65\x72\x76\x6f\x20\x28\x6d\x73\x29", 14))
						 { state=0;  }
else if (utility::string_equal(str, length, "\x4d\x61\x78\x20\x53\x65\x72\x76\x6f\x20\x28\x6d\x73\x29", 14))
						 { state=1;  }
else if (utility::string_equal(str, length, "\x4d\x69\x6e\x20\x50\x57\x4d", 7))
						 { state=2;  }
else if (utility::string_equal(str, length, "\x4d\x61\x78\x20\x50\x57\x4d", 7))
						 { state=3;  }
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

template < class Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37 >
struct Serializer< Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37, ::sz::PIGPIO::PWM_Config > {

    void operator()( Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37& w, const ::sz::PIGPIO::PWM_Config& value) const
    {
        w.StartObject();

        w.Key("\x4d\x69\x6e\x20\x53\x65\x72\x76\x6f\x20\x28\x6d\x73\x29", 14, false); Serializer< Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37, float >()(w, value.min_servo);
w.Key("\x4d\x61\x78\x20\x53\x65\x72\x76\x6f\x20\x28\x6d\x73\x29", 14, false); Serializer< Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37, float >()(w, value.max_servo);
w.Key("\x4d\x69\x6e\x20\x50\x57\x4d", 7, false); Serializer< Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37, float >()(w, value.min_pwm);
w.Key("\x4d\x61\x78\x20\x50\x57\x4d", 7, false); Serializer< Writerdc1310bacba9232b01f7074c87d813dfa462410e8201cea01effde189a943e37, float >()(w, value.max_pwm);

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

namespace sz { namespace PIGPIO { struct Init_Params {
 sz::PIGPIO::PWM_Channel gpio_2;
sz::PIGPIO::PWM_Channel gpio_3;
sz::PIGPIO::PWM_Channel gpio_4;
sz::PIGPIO::PWM_Channel gpio_5;
sz::PIGPIO::PWM_Channel gpio_6;
sz::PIGPIO::PWM_Channel gpio_7;
sz::PIGPIO::PWM_Channel gpio_8;
sz::PIGPIO::PWM_Channel gpio_9;
sz::PIGPIO::PWM_Channel gpio_10;
sz::PIGPIO::PWM_Channel gpio_11;
sz::PIGPIO::PWM_Channel gpio_12;
sz::PIGPIO::PWM_Channel gpio_13;
sz::PIGPIO::PWM_Channel gpio_14;
sz::PIGPIO::PWM_Channel gpio_15;
sz::PIGPIO::PWM_Channel gpio_16;
sz::PIGPIO::PWM_Channel gpio_17;
sz::PIGPIO::PWM_Channel gpio_18;
sz::PIGPIO::PWM_Channel gpio_19;
sz::PIGPIO::PWM_Channel gpio_20;
sz::PIGPIO::PWM_Channel gpio_21;
sz::PIGPIO::PWM_Channel gpio_22;
sz::PIGPIO::PWM_Channel gpio_23;
sz::PIGPIO::PWM_Channel gpio_24;
sz::PIGPIO::PWM_Channel gpio_25;
sz::PIGPIO::PWM_Channel gpio_26;
sz::PIGPIO::PWM_Channel gpio_27;

explicit Init_Params():gpio_2(), gpio_3(), gpio_4(), gpio_5(), gpio_6(), gpio_7(), gpio_8(), gpio_9(), gpio_10(), gpio_11(), gpio_12(), gpio_13(), gpio_14(), gpio_15(), gpio_16(), gpio_17(), gpio_18(), gpio_19(), gpio_20(), gpio_21(), gpio_22(), gpio_23(), gpio_24(), gpio_25(), gpio_26(), gpio_27() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PIGPIO::Init_Params > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_0;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_1;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_2;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_3;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_4;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_5;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_6;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_7;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_8;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_9;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_10;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_11;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_12;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_13;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_14;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_15;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_16;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_17;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_18;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_19;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_20;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_21;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_22;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_23;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_24;
SAXEventHandler< sz::PIGPIO::PWM_Channel > handler_25;

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
    return "gpio_2";
case 1:
    return "gpio_3";
case 2:
    return "gpio_4";
case 3:
    return "gpio_5";
case 4:
    return "gpio_6";
case 5:
    return "gpio_7";
case 6:
    return "gpio_8";
case 7:
    return "gpio_9";
case 8:
    return "gpio_10";
case 9:
    return "gpio_11";
case 10:
    return "gpio_12";
case 11:
    return "gpio_13";
case 12:
    return "gpio_14";
case 13:
    return "gpio_15";
case 14:
    return "gpio_16";
case 15:
    return "gpio_17";
case 16:
    return "gpio_18";
case 17:
    return "gpio_19";
case 18:
    return "gpio_20";
case 19:
    return "gpio_21";
case 20:
    return "gpio_22";
case 21:
    return "gpio_23";
case 22:
    return "gpio_24";
case 23:
    return "gpio_25";
case 24:
    return "gpio_26";
case 25:
    return "gpio_27";
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
        

























    }

public:
    explicit SAXEventHandler( ::sz::PIGPIO::Init_Params * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->gpio_2)
, handler_1(&obj->gpio_3)
, handler_2(&obj->gpio_4)
, handler_3(&obj->gpio_5)
, handler_4(&obj->gpio_6)
, handler_5(&obj->gpio_7)
, handler_6(&obj->gpio_8)
, handler_7(&obj->gpio_9)
, handler_8(&obj->gpio_10)
, handler_9(&obj->gpio_11)
, handler_10(&obj->gpio_12)
, handler_11(&obj->gpio_13)
, handler_12(&obj->gpio_14)
, handler_13(&obj->gpio_15)
, handler_14(&obj->gpio_16)
, handler_15(&obj->gpio_17)
, handler_16(&obj->gpio_18)
, handler_17(&obj->gpio_19)
, handler_18(&obj->gpio_20)
, handler_19(&obj->gpio_21)
, handler_20(&obj->gpio_22)
, handler_21(&obj->gpio_23)
, handler_22(&obj->gpio_24)
, handler_23(&obj->gpio_25)
, handler_24(&obj->gpio_26)
, handler_25(&obj->gpio_27)
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

case 17:
    return checked_event_forwarding(handler_17.Null());

case 18:
    return checked_event_forwarding(handler_18.Null());

case 19:
    return checked_event_forwarding(handler_19.Null());

case 20:
    return checked_event_forwarding(handler_20.Null());

case 21:
    return checked_event_forwarding(handler_21.Null());

case 22:
    return checked_event_forwarding(handler_22.Null());

case 23:
    return checked_event_forwarding(handler_23.Null());

case 24:
    return checked_event_forwarding(handler_24.Null());

case 25:
    return checked_event_forwarding(handler_25.Null());

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

case 17:
    return checked_event_forwarding(handler_17.Bool(b));

case 18:
    return checked_event_forwarding(handler_18.Bool(b));

case 19:
    return checked_event_forwarding(handler_19.Bool(b));

case 20:
    return checked_event_forwarding(handler_20.Bool(b));

case 21:
    return checked_event_forwarding(handler_21.Bool(b));

case 22:
    return checked_event_forwarding(handler_22.Bool(b));

case 23:
    return checked_event_forwarding(handler_23.Bool(b));

case 24:
    return checked_event_forwarding(handler_24.Bool(b));

case 25:
    return checked_event_forwarding(handler_25.Bool(b));

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

case 17:
    return checked_event_forwarding(handler_17.Int(i));

case 18:
    return checked_event_forwarding(handler_18.Int(i));

case 19:
    return checked_event_forwarding(handler_19.Int(i));

case 20:
    return checked_event_forwarding(handler_20.Int(i));

case 21:
    return checked_event_forwarding(handler_21.Int(i));

case 22:
    return checked_event_forwarding(handler_22.Int(i));

case 23:
    return checked_event_forwarding(handler_23.Int(i));

case 24:
    return checked_event_forwarding(handler_24.Int(i));

case 25:
    return checked_event_forwarding(handler_25.Int(i));

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

case 17:
    return checked_event_forwarding(handler_17.Uint(i));

case 18:
    return checked_event_forwarding(handler_18.Uint(i));

case 19:
    return checked_event_forwarding(handler_19.Uint(i));

case 20:
    return checked_event_forwarding(handler_20.Uint(i));

case 21:
    return checked_event_forwarding(handler_21.Uint(i));

case 22:
    return checked_event_forwarding(handler_22.Uint(i));

case 23:
    return checked_event_forwarding(handler_23.Uint(i));

case 24:
    return checked_event_forwarding(handler_24.Uint(i));

case 25:
    return checked_event_forwarding(handler_25.Uint(i));

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

case 17:
    return checked_event_forwarding(handler_17.Int64(i));

case 18:
    return checked_event_forwarding(handler_18.Int64(i));

case 19:
    return checked_event_forwarding(handler_19.Int64(i));

case 20:
    return checked_event_forwarding(handler_20.Int64(i));

case 21:
    return checked_event_forwarding(handler_21.Int64(i));

case 22:
    return checked_event_forwarding(handler_22.Int64(i));

case 23:
    return checked_event_forwarding(handler_23.Int64(i));

case 24:
    return checked_event_forwarding(handler_24.Int64(i));

case 25:
    return checked_event_forwarding(handler_25.Int64(i));

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

case 17:
    return checked_event_forwarding(handler_17.Uint64(i));

case 18:
    return checked_event_forwarding(handler_18.Uint64(i));

case 19:
    return checked_event_forwarding(handler_19.Uint64(i));

case 20:
    return checked_event_forwarding(handler_20.Uint64(i));

case 21:
    return checked_event_forwarding(handler_21.Uint64(i));

case 22:
    return checked_event_forwarding(handler_22.Uint64(i));

case 23:
    return checked_event_forwarding(handler_23.Uint64(i));

case 24:
    return checked_event_forwarding(handler_24.Uint64(i));

case 25:
    return checked_event_forwarding(handler_25.Uint64(i));

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

case 17:
    return checked_event_forwarding(handler_17.Double(d));

case 18:
    return checked_event_forwarding(handler_18.Double(d));

case 19:
    return checked_event_forwarding(handler_19.Double(d));

case 20:
    return checked_event_forwarding(handler_20.Double(d));

case 21:
    return checked_event_forwarding(handler_21.Double(d));

case 22:
    return checked_event_forwarding(handler_22.Double(d));

case 23:
    return checked_event_forwarding(handler_23.Double(d));

case 24:
    return checked_event_forwarding(handler_24.Double(d));

case 25:
    return checked_event_forwarding(handler_25.Double(d));

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

case 17:
    return checked_event_forwarding(handler_17.String(str, length, copy));

case 18:
    return checked_event_forwarding(handler_18.String(str, length, copy));

case 19:
    return checked_event_forwarding(handler_19.String(str, length, copy));

case 20:
    return checked_event_forwarding(handler_20.String(str, length, copy));

case 21:
    return checked_event_forwarding(handler_21.String(str, length, copy));

case 22:
    return checked_event_forwarding(handler_22.String(str, length, copy));

case 23:
    return checked_event_forwarding(handler_23.String(str, length, copy));

case 24:
    return checked_event_forwarding(handler_24.String(str, length, copy));

case 25:
    return checked_event_forwarding(handler_25.String(str, length, copy));

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
            else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32", 6))
						 { state=0;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x33", 6))
						 { state=1;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x34", 6))
						 { state=2;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x35", 6))
						 { state=3;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x36", 6))
						 { state=4;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x37", 6))
						 { state=5;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x38", 6))
						 { state=6;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x39", 6))
						 { state=7;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x30", 7))
						 { state=8;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x31", 7))
						 { state=9;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x32", 7))
						 { state=10;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x33", 7))
						 { state=11;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x34", 7))
						 { state=12;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x35", 7))
						 { state=13;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x36", 7))
						 { state=14;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x37", 7))
						 { state=15;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x38", 7))
						 { state=16;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x39", 7))
						 { state=17;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x30", 7))
						 { state=18;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x31", 7))
						 { state=19;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x32", 7))
						 { state=20;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x33", 7))
						 { state=21;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x34", 7))
						 { state=22;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x35", 7))
						 { state=23;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x36", 7))
						 { state=24;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x37", 7))
						 { state=25;  }
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

case 17:
    return checked_event_forwarding(handler_17.Key(str, length, copy));

case 18:
    return checked_event_forwarding(handler_18.Key(str, length, copy));

case 19:
    return checked_event_forwarding(handler_19.Key(str, length, copy));

case 20:
    return checked_event_forwarding(handler_20.Key(str, length, copy));

case 21:
    return checked_event_forwarding(handler_21.Key(str, length, copy));

case 22:
    return checked_event_forwarding(handler_22.Key(str, length, copy));

case 23:
    return checked_event_forwarding(handler_23.Key(str, length, copy));

case 24:
    return checked_event_forwarding(handler_24.Key(str, length, copy));

case 25:
    return checked_event_forwarding(handler_25.Key(str, length, copy));

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

case 17:
    return checked_event_forwarding(handler_17.StartArray());

case 18:
    return checked_event_forwarding(handler_18.StartArray());

case 19:
    return checked_event_forwarding(handler_19.StartArray());

case 20:
    return checked_event_forwarding(handler_20.StartArray());

case 21:
    return checked_event_forwarding(handler_21.StartArray());

case 22:
    return checked_event_forwarding(handler_22.StartArray());

case 23:
    return checked_event_forwarding(handler_23.StartArray());

case 24:
    return checked_event_forwarding(handler_24.StartArray());

case 25:
    return checked_event_forwarding(handler_25.StartArray());

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

case 17:
    return checked_event_forwarding(handler_17.EndArray(length));

case 18:
    return checked_event_forwarding(handler_18.EndArray(length));

case 19:
    return checked_event_forwarding(handler_19.EndArray(length));

case 20:
    return checked_event_forwarding(handler_20.EndArray(length));

case 21:
    return checked_event_forwarding(handler_21.EndArray(length));

case 22:
    return checked_event_forwarding(handler_22.EndArray(length));

case 23:
    return checked_event_forwarding(handler_23.EndArray(length));

case 24:
    return checked_event_forwarding(handler_24.EndArray(length));

case 25:
    return checked_event_forwarding(handler_25.EndArray(length));

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

case 17:
    return checked_event_forwarding(handler_17.StartObject());

case 18:
    return checked_event_forwarding(handler_18.StartObject());

case 19:
    return checked_event_forwarding(handler_19.StartObject());

case 20:
    return checked_event_forwarding(handler_20.StartObject());

case 21:
    return checked_event_forwarding(handler_21.StartObject());

case 22:
    return checked_event_forwarding(handler_22.StartObject());

case 23:
    return checked_event_forwarding(handler_23.StartObject());

case 24:
    return checked_event_forwarding(handler_24.StartObject());

case 25:
    return checked_event_forwarding(handler_25.StartObject());

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

case 17:
    return checked_event_forwarding(handler_17.EndObject(length));

case 18:
    return checked_event_forwarding(handler_18.EndObject(length));

case 19:
    return checked_event_forwarding(handler_19.EndObject(length));

case 20:
    return checked_event_forwarding(handler_20.EndObject(length));

case 21:
    return checked_event_forwarding(handler_21.EndObject(length));

case 22:
    return checked_event_forwarding(handler_22.EndObject(length));

case 23:
    return checked_event_forwarding(handler_23.EndObject(length));

case 24:
    return checked_event_forwarding(handler_24.EndObject(length));

case 25:
    return checked_event_forwarding(handler_25.EndObject(length));

            default:
                break;
            }
        } else {
            
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
case 17:
     handler_17.ReapError(errs); break;
case 18:
     handler_18.ReapError(errs); break;
case 19:
     handler_19.ReapError(errs); break;
case 20:
     handler_20.ReapError(errs); break;
case 21:
     handler_21.ReapError(errs); break;
case 22:
     handler_22.ReapError(errs); break;
case 23:
     handler_23.ReapError(errs); break;
case 24:
     handler_24.ReapError(errs); break;
case 25:
     handler_25.ReapError(errs); break;

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
handler_17.PrepareForReuse();
handler_18.PrepareForReuse();
handler_19.PrepareForReuse();
handler_20.PrepareForReuse();
handler_21.PrepareForReuse();
handler_22.PrepareForReuse();
handler_23.PrepareForReuse();
handler_24.PrepareForReuse();
handler_25.PrepareForReuse();

    }
};

template < class Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e >
struct Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, ::sz::PIGPIO::Init_Params > {

    void operator()( Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e& w, const ::sz::PIGPIO::Init_Params& value) const
    {
        w.StartObject();

        w.Key("\x47\x50\x49\x4f\x20\x32", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_2);
w.Key("\x47\x50\x49\x4f\x20\x33", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_3);
w.Key("\x47\x50\x49\x4f\x20\x34", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_4);
w.Key("\x47\x50\x49\x4f\x20\x35", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_5);
w.Key("\x47\x50\x49\x4f\x20\x36", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_6);
w.Key("\x47\x50\x49\x4f\x20\x37", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_7);
w.Key("\x47\x50\x49\x4f\x20\x38", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_8);
w.Key("\x47\x50\x49\x4f\x20\x39", 6, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_9);
w.Key("\x47\x50\x49\x4f\x20\x31\x30", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_10);
w.Key("\x47\x50\x49\x4f\x20\x31\x31", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_11);
w.Key("\x47\x50\x49\x4f\x20\x31\x32", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_12);
w.Key("\x47\x50\x49\x4f\x20\x31\x33", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_13);
w.Key("\x47\x50\x49\x4f\x20\x31\x34", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_14);
w.Key("\x47\x50\x49\x4f\x20\x31\x35", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_15);
w.Key("\x47\x50\x49\x4f\x20\x31\x36", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_16);
w.Key("\x47\x50\x49\x4f\x20\x31\x37", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_17);
w.Key("\x47\x50\x49\x4f\x20\x31\x38", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_18);
w.Key("\x47\x50\x49\x4f\x20\x31\x39", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_19);
w.Key("\x47\x50\x49\x4f\x20\x32\x30", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_20);
w.Key("\x47\x50\x49\x4f\x20\x32\x31", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_21);
w.Key("\x47\x50\x49\x4f\x20\x32\x32", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_22);
w.Key("\x47\x50\x49\x4f\x20\x32\x33", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_23);
w.Key("\x47\x50\x49\x4f\x20\x32\x34", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_24);
w.Key("\x47\x50\x49\x4f\x20\x32\x35", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_25);
w.Key("\x47\x50\x49\x4f\x20\x32\x36", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_26);
w.Key("\x47\x50\x49\x4f\x20\x32\x37", 7, false); Serializer< Writer2f3515a066b342d6136021fc7d4acb98a48223fbc5d7634711320911c027882e, sz::PIGPIO::PWM_Channel >()(w, value.gpio_27);

        w.EndObject(26);
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

namespace sz { namespace PIGPIO { struct Config {
 sz::PIGPIO::PWM_Config gpio_2;
sz::PIGPIO::PWM_Config gpio_3;
sz::PIGPIO::PWM_Config gpio_4;
sz::PIGPIO::PWM_Config gpio_5;
sz::PIGPIO::PWM_Config gpio_6;
sz::PIGPIO::PWM_Config gpio_7;
sz::PIGPIO::PWM_Config gpio_8;
sz::PIGPIO::PWM_Config gpio_9;
sz::PIGPIO::PWM_Config gpio_10;
sz::PIGPIO::PWM_Config gpio_11;
sz::PIGPIO::PWM_Config gpio_12;
sz::PIGPIO::PWM_Config gpio_13;
sz::PIGPIO::PWM_Config gpio_14;
sz::PIGPIO::PWM_Config gpio_15;
sz::PIGPIO::PWM_Config gpio_16;
sz::PIGPIO::PWM_Config gpio_17;
sz::PIGPIO::PWM_Config gpio_18;
sz::PIGPIO::PWM_Config gpio_19;
sz::PIGPIO::PWM_Config gpio_20;
sz::PIGPIO::PWM_Config gpio_21;
sz::PIGPIO::PWM_Config gpio_22;
sz::PIGPIO::PWM_Config gpio_23;
sz::PIGPIO::PWM_Config gpio_24;
sz::PIGPIO::PWM_Config gpio_25;
sz::PIGPIO::PWM_Config gpio_26;
sz::PIGPIO::PWM_Config gpio_27;

explicit Config():gpio_2(), gpio_3(), gpio_4(), gpio_5(), gpio_6(), gpio_7(), gpio_8(), gpio_9(), gpio_10(), gpio_11(), gpio_12(), gpio_13(), gpio_14(), gpio_15(), gpio_16(), gpio_17(), gpio_18(), gpio_19(), gpio_20(), gpio_21(), gpio_22(), gpio_23(), gpio_24(), gpio_25(), gpio_26(), gpio_27() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::PIGPIO::Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< sz::PIGPIO::PWM_Config > handler_0;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_1;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_2;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_3;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_4;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_5;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_6;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_7;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_8;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_9;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_10;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_11;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_12;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_13;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_14;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_15;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_16;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_17;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_18;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_19;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_20;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_21;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_22;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_23;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_24;
SAXEventHandler< sz::PIGPIO::PWM_Config > handler_25;

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
    return "gpio_2";
case 1:
    return "gpio_3";
case 2:
    return "gpio_4";
case 3:
    return "gpio_5";
case 4:
    return "gpio_6";
case 5:
    return "gpio_7";
case 6:
    return "gpio_8";
case 7:
    return "gpio_9";
case 8:
    return "gpio_10";
case 9:
    return "gpio_11";
case 10:
    return "gpio_12";
case 11:
    return "gpio_13";
case 12:
    return "gpio_14";
case 13:
    return "gpio_15";
case 14:
    return "gpio_16";
case 15:
    return "gpio_17";
case 16:
    return "gpio_18";
case 17:
    return "gpio_19";
case 18:
    return "gpio_20";
case 19:
    return "gpio_21";
case 20:
    return "gpio_22";
case 21:
    return "gpio_23";
case 22:
    return "gpio_24";
case 23:
    return "gpio_25";
case 24:
    return "gpio_26";
case 25:
    return "gpio_27";
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
        

























    }

public:
    explicit SAXEventHandler( ::sz::PIGPIO::Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->gpio_2)
, handler_1(&obj->gpio_3)
, handler_2(&obj->gpio_4)
, handler_3(&obj->gpio_5)
, handler_4(&obj->gpio_6)
, handler_5(&obj->gpio_7)
, handler_6(&obj->gpio_8)
, handler_7(&obj->gpio_9)
, handler_8(&obj->gpio_10)
, handler_9(&obj->gpio_11)
, handler_10(&obj->gpio_12)
, handler_11(&obj->gpio_13)
, handler_12(&obj->gpio_14)
, handler_13(&obj->gpio_15)
, handler_14(&obj->gpio_16)
, handler_15(&obj->gpio_17)
, handler_16(&obj->gpio_18)
, handler_17(&obj->gpio_19)
, handler_18(&obj->gpio_20)
, handler_19(&obj->gpio_21)
, handler_20(&obj->gpio_22)
, handler_21(&obj->gpio_23)
, handler_22(&obj->gpio_24)
, handler_23(&obj->gpio_25)
, handler_24(&obj->gpio_26)
, handler_25(&obj->gpio_27)
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

case 17:
    return checked_event_forwarding(handler_17.Null());

case 18:
    return checked_event_forwarding(handler_18.Null());

case 19:
    return checked_event_forwarding(handler_19.Null());

case 20:
    return checked_event_forwarding(handler_20.Null());

case 21:
    return checked_event_forwarding(handler_21.Null());

case 22:
    return checked_event_forwarding(handler_22.Null());

case 23:
    return checked_event_forwarding(handler_23.Null());

case 24:
    return checked_event_forwarding(handler_24.Null());

case 25:
    return checked_event_forwarding(handler_25.Null());

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

case 17:
    return checked_event_forwarding(handler_17.Bool(b));

case 18:
    return checked_event_forwarding(handler_18.Bool(b));

case 19:
    return checked_event_forwarding(handler_19.Bool(b));

case 20:
    return checked_event_forwarding(handler_20.Bool(b));

case 21:
    return checked_event_forwarding(handler_21.Bool(b));

case 22:
    return checked_event_forwarding(handler_22.Bool(b));

case 23:
    return checked_event_forwarding(handler_23.Bool(b));

case 24:
    return checked_event_forwarding(handler_24.Bool(b));

case 25:
    return checked_event_forwarding(handler_25.Bool(b));

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

case 17:
    return checked_event_forwarding(handler_17.Int(i));

case 18:
    return checked_event_forwarding(handler_18.Int(i));

case 19:
    return checked_event_forwarding(handler_19.Int(i));

case 20:
    return checked_event_forwarding(handler_20.Int(i));

case 21:
    return checked_event_forwarding(handler_21.Int(i));

case 22:
    return checked_event_forwarding(handler_22.Int(i));

case 23:
    return checked_event_forwarding(handler_23.Int(i));

case 24:
    return checked_event_forwarding(handler_24.Int(i));

case 25:
    return checked_event_forwarding(handler_25.Int(i));

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

case 17:
    return checked_event_forwarding(handler_17.Uint(i));

case 18:
    return checked_event_forwarding(handler_18.Uint(i));

case 19:
    return checked_event_forwarding(handler_19.Uint(i));

case 20:
    return checked_event_forwarding(handler_20.Uint(i));

case 21:
    return checked_event_forwarding(handler_21.Uint(i));

case 22:
    return checked_event_forwarding(handler_22.Uint(i));

case 23:
    return checked_event_forwarding(handler_23.Uint(i));

case 24:
    return checked_event_forwarding(handler_24.Uint(i));

case 25:
    return checked_event_forwarding(handler_25.Uint(i));

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

case 17:
    return checked_event_forwarding(handler_17.Int64(i));

case 18:
    return checked_event_forwarding(handler_18.Int64(i));

case 19:
    return checked_event_forwarding(handler_19.Int64(i));

case 20:
    return checked_event_forwarding(handler_20.Int64(i));

case 21:
    return checked_event_forwarding(handler_21.Int64(i));

case 22:
    return checked_event_forwarding(handler_22.Int64(i));

case 23:
    return checked_event_forwarding(handler_23.Int64(i));

case 24:
    return checked_event_forwarding(handler_24.Int64(i));

case 25:
    return checked_event_forwarding(handler_25.Int64(i));

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

case 17:
    return checked_event_forwarding(handler_17.Uint64(i));

case 18:
    return checked_event_forwarding(handler_18.Uint64(i));

case 19:
    return checked_event_forwarding(handler_19.Uint64(i));

case 20:
    return checked_event_forwarding(handler_20.Uint64(i));

case 21:
    return checked_event_forwarding(handler_21.Uint64(i));

case 22:
    return checked_event_forwarding(handler_22.Uint64(i));

case 23:
    return checked_event_forwarding(handler_23.Uint64(i));

case 24:
    return checked_event_forwarding(handler_24.Uint64(i));

case 25:
    return checked_event_forwarding(handler_25.Uint64(i));

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

case 17:
    return checked_event_forwarding(handler_17.Double(d));

case 18:
    return checked_event_forwarding(handler_18.Double(d));

case 19:
    return checked_event_forwarding(handler_19.Double(d));

case 20:
    return checked_event_forwarding(handler_20.Double(d));

case 21:
    return checked_event_forwarding(handler_21.Double(d));

case 22:
    return checked_event_forwarding(handler_22.Double(d));

case 23:
    return checked_event_forwarding(handler_23.Double(d));

case 24:
    return checked_event_forwarding(handler_24.Double(d));

case 25:
    return checked_event_forwarding(handler_25.Double(d));

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

case 17:
    return checked_event_forwarding(handler_17.String(str, length, copy));

case 18:
    return checked_event_forwarding(handler_18.String(str, length, copy));

case 19:
    return checked_event_forwarding(handler_19.String(str, length, copy));

case 20:
    return checked_event_forwarding(handler_20.String(str, length, copy));

case 21:
    return checked_event_forwarding(handler_21.String(str, length, copy));

case 22:
    return checked_event_forwarding(handler_22.String(str, length, copy));

case 23:
    return checked_event_forwarding(handler_23.String(str, length, copy));

case 24:
    return checked_event_forwarding(handler_24.String(str, length, copy));

case 25:
    return checked_event_forwarding(handler_25.String(str, length, copy));

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
            else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32", 6))
						 { state=0;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x33", 6))
						 { state=1;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x34", 6))
						 { state=2;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x35", 6))
						 { state=3;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x36", 6))
						 { state=4;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x37", 6))
						 { state=5;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x38", 6))
						 { state=6;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x39", 6))
						 { state=7;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x30", 7))
						 { state=8;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x31", 7))
						 { state=9;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x32", 7))
						 { state=10;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x33", 7))
						 { state=11;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x34", 7))
						 { state=12;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x35", 7))
						 { state=13;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x36", 7))
						 { state=14;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x37", 7))
						 { state=15;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x38", 7))
						 { state=16;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x31\x39", 7))
						 { state=17;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x30", 7))
						 { state=18;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x31", 7))
						 { state=19;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x32", 7))
						 { state=20;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x33", 7))
						 { state=21;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x34", 7))
						 { state=22;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x35", 7))
						 { state=23;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x36", 7))
						 { state=24;  }
else if (utility::string_equal(str, length, "\x47\x50\x49\x4f\x20\x32\x37", 7))
						 { state=25;  }
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

case 17:
    return checked_event_forwarding(handler_17.Key(str, length, copy));

case 18:
    return checked_event_forwarding(handler_18.Key(str, length, copy));

case 19:
    return checked_event_forwarding(handler_19.Key(str, length, copy));

case 20:
    return checked_event_forwarding(handler_20.Key(str, length, copy));

case 21:
    return checked_event_forwarding(handler_21.Key(str, length, copy));

case 22:
    return checked_event_forwarding(handler_22.Key(str, length, copy));

case 23:
    return checked_event_forwarding(handler_23.Key(str, length, copy));

case 24:
    return checked_event_forwarding(handler_24.Key(str, length, copy));

case 25:
    return checked_event_forwarding(handler_25.Key(str, length, copy));

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

case 17:
    return checked_event_forwarding(handler_17.StartArray());

case 18:
    return checked_event_forwarding(handler_18.StartArray());

case 19:
    return checked_event_forwarding(handler_19.StartArray());

case 20:
    return checked_event_forwarding(handler_20.StartArray());

case 21:
    return checked_event_forwarding(handler_21.StartArray());

case 22:
    return checked_event_forwarding(handler_22.StartArray());

case 23:
    return checked_event_forwarding(handler_23.StartArray());

case 24:
    return checked_event_forwarding(handler_24.StartArray());

case 25:
    return checked_event_forwarding(handler_25.StartArray());

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

case 17:
    return checked_event_forwarding(handler_17.EndArray(length));

case 18:
    return checked_event_forwarding(handler_18.EndArray(length));

case 19:
    return checked_event_forwarding(handler_19.EndArray(length));

case 20:
    return checked_event_forwarding(handler_20.EndArray(length));

case 21:
    return checked_event_forwarding(handler_21.EndArray(length));

case 22:
    return checked_event_forwarding(handler_22.EndArray(length));

case 23:
    return checked_event_forwarding(handler_23.EndArray(length));

case 24:
    return checked_event_forwarding(handler_24.EndArray(length));

case 25:
    return checked_event_forwarding(handler_25.EndArray(length));

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

case 17:
    return checked_event_forwarding(handler_17.StartObject());

case 18:
    return checked_event_forwarding(handler_18.StartObject());

case 19:
    return checked_event_forwarding(handler_19.StartObject());

case 20:
    return checked_event_forwarding(handler_20.StartObject());

case 21:
    return checked_event_forwarding(handler_21.StartObject());

case 22:
    return checked_event_forwarding(handler_22.StartObject());

case 23:
    return checked_event_forwarding(handler_23.StartObject());

case 24:
    return checked_event_forwarding(handler_24.StartObject());

case 25:
    return checked_event_forwarding(handler_25.StartObject());

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

case 17:
    return checked_event_forwarding(handler_17.EndObject(length));

case 18:
    return checked_event_forwarding(handler_18.EndObject(length));

case 19:
    return checked_event_forwarding(handler_19.EndObject(length));

case 20:
    return checked_event_forwarding(handler_20.EndObject(length));

case 21:
    return checked_event_forwarding(handler_21.EndObject(length));

case 22:
    return checked_event_forwarding(handler_22.EndObject(length));

case 23:
    return checked_event_forwarding(handler_23.EndObject(length));

case 24:
    return checked_event_forwarding(handler_24.EndObject(length));

case 25:
    return checked_event_forwarding(handler_25.EndObject(length));

            default:
                break;
            }
        } else {
            
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
case 17:
     handler_17.ReapError(errs); break;
case 18:
     handler_18.ReapError(errs); break;
case 19:
     handler_19.ReapError(errs); break;
case 20:
     handler_20.ReapError(errs); break;
case 21:
     handler_21.ReapError(errs); break;
case 22:
     handler_22.ReapError(errs); break;
case 23:
     handler_23.ReapError(errs); break;
case 24:
     handler_24.ReapError(errs); break;
case 25:
     handler_25.ReapError(errs); break;

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
handler_17.PrepareForReuse();
handler_18.PrepareForReuse();
handler_19.PrepareForReuse();
handler_20.PrepareForReuse();
handler_21.PrepareForReuse();
handler_22.PrepareForReuse();
handler_23.PrepareForReuse();
handler_24.PrepareForReuse();
handler_25.PrepareForReuse();

    }
};

template < class Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349 >
struct Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, ::sz::PIGPIO::Config > {

    void operator()( Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349& w, const ::sz::PIGPIO::Config& value) const
    {
        w.StartObject();

        w.Key("\x47\x50\x49\x4f\x20\x32", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_2);
w.Key("\x47\x50\x49\x4f\x20\x33", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_3);
w.Key("\x47\x50\x49\x4f\x20\x34", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_4);
w.Key("\x47\x50\x49\x4f\x20\x35", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_5);
w.Key("\x47\x50\x49\x4f\x20\x36", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_6);
w.Key("\x47\x50\x49\x4f\x20\x37", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_7);
w.Key("\x47\x50\x49\x4f\x20\x38", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_8);
w.Key("\x47\x50\x49\x4f\x20\x39", 6, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_9);
w.Key("\x47\x50\x49\x4f\x20\x31\x30", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_10);
w.Key("\x47\x50\x49\x4f\x20\x31\x31", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_11);
w.Key("\x47\x50\x49\x4f\x20\x31\x32", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_12);
w.Key("\x47\x50\x49\x4f\x20\x31\x33", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_13);
w.Key("\x47\x50\x49\x4f\x20\x31\x34", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_14);
w.Key("\x47\x50\x49\x4f\x20\x31\x35", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_15);
w.Key("\x47\x50\x49\x4f\x20\x31\x36", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_16);
w.Key("\x47\x50\x49\x4f\x20\x31\x37", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_17);
w.Key("\x47\x50\x49\x4f\x20\x31\x38", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_18);
w.Key("\x47\x50\x49\x4f\x20\x31\x39", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_19);
w.Key("\x47\x50\x49\x4f\x20\x32\x30", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_20);
w.Key("\x47\x50\x49\x4f\x20\x32\x31", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_21);
w.Key("\x47\x50\x49\x4f\x20\x32\x32", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_22);
w.Key("\x47\x50\x49\x4f\x20\x32\x33", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_23);
w.Key("\x47\x50\x49\x4f\x20\x32\x34", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_24);
w.Key("\x47\x50\x49\x4f\x20\x32\x35", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_25);
w.Key("\x47\x50\x49\x4f\x20\x32\x36", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_26);
w.Key("\x47\x50\x49\x4f\x20\x32\x37", 7, false); Serializer< Writerf8f206d8edf0a8e434b9de998c1beca38835264e2db8c30bb96e7805b83ae349, sz::PIGPIO::PWM_Config >()(w, value.gpio_27);

        w.EndObject(26);
    }

};
}



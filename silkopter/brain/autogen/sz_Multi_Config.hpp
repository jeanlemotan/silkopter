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

//struct Motor;

namespace autojsoncxx {

template <>
class SAXEventHandler< ::silk::config::Multi::Motor > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< math::vec3f > handler_0;
SAXEventHandler< bool > handler_1;bool has_position;
bool has_clockwise;

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
    return "position";
case 1:
    return "clockwise";
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
        has_position = false;
has_clockwise = false;
    }

public:
    explicit SAXEventHandler( ::silk::config::Multi::Motor * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->position)
, handler_1(&obj->clockwise)
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
            else if (utility::string_equal(str, length, "\x50\x6f\x73\x69\x74\x69\x6f\x6e", 8))
						 { state=0; has_position = true; }
else if (utility::string_equal(str, length, "\x43\x6c\x6f\x63\x6b\x77\x69\x73\x65", 9))
						 { state=1; has_clockwise = true; }
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

            default:
                break;
            }
        } else {
            if (!has_position) set_missing_required("position");
if (!has_clockwise) set_missing_required("clockwise");
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

    }
};

template < class Writer538b274f76b1700356c4f6eea1d3b65d5ff3214a2c54783039bbfeca6493a2e0 >
struct Serializer< Writer538b274f76b1700356c4f6eea1d3b65d5ff3214a2c54783039bbfeca6493a2e0, ::silk::config::Multi::Motor > {

    void operator()( Writer538b274f76b1700356c4f6eea1d3b65d5ff3214a2c54783039bbfeca6493a2e0& w, const ::silk::config::Multi::Motor& value) const
    {
        w.StartObject();

        w.Key("\x50\x6f\x73\x69\x74\x69\x6f\x6e", 8, false); Serializer< Writer538b274f76b1700356c4f6eea1d3b65d5ff3214a2c54783039bbfeca6493a2e0, math::vec3f >()(w, value.position);
w.Key("\x43\x6c\x6f\x63\x6b\x77\x69\x73\x65", 9, false); Serializer< Writer538b274f76b1700356c4f6eea1d3b65d5ff3214a2c54783039bbfeca6493a2e0, bool >()(w, value.clockwise);

        w.EndObject(2);
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

//struct Multi;

namespace autojsoncxx {

template <>
class SAXEventHandler< ::silk::config::Multi > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::string > handler_0;
SAXEventHandler< float > handler_1;
SAXEventHandler< float > handler_2;
SAXEventHandler< float > handler_3;
SAXEventHandler< float > handler_4;
SAXEventHandler< float > handler_5;
SAXEventHandler< float > handler_6;
SAXEventHandler< float > handler_7;
SAXEventHandler< std::vector<silk::config::Multi::Motor> > handler_8;bool has_name;
bool has_mass;
bool has_height;
bool has_radius;
bool has_motor_thrust;
bool has_motor_z_torque;
bool has_motor_acceleration;
bool has_motor_deceleration;
bool has_motors;

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
    return "name";
case 1:
    return "mass";
case 2:
    return "height";
case 3:
    return "radius";
case 4:
    return "motor_thrust";
case 5:
    return "motor_z_torque";
case 6:
    return "motor_acceleration";
case 7:
    return "motor_deceleration";
case 8:
    return "motors";
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
        has_name = false;
has_mass = false;
has_height = false;
has_radius = false;
has_motor_thrust = false;
has_motor_z_torque = false;
has_motor_acceleration = false;
has_motor_deceleration = false;
has_motors = false;
    }

public:
    explicit SAXEventHandler( ::silk::config::Multi * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->name)
, handler_1(&obj->mass)
, handler_2(&obj->height)
, handler_3(&obj->radius)
, handler_4(&obj->motor_thrust)
, handler_5(&obj->motor_z_torque)
, handler_6(&obj->motor_acceleration)
, handler_7(&obj->motor_deceleration)
, handler_8(&obj->motors)
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
            else if (utility::string_equal(str, length, "\x4e\x61\x6d\x65", 4))
						 { state=0; has_name = true; }
else if (utility::string_equal(str, length, "\x4d\x61\x73\x73\x20\x28\x4b\x67\x29", 9))
						 { state=1; has_mass = true; }
else if (utility::string_equal(str, length, "\x48\x65\x69\x67\x68\x74\x20\x28\x6d\x29", 10))
						 { state=2; has_height = true; }
else if (utility::string_equal(str, length, "\x52\x61\x64\x69\x75\x73\x20\x28\x6d\x29", 10))
						 { state=3; has_radius = true; }
else if (utility::string_equal(str, length, "\x4d\x6f\x74\x6f\x72\x20\x54\x68\x72\x75\x73\x74\x20\x40\x20\x31\x30\x30\x25\x20\x28\x4e\x29", 23))
						 { state=4; has_motor_thrust = true; }
else if (utility::string_equal(str, length, "\x4d\x6f\x74\x6f\x72\x20\x5a\x20\x54\x6f\x72\x71\x75\x65\x20\x40\x20\x31\x30\x30\x25\x20\x28\x4e\x6d\x29", 26))
						 { state=5; has_motor_z_torque = true; }
else if (utility::string_equal(str, length, "\x4d\x6f\x74\x6f\x72\x20\x41\x63\x63\x65\x6c\x65\x72\x61\x74\x69\x6f\x6e\x20\x28\x73\x29", 22))
						 { state=6; has_motor_acceleration = true; }
else if (utility::string_equal(str, length, "\x4d\x6f\x74\x6f\x72\x20\x44\x65\x63\x65\x6c\x65\x72\x61\x74\x69\x6f\x6e\x20\x28\x73\x29", 22))
						 { state=7; has_motor_deceleration = true; }
else if (utility::string_equal(str, length, "\x4d\x6f\x74\x6f\x72\x73", 6))
						 { state=8; has_motors = true; }
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

            default:
                break;
            }
        } else {
            if (!has_name) set_missing_required("name");
if (!has_mass) set_missing_required("mass");
if (!has_height) set_missing_required("height");
if (!has_radius) set_missing_required("radius");
if (!has_motor_thrust) set_missing_required("motor_thrust");
if (!has_motor_z_torque) set_missing_required("motor_z_torque");
if (!has_motor_acceleration) set_missing_required("motor_acceleration");
if (!has_motor_deceleration) set_missing_required("motor_deceleration");
if (!has_motors) set_missing_required("motors");
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

    }
};

template < class Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf >
struct Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, ::silk::config::Multi > {

    void operator()( Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf& w, const ::silk::config::Multi& value) const
    {
        w.StartObject();

        w.Key("\x4e\x61\x6d\x65", 4, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, std::string >()(w, value.name);
w.Key("\x4d\x61\x73\x73\x20\x28\x4b\x67\x29", 9, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.mass);
w.Key("\x48\x65\x69\x67\x68\x74\x20\x28\x6d\x29", 10, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.height);
w.Key("\x52\x61\x64\x69\x75\x73\x20\x28\x6d\x29", 10, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.radius);
w.Key("\x4d\x6f\x74\x6f\x72\x20\x54\x68\x72\x75\x73\x74\x20\x40\x20\x31\x30\x30\x25\x20\x28\x4e\x29", 23, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.motor_thrust);
w.Key("\x4d\x6f\x74\x6f\x72\x20\x5a\x20\x54\x6f\x72\x71\x75\x65\x20\x40\x20\x31\x30\x30\x25\x20\x28\x4e\x6d\x29", 26, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.motor_z_torque);
w.Key("\x4d\x6f\x74\x6f\x72\x20\x41\x63\x63\x65\x6c\x65\x72\x61\x74\x69\x6f\x6e\x20\x28\x73\x29", 22, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.motor_acceleration);
w.Key("\x4d\x6f\x74\x6f\x72\x20\x44\x65\x63\x65\x6c\x65\x72\x61\x74\x69\x6f\x6e\x20\x28\x73\x29", 22, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, float >()(w, value.motor_deceleration);
w.Key("\x4d\x6f\x74\x6f\x72\x73", 6, false); Serializer< Writere0fc10ae43b444565803daece3031f5b66e245697452a7e6c119402d648195bf, std::vector<silk::config::Multi::Motor> >()(w, value.motors);

        w.EndObject(9);
    }

};
}



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

struct PIDs {
 util::PID_Params yaw_rate;
util::PID_Params pitch_rate;
util::PID_Params roll_rate;
util::PID_Params altitude_rate;
util::PID_Params yaw;
util::PID_Params pitch;
util::PID_Params roll;
util::PID_Params altitude;

explicit PIDs():yaw_rate(), pitch_rate(), roll_rate(), altitude_rate(), yaw(), pitch(), roll(), altitude() {  }


 
};

namespace autojsoncxx {

template <>
class SAXEventHandler< ::PIDs > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< util::PID_Params > handler_0;
SAXEventHandler< util::PID_Params > handler_1;
SAXEventHandler< util::PID_Params > handler_2;
SAXEventHandler< util::PID_Params > handler_3;
SAXEventHandler< util::PID_Params > handler_4;
SAXEventHandler< util::PID_Params > handler_5;
SAXEventHandler< util::PID_Params > handler_6;
SAXEventHandler< util::PID_Params > handler_7;bool has_yaw_rate;
bool has_pitch_rate;
bool has_roll_rate;
bool has_altitude_rate;
bool has_yaw;
bool has_pitch;
bool has_roll;
bool has_altitude;

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
    return "yaw_rate";
case 1:
    return "pitch_rate";
case 2:
    return "roll_rate";
case 3:
    return "altitude_rate";
case 4:
    return "yaw";
case 5:
    return "pitch";
case 6:
    return "roll";
case 7:
    return "altitude";
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
        has_yaw_rate = false;
has_pitch_rate = false;
has_roll_rate = false;
has_altitude_rate = false;
has_yaw = false;
has_pitch = false;
has_roll = false;
has_altitude = false;
    }

public:
    explicit SAXEventHandler( ::PIDs * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->yaw_rate)
, handler_1(&obj->pitch_rate)
, handler_2(&obj->roll_rate)
, handler_3(&obj->altitude_rate)
, handler_4(&obj->yaw)
, handler_5(&obj->pitch)
, handler_6(&obj->roll)
, handler_7(&obj->altitude)
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
            else if (utility::string_equal(str, length, "\x79\x61\x77\x5f\x72\x61\x74\x65", 8))
						 { state=0; has_yaw_rate = true; }
else if (utility::string_equal(str, length, "\x70\x69\x74\x63\x68\x5f\x72\x61\x74\x65", 10))
						 { state=1; has_pitch_rate = true; }
else if (utility::string_equal(str, length, "\x72\x6f\x6c\x6c\x5f\x72\x61\x74\x65", 9))
						 { state=2; has_roll_rate = true; }
else if (utility::string_equal(str, length, "\x61\x6c\x74\x69\x74\x75\x64\x65\x5f\x72\x61\x74\x65", 13))
						 { state=3; has_altitude_rate = true; }
else if (utility::string_equal(str, length, "\x79\x61\x77", 3))
						 { state=4; has_yaw = true; }
else if (utility::string_equal(str, length, "\x70\x69\x74\x63\x68", 5))
						 { state=5; has_pitch = true; }
else if (utility::string_equal(str, length, "\x72\x6f\x6c\x6c", 4))
						 { state=6; has_roll = true; }
else if (utility::string_equal(str, length, "\x61\x6c\x74\x69\x74\x75\x64\x65", 8))
						 { state=7; has_altitude = true; }
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
            if (!has_yaw_rate) set_missing_required("yaw_rate");
if (!has_pitch_rate) set_missing_required("pitch_rate");
if (!has_roll_rate) set_missing_required("roll_rate");
if (!has_altitude_rate) set_missing_required("altitude_rate");
if (!has_yaw) set_missing_required("yaw");
if (!has_pitch) set_missing_required("pitch");
if (!has_roll) set_missing_required("roll");
if (!has_altitude) set_missing_required("altitude");
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

template < class Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd >
struct Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, ::PIDs > {

    void operator()( Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd& w, const ::PIDs& value) const
    {
        w.StartObject();

        w.Key("\x79\x61\x77\x5f\x72\x61\x74\x65"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.yaw_rate);
w.Key("\x70\x69\x74\x63\x68\x5f\x72\x61\x74\x65"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.pitch_rate);
w.Key("\x72\x6f\x6c\x6c\x5f\x72\x61\x74\x65"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.roll_rate);
w.Key("\x61\x6c\x74\x69\x74\x75\x64\x65\x5f\x72\x61\x74\x65"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.altitude_rate);
w.Key("\x79\x61\x77"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.yaw);
w.Key("\x70\x69\x74\x63\x68"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.pitch);
w.Key("\x72\x6f\x6c\x6c"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.roll);
w.Key("\x61\x6c\x74\x69\x74\x75\x64\x65"); Serializer< Writer997e4a334c890b96f51b3af1a89d5b252602e93ce78eb380d8eecb8eee5163bd, util::PID_Params >()(w, value.altitude);

        w.EndObject();
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

struct UAV_Config {
 std::vector<silk::Motor_Mixer::Motor_Info> motors;
PIDs pids;
float max_throttle_rate;
float max_pitch_rate;
float max_roll_rate;
float max_yaw_rate;
float max_altitude_rate;

explicit UAV_Config():motors(), pids(), max_throttle_rate(), max_pitch_rate(), max_roll_rate(), max_yaw_rate(), max_altitude_rate() {  }


 
};

namespace autojsoncxx {

template <>
class SAXEventHandler< ::UAV_Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::vector<silk::Motor_Mixer::Motor_Info> > handler_0;
SAXEventHandler< PIDs > handler_1;
SAXEventHandler< float > handler_2;
SAXEventHandler< float > handler_3;
SAXEventHandler< float > handler_4;
SAXEventHandler< float > handler_5;
SAXEventHandler< float > handler_6;bool has_motors;
bool has_pids;
bool has_max_throttle_rate;
bool has_max_pitch_rate;
bool has_max_roll_rate;
bool has_max_yaw_rate;
bool has_max_altitude_rate;

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
    return "motors";
case 1:
    return "pids";
case 2:
    return "max_throttle_rate";
case 3:
    return "max_pitch_rate";
case 4:
    return "max_roll_rate";
case 5:
    return "max_yaw_rate";
case 6:
    return "max_altitude_rate";
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
        has_motors = false;
has_pids = false;
has_max_throttle_rate = false;
has_max_pitch_rate = false;
has_max_roll_rate = false;
has_max_yaw_rate = false;
has_max_altitude_rate = false;
    }

public:
    explicit SAXEventHandler( ::UAV_Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->motors)
, handler_1(&obj->pids)
, handler_2(&obj->max_throttle_rate)
, handler_3(&obj->max_pitch_rate)
, handler_4(&obj->max_roll_rate)
, handler_5(&obj->max_yaw_rate)
, handler_6(&obj->max_altitude_rate)
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
            else if (utility::string_equal(str, length, "\x6d\x6f\x74\x6f\x72\x73", 6))
						 { state=0; has_motors = true; }
else if (utility::string_equal(str, length, "\x70\x69\x64\x73", 4))
						 { state=1; has_pids = true; }
else if (utility::string_equal(str, length, "\x6d\x61\x78\x5f\x74\x68\x72\x6f\x74\x74\x6c\x65\x5f\x72\x61\x74\x65", 17))
						 { state=2; has_max_throttle_rate = true; }
else if (utility::string_equal(str, length, "\x6d\x61\x78\x5f\x70\x69\x74\x63\x68\x5f\x72\x61\x74\x65", 14))
						 { state=3; has_max_pitch_rate = true; }
else if (utility::string_equal(str, length, "\x6d\x61\x78\x5f\x72\x6f\x6c\x6c\x5f\x72\x61\x74\x65", 13))
						 { state=4; has_max_roll_rate = true; }
else if (utility::string_equal(str, length, "\x6d\x61\x78\x5f\x79\x61\x77\x5f\x72\x61\x74\x65", 12))
						 { state=5; has_max_yaw_rate = true; }
else if (utility::string_equal(str, length, "\x6d\x61\x78\x5f\x61\x6c\x74\x69\x74\x75\x64\x65\x5f\x72\x61\x74\x65", 17))
						 { state=6; has_max_altitude_rate = true; }
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

            default:
                break;
            }
        } else {
            if (!has_motors) set_missing_required("motors");
if (!has_pids) set_missing_required("pids");
if (!has_max_throttle_rate) set_missing_required("max_throttle_rate");
if (!has_max_pitch_rate) set_missing_required("max_pitch_rate");
if (!has_max_roll_rate) set_missing_required("max_roll_rate");
if (!has_max_yaw_rate) set_missing_required("max_yaw_rate");
if (!has_max_altitude_rate) set_missing_required("max_altitude_rate");
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

template < class Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519 >
struct Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, ::UAV_Config > {

    void operator()( Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519& w, const ::UAV_Config& value) const
    {
        w.StartObject();

        w.Key("\x6d\x6f\x74\x6f\x72\x73"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, std::vector<silk::Motor_Mixer::Motor_Info> >()(w, value.motors);
w.Key("\x70\x69\x64\x73"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, PIDs >()(w, value.pids);
w.Key("\x6d\x61\x78\x5f\x74\x68\x72\x6f\x74\x74\x6c\x65\x5f\x72\x61\x74\x65"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, float >()(w, value.max_throttle_rate);
w.Key("\x6d\x61\x78\x5f\x70\x69\x74\x63\x68\x5f\x72\x61\x74\x65"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, float >()(w, value.max_pitch_rate);
w.Key("\x6d\x61\x78\x5f\x72\x6f\x6c\x6c\x5f\x72\x61\x74\x65"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, float >()(w, value.max_roll_rate);
w.Key("\x6d\x61\x78\x5f\x79\x61\x77\x5f\x72\x61\x74\x65"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, float >()(w, value.max_yaw_rate);
w.Key("\x6d\x61\x78\x5f\x61\x6c\x74\x69\x74\x75\x64\x65\x5f\x72\x61\x74\x65"); Serializer< Writerab92aee6882a1f08fe079d580645375d76aafd2423f205b8af89a691d8f01519, float >()(w, value.max_altitude_rate);

        w.EndObject();
    }

};
}



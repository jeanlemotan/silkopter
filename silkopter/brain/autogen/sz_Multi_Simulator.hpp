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

namespace sz { namespace Multi_Simulator { struct Init_Params {
 uint32_t throttle_rate;
uint32_t acceleration_rate;
uint32_t angular_velocity_rate;
uint32_t magnetic_field_rate;
uint32_t pressure_rate;
uint32_t temperature_rate;
uint32_t distance_rate;
uint32_t gps_rate;

explicit Init_Params():throttle_rate(100), acceleration_rate(1000), angular_velocity_rate(1000), magnetic_field_rate(100), pressure_rate(100), temperature_rate(10), distance_rate(12), gps_rate(5) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::Multi_Simulator::Init_Params > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< uint32_t > handler_0;
SAXEventHandler< uint32_t > handler_1;
SAXEventHandler< uint32_t > handler_2;
SAXEventHandler< uint32_t > handler_3;
SAXEventHandler< uint32_t > handler_4;
SAXEventHandler< uint32_t > handler_5;
SAXEventHandler< uint32_t > handler_6;
SAXEventHandler< uint32_t > handler_7;bool has_throttle_rate;
bool has_acceleration_rate;
bool has_angular_velocity_rate;
bool has_magnetic_field_rate;
bool has_pressure_rate;
bool has_temperature_rate;
bool has_distance_rate;
bool has_gps_rate;

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
    return "throttle_rate";
case 1:
    return "acceleration_rate";
case 2:
    return "angular_velocity_rate";
case 3:
    return "magnetic_field_rate";
case 4:
    return "pressure_rate";
case 5:
    return "temperature_rate";
case 6:
    return "distance_rate";
case 7:
    return "gps_rate";
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
        has_throttle_rate = false;
has_acceleration_rate = false;
has_angular_velocity_rate = false;
has_magnetic_field_rate = false;
has_pressure_rate = false;
has_temperature_rate = false;
has_distance_rate = false;
has_gps_rate = false;
    }

public:
    explicit SAXEventHandler( ::sz::Multi_Simulator::Init_Params * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->throttle_rate)
, handler_1(&obj->acceleration_rate)
, handler_2(&obj->angular_velocity_rate)
, handler_3(&obj->magnetic_field_rate)
, handler_4(&obj->pressure_rate)
, handler_5(&obj->temperature_rate)
, handler_6(&obj->distance_rate)
, handler_7(&obj->gps_rate)
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
            else if (utility::string_equal(str, length, "\x54\x68\x72\x6f\x74\x74\x6c\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 18))
						 { state=0; has_throttle_rate = true; }
else if (utility::string_equal(str, length, "\x41\x63\x63\x65\x6c\x65\x72\x61\x74\x69\x6f\x6e\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 22))
						 { state=1; has_acceleration_rate = true; }
else if (utility::string_equal(str, length, "\x41\x6e\x67\x75\x6c\x61\x72\x20\x56\x65\x6c\x6f\x63\x69\x74\x79\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 26))
						 { state=2; has_angular_velocity_rate = true; }
else if (utility::string_equal(str, length, "\x4d\x61\x67\x6e\x65\x74\x69\x63\x20\x46\x69\x65\x6c\x64\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 24))
						 { state=3; has_magnetic_field_rate = true; }
else if (utility::string_equal(str, length, "\x50\x72\x65\x73\x73\x75\x72\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 18))
						 { state=4; has_pressure_rate = true; }
else if (utility::string_equal(str, length, "\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 21))
						 { state=5; has_temperature_rate = true; }
else if (utility::string_equal(str, length, "\x44\x69\x73\x74\x61\x6e\x63\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 18))
						 { state=6; has_distance_rate = true; }
else if (utility::string_equal(str, length, "\x47\x50\x53\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 13))
						 { state=7; has_gps_rate = true; }
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
            if (!has_throttle_rate) set_missing_required("throttle_rate");
if (!has_acceleration_rate) set_missing_required("acceleration_rate");
if (!has_angular_velocity_rate) set_missing_required("angular_velocity_rate");
if (!has_magnetic_field_rate) set_missing_required("magnetic_field_rate");
if (!has_pressure_rate) set_missing_required("pressure_rate");
if (!has_temperature_rate) set_missing_required("temperature_rate");
if (!has_distance_rate) set_missing_required("distance_rate");
if (!has_gps_rate) set_missing_required("gps_rate");
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

    }
};

template < class Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1 >
struct Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, ::sz::Multi_Simulator::Init_Params > {

    void operator()( Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1& w, const ::sz::Multi_Simulator::Init_Params& value) const
    {
        w.StartObject();

        w.Key("\x54\x68\x72\x6f\x74\x74\x6c\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 18, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.throttle_rate);
w.Key("\x41\x63\x63\x65\x6c\x65\x72\x61\x74\x69\x6f\x6e\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 22, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.acceleration_rate);
w.Key("\x41\x6e\x67\x75\x6c\x61\x72\x20\x56\x65\x6c\x6f\x63\x69\x74\x79\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 26, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.angular_velocity_rate);
w.Key("\x4d\x61\x67\x6e\x65\x74\x69\x63\x20\x46\x69\x65\x6c\x64\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 24, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.magnetic_field_rate);
w.Key("\x50\x72\x65\x73\x73\x75\x72\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 18, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.pressure_rate);
w.Key("\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 21, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.temperature_rate);
w.Key("\x44\x69\x73\x74\x61\x6e\x63\x65\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 18, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.distance_rate);
w.Key("\x47\x50\x53\x20\x52\x61\x74\x65\x20\x28\x48\x7a\x29", 13, false); Serializer< Writer3691670b2ec5fc8ffa61fbaaa32579c3202a80dfcc259ae0899e00921f72ebe1, uint32_t >()(w, value.gps_rate);

        w.EndObject(8);
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

namespace sz { namespace Multi_Simulator { struct Config {
 bool simulation_enabled;
bool drag_enabled;
bool ground_enabled;
bool gravity_enabled;

explicit Config():simulation_enabled(true), drag_enabled(true), ground_enabled(true), gravity_enabled(true) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::Multi_Simulator::Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< bool > handler_0;
SAXEventHandler< bool > handler_1;
SAXEventHandler< bool > handler_2;
SAXEventHandler< bool > handler_3;bool has_simulation_enabled;
bool has_drag_enabled;
bool has_ground_enabled;
bool has_gravity_enabled;

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
    return "simulation_enabled";
case 1:
    return "drag_enabled";
case 2:
    return "ground_enabled";
case 3:
    return "gravity_enabled";
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
        has_simulation_enabled = false;
has_drag_enabled = false;
has_ground_enabled = false;
has_gravity_enabled = false;
    }

public:
    explicit SAXEventHandler( ::sz::Multi_Simulator::Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->simulation_enabled)
, handler_1(&obj->drag_enabled)
, handler_2(&obj->ground_enabled)
, handler_3(&obj->gravity_enabled)
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
            else if (utility::string_equal(str, length, "\x53\x69\x6d\x75\x6c\x61\x74\x69\x6f\x6e\x20\x45\x6e\x61\x62\x6c\x65\x64", 18))
						 { state=0; has_simulation_enabled = true; }
else if (utility::string_equal(str, length, "\x44\x72\x61\x67\x20\x45\x6e\x61\x62\x6c\x65\x64", 12))
						 { state=1; has_drag_enabled = true; }
else if (utility::string_equal(str, length, "\x47\x72\x6f\x75\x6e\x64\x20\x45\x6e\x61\x62\x6c\x65\x64", 14))
						 { state=2; has_ground_enabled = true; }
else if (utility::string_equal(str, length, "\x47\x72\x61\x76\x69\x74\x79\x20\x45\x6e\x61\x62\x6c\x65\x64", 15))
						 { state=3; has_gravity_enabled = true; }
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
            if (!has_simulation_enabled) set_missing_required("simulation_enabled");
if (!has_drag_enabled) set_missing_required("drag_enabled");
if (!has_ground_enabled) set_missing_required("ground_enabled");
if (!has_gravity_enabled) set_missing_required("gravity_enabled");
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

template < class Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef >
struct Serializer< Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef, ::sz::Multi_Simulator::Config > {

    void operator()( Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef& w, const ::sz::Multi_Simulator::Config& value) const
    {
        w.StartObject();

        w.Key("\x53\x69\x6d\x75\x6c\x61\x74\x69\x6f\x6e\x20\x45\x6e\x61\x62\x6c\x65\x64", 18, false); Serializer< Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef, bool >()(w, value.simulation_enabled);
w.Key("\x44\x72\x61\x67\x20\x45\x6e\x61\x62\x6c\x65\x64", 12, false); Serializer< Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef, bool >()(w, value.drag_enabled);
w.Key("\x47\x72\x6f\x75\x6e\x64\x20\x45\x6e\x61\x62\x6c\x65\x64", 14, false); Serializer< Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef, bool >()(w, value.ground_enabled);
w.Key("\x47\x72\x61\x76\x69\x74\x79\x20\x45\x6e\x61\x62\x6c\x65\x64", 15, false); Serializer< Writerea792b52c9c9efa6bcec16dcc014708ff3a5e7b621293f760386a97732a4b5ef, bool >()(w, value.gravity_enabled);

        w.EndObject(4);
    }

};
}



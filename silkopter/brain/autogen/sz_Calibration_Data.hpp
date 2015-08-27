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

namespace sz { namespace calibration { struct Acceleration {
 double temperature;
math::vec3d bias;
math::vec3d scale;

explicit Acceleration():temperature(0), bias(), scale(1) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::calibration::Acceleration > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< double > handler_0;
SAXEventHandler< math::vec3d > handler_1;
SAXEventHandler< math::vec3d > handler_2;bool has_temperature;
bool has_bias;
bool has_scale;

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
    return "temperature";
case 1:
    return "bias";
case 2:
    return "scale";
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
        has_temperature = false;
has_bias = false;
has_scale = false;
    }

public:
    explicit SAXEventHandler( ::sz::calibration::Acceleration * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->temperature)
, handler_1(&obj->bias)
, handler_2(&obj->scale)
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
            else if (utility::string_equal(str, length, "\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65", 11))
						 { state=0; has_temperature = true; }
else if (utility::string_equal(str, length, "\x42\x69\x61\x73", 4))
						 { state=1; has_bias = true; }
else if (utility::string_equal(str, length, "\x53\x63\x61\x6c\x65", 5))
						 { state=2; has_scale = true; }
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
            if (!has_temperature) set_missing_required("temperature");
if (!has_bias) set_missing_required("bias");
if (!has_scale) set_missing_required("scale");
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

template < class Writer9e5315053059ad61be74ef5a385691ae34e06e8303a2737bec784ef55b195e2c >
struct Serializer< Writer9e5315053059ad61be74ef5a385691ae34e06e8303a2737bec784ef55b195e2c, ::sz::calibration::Acceleration > {

    void operator()( Writer9e5315053059ad61be74ef5a385691ae34e06e8303a2737bec784ef55b195e2c& w, const ::sz::calibration::Acceleration& value) const
    {
        w.StartObject();

        w.Key("\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65", 11, false); Serializer< Writer9e5315053059ad61be74ef5a385691ae34e06e8303a2737bec784ef55b195e2c, double >()(w, value.temperature);
w.Key("\x42\x69\x61\x73", 4, false); Serializer< Writer9e5315053059ad61be74ef5a385691ae34e06e8303a2737bec784ef55b195e2c, math::vec3d >()(w, value.bias);
w.Key("\x53\x63\x61\x6c\x65", 5, false); Serializer< Writer9e5315053059ad61be74ef5a385691ae34e06e8303a2737bec784ef55b195e2c, math::vec3d >()(w, value.scale);

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

namespace sz { namespace calibration { struct Acceleration_Points {
 std::vector<sz::calibration::Acceleration> points;

explicit Acceleration_Points():points() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::calibration::Acceleration_Points > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::vector<sz::calibration::Acceleration> > handler_0;bool has_points;

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
    return "points";
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
        has_points = false;
    }

public:
    explicit SAXEventHandler( ::sz::calibration::Acceleration_Points * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->points)
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
            else if (utility::string_equal(str, length, "\x50\x6f\x69\x6e\x74\x73", 6))
						 { state=0; has_points = true; }
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
            if (!has_points) set_missing_required("points");
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

template < class Writera4e57f46ad300bdbf2194524f7b238de155d0ad084b0d733fd8a83da4330e80f >
struct Serializer< Writera4e57f46ad300bdbf2194524f7b238de155d0ad084b0d733fd8a83da4330e80f, ::sz::calibration::Acceleration_Points > {

    void operator()( Writera4e57f46ad300bdbf2194524f7b238de155d0ad084b0d733fd8a83da4330e80f& w, const ::sz::calibration::Acceleration_Points& value) const
    {
        w.StartObject();

        w.Key("\x50\x6f\x69\x6e\x74\x73", 6, false); Serializer< Writera4e57f46ad300bdbf2194524f7b238de155d0ad084b0d733fd8a83da4330e80f, std::vector<sz::calibration::Acceleration> >()(w, value.points);

        w.EndObject(1);
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

namespace sz { namespace calibration { struct Angular_Velocity {
 double temperature;
math::vec3d bias;

explicit Angular_Velocity():temperature(0), bias() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::calibration::Angular_Velocity > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< double > handler_0;
SAXEventHandler< math::vec3d > handler_1;bool has_temperature;
bool has_bias;

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
    return "temperature";
case 1:
    return "bias";
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
        has_temperature = false;
has_bias = false;
    }

public:
    explicit SAXEventHandler( ::sz::calibration::Angular_Velocity * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->temperature)
, handler_1(&obj->bias)
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
            else if (utility::string_equal(str, length, "\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65", 11))
						 { state=0; has_temperature = true; }
else if (utility::string_equal(str, length, "\x42\x69\x61\x73", 4))
						 { state=1; has_bias = true; }
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
            if (!has_temperature) set_missing_required("temperature");
if (!has_bias) set_missing_required("bias");
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

template < class Writer9d57d580fafb62efe90d09c25a3467d36b11f66318fa6f37b9f92bf4b734e642 >
struct Serializer< Writer9d57d580fafb62efe90d09c25a3467d36b11f66318fa6f37b9f92bf4b734e642, ::sz::calibration::Angular_Velocity > {

    void operator()( Writer9d57d580fafb62efe90d09c25a3467d36b11f66318fa6f37b9f92bf4b734e642& w, const ::sz::calibration::Angular_Velocity& value) const
    {
        w.StartObject();

        w.Key("\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65", 11, false); Serializer< Writer9d57d580fafb62efe90d09c25a3467d36b11f66318fa6f37b9f92bf4b734e642, double >()(w, value.temperature);
w.Key("\x42\x69\x61\x73", 4, false); Serializer< Writer9d57d580fafb62efe90d09c25a3467d36b11f66318fa6f37b9f92bf4b734e642, math::vec3d >()(w, value.bias);

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

namespace sz { namespace calibration { struct Angular_Velocity_Points {
 std::vector<sz::calibration::Angular_Velocity> points;

explicit Angular_Velocity_Points():points() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::calibration::Angular_Velocity_Points > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::vector<sz::calibration::Angular_Velocity> > handler_0;bool has_points;

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
    return "points";
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
        has_points = false;
    }

public:
    explicit SAXEventHandler( ::sz::calibration::Angular_Velocity_Points * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->points)
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
            else if (utility::string_equal(str, length, "\x50\x6f\x69\x6e\x74\x73", 6))
						 { state=0; has_points = true; }
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
            if (!has_points) set_missing_required("points");
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

template < class Writerf769641e30dc589310ee1597fcdda5144e604f381c0a39a2d7a529d5f0901ae1 >
struct Serializer< Writerf769641e30dc589310ee1597fcdda5144e604f381c0a39a2d7a529d5f0901ae1, ::sz::calibration::Angular_Velocity_Points > {

    void operator()( Writerf769641e30dc589310ee1597fcdda5144e604f381c0a39a2d7a529d5f0901ae1& w, const ::sz::calibration::Angular_Velocity_Points& value) const
    {
        w.StartObject();

        w.Key("\x50\x6f\x69\x6e\x74\x73", 6, false); Serializer< Writerf769641e30dc589310ee1597fcdda5144e604f381c0a39a2d7a529d5f0901ae1, std::vector<sz::calibration::Angular_Velocity> >()(w, value.points);

        w.EndObject(1);
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

namespace sz { namespace calibration { struct Magnetic_Field {
 double temperature;
math::vec3d bias;
math::vec3d scale;

explicit Magnetic_Field():temperature(0), bias(), scale(1) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::calibration::Magnetic_Field > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< double > handler_0;
SAXEventHandler< math::vec3d > handler_1;
SAXEventHandler< math::vec3d > handler_2;bool has_temperature;
bool has_bias;
bool has_scale;

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
    return "temperature";
case 1:
    return "bias";
case 2:
    return "scale";
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
        has_temperature = false;
has_bias = false;
has_scale = false;
    }

public:
    explicit SAXEventHandler( ::sz::calibration::Magnetic_Field * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->temperature)
, handler_1(&obj->bias)
, handler_2(&obj->scale)
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
            else if (utility::string_equal(str, length, "\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65", 11))
						 { state=0; has_temperature = true; }
else if (utility::string_equal(str, length, "\x42\x69\x61\x73", 4))
						 { state=1; has_bias = true; }
else if (utility::string_equal(str, length, "\x53\x63\x61\x6c\x65", 5))
						 { state=2; has_scale = true; }
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
            if (!has_temperature) set_missing_required("temperature");
if (!has_bias) set_missing_required("bias");
if (!has_scale) set_missing_required("scale");
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

template < class Writer12d2ff68e51df99cc4c8cdbd3ad5c337902fc2332c704753594dea48b42ed30c >
struct Serializer< Writer12d2ff68e51df99cc4c8cdbd3ad5c337902fc2332c704753594dea48b42ed30c, ::sz::calibration::Magnetic_Field > {

    void operator()( Writer12d2ff68e51df99cc4c8cdbd3ad5c337902fc2332c704753594dea48b42ed30c& w, const ::sz::calibration::Magnetic_Field& value) const
    {
        w.StartObject();

        w.Key("\x54\x65\x6d\x70\x65\x72\x61\x74\x75\x72\x65", 11, false); Serializer< Writer12d2ff68e51df99cc4c8cdbd3ad5c337902fc2332c704753594dea48b42ed30c, double >()(w, value.temperature);
w.Key("\x42\x69\x61\x73", 4, false); Serializer< Writer12d2ff68e51df99cc4c8cdbd3ad5c337902fc2332c704753594dea48b42ed30c, math::vec3d >()(w, value.bias);
w.Key("\x53\x63\x61\x6c\x65", 5, false); Serializer< Writer12d2ff68e51df99cc4c8cdbd3ad5c337902fc2332c704753594dea48b42ed30c, math::vec3d >()(w, value.scale);

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

namespace sz { namespace calibration { struct Magnetic_Field_Points {
 std::vector<sz::calibration::Magnetic_Field> points;

explicit Magnetic_Field_Points():points() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::calibration::Magnetic_Field_Points > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< std::vector<sz::calibration::Magnetic_Field> > handler_0;bool has_points;

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
    return "points";
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
        has_points = false;
    }

public:
    explicit SAXEventHandler( ::sz::calibration::Magnetic_Field_Points * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->points)
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
            else if (utility::string_equal(str, length, "\x50\x6f\x69\x6e\x74\x73", 6))
						 { state=0; has_points = true; }
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
            if (!has_points) set_missing_required("points");
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

template < class Writer3c82a5e3f67b7f20accbc4924da90458fed748b824f25552ddd8c08053b99c66 >
struct Serializer< Writer3c82a5e3f67b7f20accbc4924da90458fed748b824f25552ddd8c08053b99c66, ::sz::calibration::Magnetic_Field_Points > {

    void operator()( Writer3c82a5e3f67b7f20accbc4924da90458fed748b824f25552ddd8c08053b99c66& w, const ::sz::calibration::Magnetic_Field_Points& value) const
    {
        w.StartObject();

        w.Key("\x50\x6f\x69\x6e\x74\x73", 6, false); Serializer< Writer3c82a5e3f67b7f20accbc4924da90458fed748b824f25552ddd8c08053b99c66, std::vector<sz::calibration::Magnetic_Field> >()(w, value.points);

        w.EndObject(1);
    }

};
}



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

namespace sz { namespace OpenCV_Capture { struct Quality {
 math::vec2u32 resolution;
uint32_t bitrate;

explicit Quality():resolution(), bitrate() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::OpenCV_Capture::Quality > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< math::vec2u32 > handler_0;
SAXEventHandler< uint32_t > handler_1;bool has_resolution;
bool has_bitrate;

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
    return "resolution";
case 1:
    return "bitrate";
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
        has_resolution = false;
has_bitrate = false;
    }

public:
    explicit SAXEventHandler( ::sz::OpenCV_Capture::Quality * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->resolution)
, handler_1(&obj->bitrate)
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
            else if (utility::string_equal(str, length, "\x52\x65\x73\x6f\x6c\x75\x74\x69\x6f\x6e", 10))
						 { state=0; has_resolution = true; }
else if (utility::string_equal(str, length, "\x42\x69\x74\x72\x61\x74\x65\x20\x28\x62\x70\x73\x29", 13))
						 { state=1; has_bitrate = true; }
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
            if (!has_resolution) set_missing_required("resolution");
if (!has_bitrate) set_missing_required("bitrate");
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

template < class Writer42a848989ff93cce72304b423292ce9b73b26792ac4ae9ce57d1ee8d5e3d7fcd >
struct Serializer< Writer42a848989ff93cce72304b423292ce9b73b26792ac4ae9ce57d1ee8d5e3d7fcd, ::sz::OpenCV_Capture::Quality > {

    void operator()( Writer42a848989ff93cce72304b423292ce9b73b26792ac4ae9ce57d1ee8d5e3d7fcd& w, const ::sz::OpenCV_Capture::Quality& value) const
    {
        w.StartObject();

        w.Key("\x52\x65\x73\x6f\x6c\x75\x74\x69\x6f\x6e", 10, false); Serializer< Writer42a848989ff93cce72304b423292ce9b73b26792ac4ae9ce57d1ee8d5e3d7fcd, math::vec2u32 >()(w, value.resolution);
w.Key("\x42\x69\x74\x72\x61\x74\x65\x20\x28\x62\x70\x73\x29", 13, false); Serializer< Writer42a848989ff93cce72304b423292ce9b73b26792ac4ae9ce57d1ee8d5e3d7fcd, uint32_t >()(w, value.bitrate);

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

namespace sz { namespace OpenCV_Capture { struct Init_Params {
 uint32_t device;
uint32_t fps;
sz::OpenCV_Capture::Quality low;
sz::OpenCV_Capture::Quality high;
sz::OpenCV_Capture::Quality recording;

explicit Init_Params():device(0), fps(30), low(), high(), recording() {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::OpenCV_Capture::Init_Params > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< uint32_t > handler_0;
SAXEventHandler< uint32_t > handler_1;
SAXEventHandler< sz::OpenCV_Capture::Quality > handler_2;
SAXEventHandler< sz::OpenCV_Capture::Quality > handler_3;
SAXEventHandler< sz::OpenCV_Capture::Quality > handler_4;bool has_device;
bool has_fps;
bool has_low;
bool has_high;
bool has_recording;

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
    return "device";
case 1:
    return "fps";
case 2:
    return "low";
case 3:
    return "high";
case 4:
    return "recording";
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
        has_device = false;
has_fps = false;
has_low = false;
has_high = false;
has_recording = false;
    }

public:
    explicit SAXEventHandler( ::sz::OpenCV_Capture::Init_Params * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->device)
, handler_1(&obj->fps)
, handler_2(&obj->low)
, handler_3(&obj->high)
, handler_4(&obj->recording)
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
            else if (utility::string_equal(str, length, "\x44\x65\x76\x69\x63\x65", 6))
						 { state=0; has_device = true; }
else if (utility::string_equal(str, length, "\x46\x50\x53", 3))
						 { state=1; has_fps = true; }
else if (utility::string_equal(str, length, "\x6c\x6f\x77", 3))
						 { state=2; has_low = true; }
else if (utility::string_equal(str, length, "\x68\x69\x67\x68", 4))
						 { state=3; has_high = true; }
else if (utility::string_equal(str, length, "\x72\x65\x63\x6f\x72\x64\x69\x6e\x67", 9))
						 { state=4; has_recording = true; }
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

            default:
                break;
            }
        } else {
            if (!has_device) set_missing_required("device");
if (!has_fps) set_missing_required("fps");
if (!has_low) set_missing_required("low");
if (!has_high) set_missing_required("high");
if (!has_recording) set_missing_required("recording");
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

    }
};

template < class Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be >
struct Serializer< Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be, ::sz::OpenCV_Capture::Init_Params > {

    void operator()( Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be& w, const ::sz::OpenCV_Capture::Init_Params& value) const
    {
        w.StartObject();

        w.Key("\x44\x65\x76\x69\x63\x65", 6, false); Serializer< Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be, uint32_t >()(w, value.device);
w.Key("\x46\x50\x53", 3, false); Serializer< Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be, uint32_t >()(w, value.fps);
w.Key("\x6c\x6f\x77", 3, false); Serializer< Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be, sz::OpenCV_Capture::Quality >()(w, value.low);
w.Key("\x68\x69\x67\x68", 4, false); Serializer< Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be, sz::OpenCV_Capture::Quality >()(w, value.high);
w.Key("\x72\x65\x63\x6f\x72\x64\x69\x6e\x67", 9, false); Serializer< Writer896da2aa162017857d4cc67077779a46b253a484a7665acbb67decfa65a1d4be, sz::OpenCV_Capture::Quality >()(w, value.recording);

        w.EndObject(5);
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

namespace sz { namespace OpenCV_Capture { struct Config {
 uint32_t quality;

explicit Config():quality(0) {  }


 
}; }
 }


namespace autojsoncxx {

template <>
class SAXEventHandler< ::sz::OpenCV_Capture::Config > {
private:
    utility::scoped_ptr<error::ErrorBase> the_error;
    int state;
    int depth;

    SAXEventHandler< uint32_t > handler_0;bool has_quality;

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
    return "quality";
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
        has_quality = false;
    }

public:
    explicit SAXEventHandler( ::sz::OpenCV_Capture::Config * obj)
        : state(-1)
        , depth(0)
        , handler_0(&obj->quality)
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
            else if (utility::string_equal(str, length, "\x51\x75\x61\x6c\x69\x74\x79\x20\x28\x30\x2e\x2e\x31\x29", 14))
						 { state=0; has_quality = true; }
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
            if (!has_quality) set_missing_required("quality");
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

template < class Writere74469bbd41dce1b6e95faaeaf9d334d458c623f6fc939b3f941b651a0b77cdd >
struct Serializer< Writere74469bbd41dce1b6e95faaeaf9d334d458c623f6fc939b3f941b651a0b77cdd, ::sz::OpenCV_Capture::Config > {

    void operator()( Writere74469bbd41dce1b6e95faaeaf9d334d458c623f6fc939b3f941b651a0b77cdd& w, const ::sz::OpenCV_Capture::Config& value) const
    {
        w.StartObject();

        w.Key("\x51\x75\x61\x6c\x69\x74\x79\x20\x28\x30\x2e\x2e\x31\x29", 14, false); Serializer< Writere74469bbd41dce1b6e95faaeaf9d334d458c623f6fc939b3f941b651a0b77cdd, uint32_t >()(w, value.quality);

        w.EndObject(1);
    }

};
}



#pragma once

#include "common/input/Camera_Input.h"

namespace silk
{
    struct Camera_Input;

    class Camera : q::util::Noncopyable
    {
    public:
        Camera(size_t fps = 30);
        ~Camera();

        typedef std::function<void(uint8_t const* data, size_t size)> Callback;

        void setup_high_quality(math::vec2u32 const& resolution, size_t bitrate);
        void setup_medium_quality(math::vec2u32 const& resolution, size_t bitrate);
        void setup_low_quality(math::vec2u32 const& resolution, size_t bitrate);

        void set_stream_callback(Callback cb);

        enum class Result
        {
            OK,
            FAILED
        };

        auto start() -> Result;
        void stop();

        auto start_recording() -> Result;
        void stop_recording();

        void set_iso(camera_input::Iso iso);
        void set_shutter_speed(camera_input::Shutter_Speed ss);
        void set_stream_quality(camera_input::Stream_Quality sq);

        void process_input(Camera_Input const& input);

        struct Impl;
    private:
        std::shared_ptr<Impl> m_impl;
        size_t m_fps = 0;

        void file_callback(uint8_t const* data, size_t size);
        void create_file_sink();

        std::shared_ptr<q::data::File_Sink> m_file_sink;
        camera_input::Iso m_iso = 0;
        camera_input::Shutter_Speed m_shutter_speed{0};
        camera_input::Stream_Quality m_stream_quality = camera_input::Stream_Quality::MEDIUM;

        auto create_components() -> Result;

        void set_active_streams(bool high, bool medium, bool low);
    };


}


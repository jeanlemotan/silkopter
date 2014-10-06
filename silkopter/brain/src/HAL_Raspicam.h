#pragma once

namespace silk
{

class HAL_Camera_MMAL : q::util::Noncopyable
{
public:
    ~HAL_Camera_MMAL() {}

    struct Params
    {
        size_t fps = 30;
    };

    //----------------------------------------------------------------------

    typedef std::function<void(uint8_t const* data, size_t size)> Data_Available_Callback;

    void setup_record_quality(math::vec2u32 const& resolution, size_t bitrate);
    void setup_high_quality(math::vec2u32 const& resolution, size_t bitrate);
    void setup_medium_quality(math::vec2u32 const& resolution, size_t bitrate);
    void setup_low_quality(math::vec2u32 const& resolution, size_t bitrate);

    void set_data_callback(Data_Available_Callback cb);

    enum class Result : uint8_t
    {
        OK,
        FAILED
    };

    auto start_capture() -> Result;
    auto start_recording() -> Result;

    void stop_capture();
    void stop_recording();

    void set_iso(camera_input::Iso iso);
    void set_shutter_speed(camera_input::Shutter_Speed ss);
    void set_quality(camera_input::Stream_Quality sq);

    //----------------------------------------------------------------------
    struct Impl;
    void process();

private:
    std::shared_ptr<Impl> m_impl;
    size_t m_fps = 30;

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

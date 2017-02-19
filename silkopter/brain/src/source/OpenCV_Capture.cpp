#include "FCStdAfx.h"
#include "OpenCV_Capture.h"
/*
#include "hal.def.h"
//#include "sz_OpenCV_Capture.hpp"

//#undef RASPBERRY_PI

#if !defined RASPBERRY_PI
#   include "opencv2/highgui/highgui.hpp"
#endif

namespace silk
{
namespace node
{


#if !defined RASPBERRY_PI

struct OpenCV_Capture::Impl
{
    std::mutex mutex;

    //camera
    cv::VideoCapture capture;

    struct Encoder_Data
    {
        cv::VideoWriter encoder;
        sz::OpenCV_Capture::Quality* quality = nullptr;

        std::vector<uint8_t> data;
        std::mutex data_mutex;

        std::atomic<bool> is_active{false};

        Clock::time_point start;

        OpenCV_Capture::Data_Available_Callback callback;
    };

    Encoder_Data recording;
    Encoder_Data high;
    Encoder_Data low;

    std::thread capture_thread;
    bool capture_thread_alive = true;

    size_t frame_idx = 0;
};

#endif

OpenCV_Capture::OpenCV_Capture(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::OpenCV_Capture_Descriptor())
    , m_config(new hal::OpenCV_Capture_Config())
{
    QLOG_TOPIC("OpenCV_Capture");
#if !defined RASPBERRY_PI
    m_impl.reset(new Impl);

    m_impl->recording.is_active = false;
    m_impl->high.is_active = false;
    m_impl->low.is_active = false;

    m_impl->recording.callback = std::bind(&OpenCV_Capture::file_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    m_impl->high.callback = std::bind(&OpenCV_Capture::streaming_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    m_impl->low.callback = std::bind(&OpenCV_Capture::streaming_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
#endif

    m_descriptor->fps = 30;

    m_descriptor->low.resolution.set(320, 240);
    m_descriptor->low.bitrate = 100000;

    m_descriptor->high.resolution.set(640, 480);
    m_descriptor->high.bitrate = 2000000;

    m_descriptor->recording.resolution.set(1280, 960);
    m_descriptor->recording.bitrate = 8000000;
}
OpenCV_Capture::~OpenCV_Capture()
{
    QLOG_TOPIC("~OpenCV_Capture");

#if !defined RASPBERRY_PI

#endif

    m_stream = std::make_shared<Stream>();
}

auto OpenCV_Capture::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Video";
    outputs[0].stream = m_stream;
    return outputs;
}

auto OpenCV_Capture::init(hal::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("OpenCV_Capture::init");

    auto specialized = dynamic_cast<hal::OpenCV_Capture_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}
auto OpenCV_Capture::init() -> bool
{
#if !defined RASPBERRY_PI
    QLOG_TOPIC("OpenCV_Capture::init");
    if (m_impl->capture.isOpened())
    {
        return true;
    }

    m_descriptor->fps = math::clamp<size_t>(m_descriptor->fps, 10, 60);
    m_stream->rate = m_descriptor->fps;

    m_impl->recording.quality = &m_descriptor->recording;
    m_impl->high.quality = &m_descriptor->high;
    m_impl->low.quality = &m_descriptor->low;

    if (!create_components())
    {
        return false;
    }

    set_active_streams(false, m_config->quality);

    return true;
#else
    QLOGE("OpenCV_Capture is only supported on the desktop build.");
    return false;
#endif
}

auto OpenCV_Capture::start(Clock::time_point tp) -> bool
{
    return true;
}


auto OpenCV_Capture::set_config(hal::INode_Config const& config) -> bool
{
    auto specialized = dynamic_cast<hal::OpenCV_Capture_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    set_active_streams(false, m_config->quality);

    return true;
}
auto OpenCV_Capture::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto OpenCV_Capture::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> OpenCV_Capture::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

void OpenCV_Capture::shutdown()
{
#if !defined RASPBERRY_PI
    QASSERT(m_impl->capture.isOpened());
    m_impl->capture_thread_alive = false;
    if (m_impl->capture_thread.joinable())
    {
        m_impl->capture_thread.join();
    }
#endif
}


void OpenCV_Capture::process()
{
    QLOG_TOPIC("OpenCV_Capture::process");

    std::lock_guard<std::mutex> lg(m_temp_samples.mutex);

    //put the samples back
    for (auto& sample: m_stream->samples)
    {
        m_temp_samples.samples.push_back(std::move(sample));
    }

    m_stream->samples.resize(m_temp_samples.count);
    if (m_temp_samples.count > 0)
    {
        for (size_t i = 0; i < m_temp_samples.count; i++)
        {
            m_stream->samples[i] = std::move(m_temp_samples.samples[i]);
        }
        m_temp_samples.samples.erase(m_temp_samples.samples.begin(), m_temp_samples.samples.begin() + m_temp_samples.count);
        m_temp_samples.count = 0;
    }
}


void OpenCV_Capture::file_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe)
{
    if (!data || size == 0)
    {
        return;
    }
    auto sink = m_file_sink; //make a copy in case the other thread destroyes the file
    if (!sink || !sink->is_open())
    {
        return;
    }
    sink->write(data, size);
}

void OpenCV_Capture::streaming_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe)
{
    if (!data || size == 0)
    {
        return;
    }

    std::lock_guard<std::mutex> lg(m_temp_samples.mutex);

    //m_temp_samples.count = 0;

    if (m_temp_samples.samples.size() <= m_temp_samples.count)
    {
        m_temp_samples.samples.resize(m_temp_samples.samples.size() + m_temp_samples.count + 1);
    }

    auto& sample = m_temp_samples.samples[m_temp_samples.count];
    sample.value.type = Stream::Value::Type::H264;
    sample.value.is_keyframe = is_keyframe;
    sample.value.resolution = resolution;
    sample.value.data.resize(size);
    std::copy(data, data + size, sample.value.data.begin());

    m_temp_samples.count++;
}

void OpenCV_Capture::set_active_streams(bool recording, uint32_t quality)
{
#if !defined RASPBERRY_PI
    std::lock_guard<std::mutex> lg(m_impl->mutex);

    bool high = quality == 0;
    bool low = quality == 1;

    if (m_impl->recording.is_active == recording &&
        m_impl->high.is_active == high &&
        m_impl->low.is_active == low)
    {
        return;
    }

    QLOGI("activating streams recording {}, quality {}", recording, quality);

    math::vec2u32 resolution = high ? m_descriptor->high.resolution : m_descriptor->low.resolution;
    bool res = m_impl->capture.set(CV_CAP_PROP_FRAME_WIDTH, resolution.x);
    res &= m_impl->capture.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.y);
    if (!res)
    {
        QLOGE("Failed to configure device {}.", m_descriptor->device);
        return;
    }

#endif
}

///////////////////////////////////////////////////////////////////////////////////////

auto OpenCV_Capture::create_components() -> bool
{
#if !defined RASPBERRY_PI

    m_impl->capture.open(m_descriptor->device);
    if (!m_impl->capture.isOpened())
    {
        QLOGE("Failed open device {}.", m_descriptor->device);
        return false;
    }

    bool res = m_impl->capture.set(CV_CAP_PROP_FRAME_WIDTH, m_descriptor->high.resolution.x);
    res &= m_impl->capture.set(CV_CAP_PROP_FRAME_HEIGHT, m_descriptor->high.resolution.y);
    res &= m_impl->capture.set(CV_CAP_PROP_FPS, m_descriptor->fps);

    if (!res)
    {
        QLOGE("Failed to configure device {}.", m_descriptor->device);
        return false;
    }

    m_impl->capture_thread = std::thread([this]()
    {
        cv::Mat frame;
        while (m_impl->capture_thread_alive)
        {
            m_impl->capture.retrieve(frame);
        }
    });

#endif
    return true;
}

void OpenCV_Capture::create_file_sink()
{
    char mbstr[256] = {0};
    std::time_t t = std::time(nullptr);
    if (!std::strftime(mbstr, 100, "%e-%m-%Y-%H-%M-%S", std::localtime(&t)))
    {
        strcpy(mbstr, "no-date");
    }

    size_t file_idx = 0;

    do
    {
        std::string filepath;
        q::util::format_emplace(filepath, "capture/{}-{}", mbstr, file_idx);
        if (!q::util::fs::exists(q::Path(filepath)))
        {
            m_file_sink.reset(new q::data::File_Sink(q::Path(filepath)));
            if (m_file_sink->is_open())
            {
                return;
            }
            m_file_sink.reset();
        }
        file_idx++;
    } while (file_idx < 16);

    QLOGW("Failed to create capture file.");
    m_file_sink.reset();
}


auto OpenCV_Capture::start_recording() -> bool
{
#if !defined RASPBERRY_PI
    if (!!q::util::fs::is_folder(q::Path("capture")) && !q::util::fs::create_folder(q::Path("capture")))
    {
        QLOGW("Cannot create capture folder");
        return false;
    }

    if (!m_file_sink)
    {
        create_file_sink();
    }

    std::lock_guard<std::mutex> lg(m_impl->mutex);
#endif
    return true;
}
void OpenCV_Capture::stop_recording()
{
#if !defined RASPBERRY_PI
    m_file_sink.reset();

    std::lock_guard<std::mutex> lg(m_impl->mutex);
#endif
}


}
}

*/

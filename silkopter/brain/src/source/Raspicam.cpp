#include "BrainStdAfx.h"
#include "Raspicam.h"

#include "uav.def.h"
//#include "sz_Raspicam.hpp"

//#undef RASPBERRY_PI

#if defined RASPBERRY_PI

extern "C"
{
    #include "interface/vcos/vcos.h"
    #include "interface/mmal/mmal.h"
    #include "interface/mmal/mmal_logging.h"
    #include "interface/mmal/mmal_buffer.h"
    #include "interface/mmal/util/mmal_util.h"
    #include "interface/mmal/util/mmal_util_params.h"
    #include "interface/mmal/util/mmal_default_components.h"
    #include "interface/mmal/util/mmal_connection.h"
}

#include "RaspiCamControl.h"

#define _QUOTE(str) #str
#define QUOTE(str) _QUOTE(str)
//#define MMAL_CALL(x) x, LOG_INFO("MMAL: {}", QUOTE(x))
#define MMAL_CALL(x) x

// Standard port setting for the camera component
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

#endif

namespace silk
{
namespace node
{


#if defined RASPBERRY_PI

typedef std::shared_ptr<MMAL_COMPONENT_T> Component_ptr;
typedef std::shared_ptr<MMAL_CONNECTION_T> Connection_ptr;
typedef std::shared_ptr<MMAL_POOL_T> Pool_ptr;

////////////////////////////////////////////////////////////////////
/// \brief The Video_Server::Camera_Private_Data struct
/// connection:
/// - camera output
///     - recording encoder
///     - high resizer
///         - high encoder
///         - low resizer
///             - low encoder

struct Raspicam::Impl
{
    std::mutex mutex;

    //camera
    Component_ptr camera;
    Component_ptr camera_splitter;
    Connection_ptr camera_splitter_connection;

    struct Encoder_Data
    {
        sz::Raspicam::Quality* quality = nullptr;

        Component_ptr encoder;
        Connection_ptr encoder_connection;
        Pool_ptr output_pool;
        Component_ptr resizer;
        Connection_ptr resizer_connection;

        std::vector<uint8_t> data;
        std::mutex data_mutex;

        std::atomic<bool> is_active{false};

        q::Clock::time_point start;

        Raspicam::Data_Available_Callback callback;
    };

    Encoder_Data recording;
    Encoder_Data high;
    Encoder_Data low;

    size_t frame_idx = 0;
};

static bool set_connection_enabled(Connection_ptr const& connection, bool yes)
{
//    SCOPED_PINS_GUARD;

    QASSERT(connection);
    if (yes == connection->is_enabled)
    {
        return true;
    }
    if (yes)
    {
        return mmal_connection_enable(connection.get()) == MMAL_SUCCESS;
    }
    else
    {
        return mmal_connection_disable(connection.get()) == MMAL_SUCCESS;
    }
}

#endif

Raspicam::Raspicam(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::Raspicam_Descriptor())
    , m_config(new uav::Raspicam_Config())
{
    QLOG_TOPIC("raspicam");
#if defined RASPBERRY_PI
    m_impl.reset(new Impl);

    m_impl->recording.is_active = false;
    m_impl->high.is_active = false;
    m_impl->low.is_active = false;

    m_impl->recording.callback = std::bind(&Raspicam::recording_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    m_impl->high.callback = std::bind(&Raspicam::streaming_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    m_impl->low.callback = std::bind(&Raspicam::streaming_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
#endif

    m_descriptor->set_fps(30);

    uav::Raspicam_Descriptor::Quality quality;
    quality.set_resolution({ 320, 240 });
    quality.set_bitrate(100000);
    m_descriptor->set_streaming_low(quality);

    quality.set_resolution({ 640, 480 });
    quality.set_bitrate(2000000);
    m_descriptor->set_streaming_high(quality);

    quality.set_resolution({ 1296, 972 });
    quality.set_bitrate(8000000);
    m_descriptor->set_recording(quality);

    m_stream = std::make_shared<Stream>();
}
Raspicam::~Raspicam()
{
    QLOG_TOPIC("~raspicam");

    m_recording_data.should_stop = true;
    if (m_recording_data.thread.joinable())
    {
        m_recording_data.thread.join();
    }

#if defined RASPBERRY_PI
    {
        //first kill the pools to disable the callbacks
        m_impl->low.output_pool.reset();
        m_impl->high.output_pool.reset();
        m_impl->recording.output_pool.reset();
    }

    //wait for the callbacks to die
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::lock_guard<std::mutex> lg(m_impl->mutex);
    m_impl->low.resizer_connection.reset();
    m_impl->low.encoder_connection.reset();
    m_impl->low.resizer.reset();
    m_impl->low.encoder.reset();

    m_impl->high.resizer_connection.reset();
    m_impl->high.encoder_connection.reset();
    m_impl->high.resizer.reset();
    m_impl->high.encoder.reset();

    m_impl->recording.resizer_connection.reset();
    m_impl->recording.encoder_connection.reset();
    m_impl->recording.resizer.reset();
    m_impl->recording.encoder.reset();

    m_impl->camera_splitter_connection.reset();
    m_impl->camera_splitter.reset();
    if (m_impl->camera)
    {
        mmal_port_disable(m_impl->camera->control);
    }
    m_impl->camera.reset();

#endif
}

auto Raspicam::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Video";
    outputs[0].stream = m_stream;
    return outputs;
}

auto Raspicam::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("raspicam::init");

    auto specialized = std::dynamic_pointer_cast<uav::Raspicam_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;
    return init();
}
auto Raspicam::init() -> bool
{
#if defined RASPBERRY_PI
    QLOG_TOPIC("raspicam::init");
    if (m_impl->camera)
    {
        return true;
    }

    m_recording_data.thread = std::thread([this]()
    {
        while (!m_recording_data.should_stop)
        {
            {
                std::lock_guard<std::mutex> lg(m_recording_data.mutex);
                if (!m_recording_data.data_in.empty())
                {
                    m_recording_data.data_out = std::move(m_recording_data.data_in);
                }
            }

            if (!m_recording_data.data_out.empty())
            {
                std::shared_ptr<q::data::File_Sink> sink = m_recording_data.file_sink;
                if (sink != nullptr && sink->is_open())
                {
                    sink->write(m_recording_data.data_out.data(), m_recording_data.data_out.size());
                    sink->flush();
                }
                m_recording_data.data_out.clear();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

#if defined RASPBERRY_PI
    {
        int policy = SCHED_IDLE;
        struct sched_param param;
        param.sched_priority = sched_get_priority_min(policy);
        if (pthread_setschedparam(m_recording_data.thread.native_handle(), policy, &param) != 0)
        {
            perror("recording thread sched_setscheduler");
            exit(EXIT_FAILURE);
        }
    }
#endif


    m_descriptor->fps = math::clamp<size_t>(m_descriptor->fps, 10, 60);

    m_stream->rate = m_descriptor->fps;

    m_impl->recording.quality = &m_descriptor->recording;
    m_impl->high.quality = &m_descriptor->high;
    m_impl->low.quality = &m_descriptor->low;

    if (!create_components())
    {
        return false;
    }

    activate_streams();

    return true;
#else
    QLOGE("Raspicam is only supported on the Raspberry pi.");
    return false;
#endif
}

auto Raspicam::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("raspicam::set_config");

    auto specialized = std::dynamic_pointer_cast<uav::Raspicam_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    activate_streams();

    if (m_config->get_recording())
    {
        start_recording();
    }
    else
    {
        stop_recording();
    }

#if defined RASPBERRY_PI
    m_config->shutter_speed = math::clamp(m_config->shutter_speed, 0.f, 1000.f / m_descriptor->fps);
    raspicamcontrol_set_shutter_speed(m_impl->camera.get(), static_cast<uint32_t>(m_config->shutter_speed * 1000.f));

    raspicamcontrol_set_ISO(m_impl->camera.get(), m_config->iso);

    m_config->ev = math::clamp(m_config->ev, -10, 10);
    raspicamcontrol_set_exposure_compensation(m_impl->camera.get(), m_config->ev);

    m_config->sharpness = math::clamp(m_config->sharpness, 0u, 100u);
    raspicamcontrol_set_sharpness(m_impl->camera.get(), int(m_config->sharpness) * 2 - 100);

    m_config->contrast = math::clamp(m_config->contrast, 0u, 100u);
    raspicamcontrol_set_contrast(m_impl->camera.get(), int(m_config->contrast) * 2 - 100);

    m_config->brightness = math::clamp(m_config->brightness, 0u, 100u);
    raspicamcontrol_set_brightness(m_impl->camera.get(), int(m_config->brightness));

    m_config->saturation = math::clamp(m_config->saturation, 0u, 100u);
    raspicamcontrol_set_saturation(m_impl->camera.get(), int(m_config->saturation) * 2 - 100);

    m_config->awb_mode = math::clamp(m_config->awb_mode, 0u, 8u);
    raspicamcontrol_set_awb_mode(m_impl->camera.get(), static_cast<MMAL_PARAM_AWBMODE_T>(static_cast<uint32_t>(MMAL_PARAM_AWBMODE_AUTO) + m_config->awb_mode));
    //raspicamcontrol_set_awb_gains(m_impl->camera.get(), m_config->awb_rb_gains.x, m_config->awb_rb_gains.y);
#endif

    return true;
}
auto Raspicam::get_config() const -> std::shared_ptr<uav::INode_Config>
{
    return m_config;
}

auto Raspicam::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto Raspicam::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

void Raspicam::shutdown()
{
#if defined RASPBERRY_PI
    QASSERT(m_impl->camera);
#endif
}

auto Raspicam::start(q::Clock::time_point tp) -> bool
{
    return true;
}

void Raspicam::process()
{
    QLOG_TOPIC("raspicam::process");

    std::lock_guard<std::mutex> lg(m_sample_queue.mutex);

    //put the samples back
    for (auto& sample: m_stream->samples)
    {
        m_sample_queue.samples.push_back(std::move(sample));
    }

    m_stream->samples.resize(m_sample_queue.count);
    if (m_sample_queue.count > 0)
    {
        for (size_t i = 0; i < m_sample_queue.count; i++)
        {
            m_stream->samples[i] = std::move(m_sample_queue.samples.front());
            m_sample_queue.samples.pop_front();
        }
        m_sample_queue.count = 0;
    }
}


void Raspicam::recording_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe)
{
    if (!data || size == 0)
    {
        return;
    }

    std::lock_guard<std::mutex> lg(m_recording_data.mutex);
    size_t offset = m_recording_data.data_in.size();
    if (offset < 10 * 1024 * 1024)
    {
        m_recording_data.data_in.resize(offset + size);
        std::copy(data, data + size, m_recording_data.data_in.begin() + offset);
    }
    else
    {
        QLOGW("Recording skipping frames!!! Pending data too big: {}", offset);
    }
}

void Raspicam::streaming_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe)
{
    if (!data || size == 0)
    {
        return;
    }

    std::lock_guard<std::mutex> lg(m_sample_queue.mutex);

    if (m_sample_queue.samples.size() <= m_sample_queue.count)
    {
        m_sample_queue.samples.resize(m_sample_queue.samples.size() + m_sample_queue.count + 1);
    }

    auto& sample = m_sample_queue.samples[m_sample_queue.count];
    sample.is_healthy = true;
    sample.value.type = Stream::Value::Type::H264;
    sample.value.is_keyframe = is_keyframe;
    sample.value.resolution = resolution;
    sample.value.data.resize(size);
    std::copy(data, data + size, sample.value.data.begin());

    m_sample_queue.count++;
}

void Raspicam::activate_streams()
{
#if defined RASPBERRY_PI
    std::lock_guard<std::mutex> lg(m_impl->mutex);

    bool recording = m_recording_data.file_sink != nullptr;
    bool high = m_config->quality == 0;
    bool low = m_config->quality == 1;

    if (m_impl->recording.is_active == recording &&
        m_impl->high.is_active == high &&
        m_impl->low.is_active == low)
    {
        return;
    }

    QLOGI("activating streams recording {}, quality {}", recording, m_config->quality);

    if (set_connection_enabled(m_impl->recording.encoder_connection, recording))
    {
        m_impl->recording.is_active = recording;
    }
    else
    {
        QLOGW("Cannot {} recording bitrate encoder", recording ? "enable" : "disable");
    }

    if (set_connection_enabled(m_impl->high.resizer_connection, high))
    {
        m_impl->high.is_active = high;
    }
    else
    {
        QLOGW("Cannot {} high bitrate encoder", high ? "enable" : "disable");
    }

    if (set_connection_enabled(m_impl->low.resizer_connection, low))
    {
        m_impl->low.is_active = low;
    }
    else
    {
        QLOGW("Cannot {} low bitrate encoder", low ? "enable" : "disable");
    }
#endif
}

void Raspicam::create_file_sink()
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
            m_recording_data.file_sink.reset(new q::data::File_Sink(q::Path(filepath)));
            if (m_recording_data.file_sink->is_open())
            {
                return;
            }
            m_recording_data.file_sink.reset();
        }
        file_idx++;
    } while (file_idx < 16);

    QLOGW("Failed to create capture file.");
    m_recording_data.file_sink.reset();
}


auto Raspicam::start_recording() -> bool
{
#if defined RASPBERRY_PI
    if (!q::util::fs::is_folder(q::Path("capture")) && !q::util::fs::create_folder(q::Path("capture")))
    {
        QLOGW("Cannot create capture folder");
        return false;
    }

    if (!m_recording_data.file_sink)
    {
        create_file_sink();
    }

    activate_streams();
#endif
    return true;
}
void Raspicam::stop_recording()
{
#if defined RASPBERRY_PI
    m_recording_data.file_sink.reset();

    activate_streams();
#endif
}

//void Raspicam::set_stream_quality(comms::Camera_Params::Stream_Quality sq)
//{
//#if defined RASPBERRY_PI
//    m_stream_quality = sq;

//    std::lock_guard<std::mutex> lg(m_impl->mutex);
//    if (m_impl->camera)
//    {
////        set_active_streams(m_file_sink != nullptr,
////                           m_stream_quality == camera_input::Stream_Quality::MEDIUM,
////                           m_stream_quality == camera_input::Stream_Quality::LOW);
//    }
//#endif
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined RASPBERRY_PI

static void dump_format_info(size_t tabs, MMAL_ES_FORMAT_T* format)
{
//    SCOPED_PINS_GUARD;

    QASSERT(format);
    std::string prefix(tabs, '\t');
    char encoding[5] = {0};
    char encoding_variant[5] = {0};
    char color_space[5] = {0};
    memcpy(encoding, &format->encoding, 4);
    memcpy(encoding_variant, &format->encoding_variant, 4);
    memcpy(color_space, &format->es->video.color_space, 4);

    QLOGI("{}type: {}", prefix, format->type);
    QLOGI("{}encoding: {} / variant: {}", prefix, encoding, encoding_variant);
    QLOGI("{}bitrate: {}", prefix, format->bitrate);
    QLOGI("{}flags: {}", prefix, format->flags);
    QLOGI("{}extradata_size: {}", prefix, format->extradata_size);
    QLOGI("{}width: {} / height: {}", prefix, format->es->video.width, format->es->video.height);
    QLOGI("{}crop (x: {}, y: {}, w: {}, h: {})", prefix, format->es->video.crop.x, format->es->video.crop.y, format->es->video.crop.width, format->es->video.crop.height);
    QLOGI("{}frame_rate: num: {} / den: {}", prefix, format->es->video.frame_rate.num, format->es->video.frame_rate.den);
    QLOGI("{}par: num: {} / den: {}", prefix, format->es->video.par.num, format->es->video.par.den);
    QLOGI("{}color_space: {}", prefix, color_space);
}

//static void dump_port_info(size_t tabs, MMAL_PORT_T* port)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(port);
//    std::string prefix(tabs, '\t');
//    QLOGI("{}name: {}", prefix, port->name);
//    QLOGI("{}type: {}", prefix, port->type);
//    QLOGI("{}index: {} / index_all: {}", prefix, port->index, port->index_all);
//    QLOGI("{}is_enabled: {}", prefix, port->is_enabled);
//    QLOGI("{}buffer_num_min: {} / buffer_size_min: {} / buffer_alignment_min: {}",
//               prefix, port->buffer_num_min, port->buffer_size_min, port->buffer_alignment_min);
//    QLOGI("{}buffer_num_recommended: {} / buffer_size_recommended: {}",
//               prefix, port->buffer_num_recommended, port->buffer_size_recommended);
//    QLOGI("{}buffer_num: {} / buffer_size: {}", prefix, port->buffer_num, port->buffer_size);
//    QLOGI("{}capabilities: {}", prefix, port->capabilities);
//    QLOGI("{}format:", prefix);
//    dump_format_info(tabs + 1, port->format);
//}

static Connection_ptr connect_ports(MMAL_PORT_T* output, MMAL_PORT_T* input)
{
    MMAL_CONNECTION_T* connection = nullptr;
    MMAL_STATUS_T status = mmal_connection_create(&connection, output, input, MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
    return (status == MMAL_SUCCESS) ? Connection_ptr(connection, mmal_connection_destroy) : Connection_ptr();
}


static Component_ptr create_component(char const* name, size_t min_input_count, size_t min_output_count)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(name);

    QLOGI("Creating component {}", name);
    MMAL_COMPONENT_T* component = nullptr;
    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_component_create(name, &component));
    if (status != MMAL_SUCCESS)
    {
        QLOGW("\tCannot create component {}: {}", name, status);
        return Component_ptr();
    }
    QLOGI("\t{} has {} inputs and {} outputs:", name, component->input_num, component->output_num);
    if (component->input_num < min_input_count || component->output_num < min_output_count)
    {
        QLOGW("\t{} has an invalid number of inputs and/or outputs: {}<{}, {}<{}", name,
                      component->input_num, min_input_count, component->output_num, min_output_count);
        return Component_ptr();
    }

//    for (size_t i = 0; i < component->input_num; i++)
//    {
//        LOG_INFO("\tinitial input port config {}", i);
//        dump_port_info(2, component->input[i]);
//    }
//    for (size_t i = 0; i < component->output_num; i++)
//    {
//        LOG_INFO("\tinitial output port config {}", i);
//        dump_port_info(2, component->output[i]);
//    }

    return Component_ptr(component, mmal_component_destroy);
}

static bool enable_port(MMAL_PORT_T* port, MMAL_PORT_BH_CB_T cb)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(port);
    QASSERT(cb);

    QLOGI("Enabling port {} from {}. CB: {}", port->name, port->component->name, cb);
    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_port_enable(port, cb));
    if (status != MMAL_SUCCESS)
    {
        QLOGW("Unable to enable port {} from {}: {}", port->name, port->component->name, status);
    }
    return status == MMAL_SUCCESS;
}
static bool enable_component(Component_ptr component)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(component);
    QLOGI("Enabling component {}", component->name);
    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_component_enable(component.get()));
    if (status != MMAL_SUCCESS)
    {
        QLOGW("Unable to enable component {}: {}", component->name, status);
    }
    return status == MMAL_SUCCESS;
}
static void copy_format(MMAL_PORT_T* dst, MMAL_PORT_T* src)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(dst);
    QASSERT(src);

    QLOGI("Copying format from {} to {}:", src->name, dst->name);

    MMAL_CALL(mmal_format_copy(dst->format, src->format));

//    LOG_INFO("src format:");
//    dump_format_info(1, src->format);
//    LOG_INFO("dst format:");
//    dump_format_info(1, dst->format);
}
static bool commit_format(MMAL_PORT_T* port)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(port);

    QLOGI("Trying to set new format to port {}:", port->name);
    //dump_format_info(1, port->format);

    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_port_format_commit(port));
    if (status != MMAL_SUCCESS)
    {
        QLOGW("Unable to commit format on port {}", port->name);
        return false;
    }

    port->buffer_size = math::max(port->buffer_size_recommended, port->buffer_size_min);
    port->buffer_num = math::max(port->buffer_num_recommended, port->buffer_num_min);
//    dump_format_info(1, port->format);

    return true;
}

MMAL_BOOL_T return_buffer_to_pool_callback(MMAL_POOL_T* pool, MMAL_BUFFER_HEADER_T* buffer, void* userdata)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(pool);
    QASSERT(buffer);

    MMAL_PORT_T* port = reinterpret_cast<MMAL_PORT_T*>(userdata);
    QASSERT(port);

    //LOG_INFO("{}: RETURNING buffer {} / port {} size {}k", port->name, buffer, port, buffer->length / 1024.f);

    // and send one back to the port (if still open)
    if (port->is_enabled)
    {
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_send_buffer(port, buffer));
        if (status != MMAL_SUCCESS)
        {
            QLOGW("Unable to return a buffer to the encoder port: {}", status);
        }
    }
    else
    {
        QLOGW("Port is disabled");
    }
    return MMAL_TRUE;
}

static Pool_ptr create_output_port_pool(MMAL_PORT_T* port, MMAL_PORT_USERDATA_T* userdata, MMAL_PORT_BH_CB_T cb, uint32_t buffer_count)
{
    //    SCOPED_PINS_GUARD;;

    //setup video port buffer and a pool to hold them
    port->buffer_size = math::max(port->buffer_size_recommended, port->buffer_size_min);
    port->buffer_num = math::max(port->buffer_num_recommended, port->buffer_num_min);
    port->buffer_num = math::max(port->buffer_num, buffer_count);

    QLOGI("Creating port {} pool with {} buffers of size {}", port->name, port->buffer_num, port->buffer_size);

    MMAL_POOL_T* p = nullptr;
    p = MMAL_CALL(mmal_port_pool_create(port, port->buffer_num, port->buffer_size));
    Pool_ptr o_pool(p, [=](MMAL_POOL_T* pool) { mmal_port_disable(port); mmal_port_pool_destroy(port, pool); } );
    if (!o_pool)
    {
        return Pool_ptr();
    }

    port->userdata = userdata;
    if (!enable_port(port, cb))
    {
        return Pool_ptr();
    }

    mmal_pool_callback_set(o_pool.get(), return_buffer_to_pool_callback, port);

    //send all the buffers in our pool to the video port ready for use
    {
        int num = 0;
        num = MMAL_CALL(mmal_queue_length(o_pool->queue));
        QLOGI("enabling {} buffers for {} port {}", num, port->component->name, port->name);
        for (int i = 0; i < num; i++)
        {
            MMAL_BUFFER_HEADER_T* buffer = nullptr;
            buffer = MMAL_CALL(mmal_queue_get(o_pool->queue));
            if (!buffer)
            {
                QLOGW("Unable to get a required buffer {} from pool queue", i);
                return Pool_ptr();
            }
            if (mmal_port_send_buffer(port, buffer) != MMAL_SUCCESS)
            {
                QLOGW("Unable to send a buffer to encoder output port ({})", i);
                return Pool_ptr();
            }
        }
    }

    return o_pool;
}

static bool setup_encoder_component(Component_ptr encoder, MMAL_PORT_T* src, math::vec2u32 const& resolution, size_t bitrate)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(encoder);
    QASSERT(bitrate > 0);

    auto* input = encoder->input[0];
    QASSERT(input);

    copy_format(input, src);
    input->format->es->video.width = resolution.x;//VCOS_ALIGN_UP(resolution.x, 32);
    input->format->es->video.height = resolution.y;//VCOS_ALIGN_UP(resolution.y, 16);
    input->format->es->video.crop.x = 0;
    input->format->es->video.crop.y = 0;
    input->format->es->video.crop.width = resolution.x;
    input->format->es->video.crop.height = resolution.y;

    if (!commit_format(input))
    {
        return false;
    }

    auto* output = encoder->output[0];
    QASSERT(output);

    // We want same format on input and output
    copy_format(output, input);
    //MMAL_CALL(mmal_format_copy(output->format, input->format));

    // Only supporting H264 at the moment
    output->format->encoding = MMAL_ENCODING_H264;
    output->format->bitrate = bitrate;

    output->format->es->video.width = resolution.x;//VCOS_ALIGN_UP(resolution.x, 32);
    output->format->es->video.height = resolution.y;//VCOS_ALIGN_UP(resolution.y, 16);
    output->format->es->video.crop.x = 0;
    output->format->es->video.crop.y = 0;
    output->format->es->video.crop.width = resolution.x;
    output->format->es->video.crop.height = resolution.y;

    // We need to set the frame rate on output to 0, to ensure it gets
    // updated correctly from the input framerate when port connected
    output->format->es->video.frame_rate.num = 0;
    output->format->es->video.frame_rate.den = 1;

    output->format->es->video.par.num = 1;
    output->format->es->video.par.den = 1;

    // Commit the port changes to the output port
    QLOGI("setting format for encoder component {}", encoder->name);
    bool res = commit_format(output);

    //output->buffer_num = 3;

    return res;
}

static Component_ptr create_encoder_component_for_recording(MMAL_PORT_T* src, math::vec2u32 const& resolution, size_t bitrate)
{
    //    SCOPED_PINS_GUARD;;

    Component_ptr encoder = create_component(MMAL_COMPONENT_DEFAULT_VIDEO_ENCODER, 1, 1);
    if (!encoder)
    {
        return Component_ptr();
    }
    if (!setup_encoder_component(encoder, src, resolution, bitrate))
    {
        return Component_ptr();
    }

    //auto input = encoder->input[0];

    {
        MMAL_PARAMETER_VIDEO_PROFILE_T  param;
        param.hdr.id = MMAL_PARAMETER_PROFILE;
        param.hdr.size = sizeof(param);

        param.profile[0].profile = MMAL_VIDEO_PROFILE_H264_HIGH;
        param.profile[0].level = MMAL_VIDEO_LEVEL_H264_4; // This is the only value supported

        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_parameter_set(encoder->output[0], &param.hdr));
        if (status != MMAL_SUCCESS)
        {
            QLOGW("Failed to set H264 profile");
            return Component_ptr();
        }
    }

    if (mmal_port_parameter_set_boolean(encoder->input[0], MMAL_PARAMETER_VIDEO_IMMUTABLE_INPUT, 1) != MMAL_SUCCESS)
    {
        QLOGW("failed to set IMMUTABLE INPUT parameters");
        return Component_ptr();
    }
    //set INLINE HEADER flag to generate SPS and PPS for every IDR if requested
    if (mmal_port_parameter_set_boolean(encoder->output[0], MMAL_PARAMETER_VIDEO_ENCODE_INLINE_HEADER, 1) != MMAL_SUCCESS)
    {
        QLOGW("failed to set INLINE HEADER FLAG parameters");
        return Component_ptr();
    }


    if (!enable_component(encoder))
    {
        return Component_ptr();
    }

//    if (!enable_port(input, input_callback))
//    {
//        return Component_ptr();
//    }

    return encoder;
}

static Component_ptr create_encoder_component_for_streaming(MMAL_PORT_T* src, math::vec2u32 const& resolution, size_t bitrate)
{
    //    SCOPED_PINS_GUARD;;

    Component_ptr encoder = create_component(MMAL_COMPONENT_DEFAULT_VIDEO_ENCODER, 1, 1);
    if (!encoder)
    {
        return Component_ptr();
    }
    if (!setup_encoder_component(encoder, src, resolution, bitrate))
    {
        return Component_ptr();
    }

    auto input = encoder->input[0];
    auto output = encoder->output[0];

    {
        MMAL_PARAMETER_VIDEO_PROFILE_T  param;
        param.hdr.id = MMAL_PARAMETER_PROFILE;
        param.hdr.size = sizeof(param);

        param.profile[0].profile = MMAL_VIDEO_PROFILE_H264_HIGH;
        param.profile[0].level = MMAL_VIDEO_LEVEL_H264_4; // This is the only value supported

        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_parameter_set(output, &param.hdr));
        if (status != MMAL_SUCCESS)
        {
            QLOGW("Failed to set H264 profile");
            return Component_ptr();
        }
    }

    if (mmal_port_parameter_set_boolean(input, MMAL_PARAMETER_VIDEO_IMMUTABLE_INPUT, 1) != MMAL_SUCCESS)
    {
        QLOGW("failed to set IMMUTABLE INPUT parameters");
        return Component_ptr();
    }
    //set INLINE HEADER flag to generate SPS and PPS for every IDR if requested
    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_INLINE_HEADER, 1) != MMAL_SUCCESS)
    {
        QLOGW("failed to set INLINE HEADER FLAG parameters");
        return Component_ptr();
    }

    //fails with I420
//    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_H264_LOW_LATENCY, 1) != MMAL_SUCCESS)
//    {
//        LOG_WARNING("failed to set MMAL_PARAMETER_VIDEO_ENCODE_H264_LOW_LATENCY");
//        return Component_ptr();
//    }
    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_H264_AU_DELIMITERS, 1) != MMAL_SUCCESS)
    {
        QLOGW("failed to set MMAL_PARAMETER_VIDEO_ENCODE_H264_AU_DELIMITERS");
        return Component_ptr();
    }
//    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_H264_VCL_HRD_PARAMETERS, 1) != MMAL_SUCCESS)
//    {
//        QLOGW("failed to set MMAL_PARAMETER_VIDEO_ENCODE_H264_VCL_HRD_PARAMETERS");
//        return Component_ptr();
//    }
//    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_SEI_ENABLE, 1) != MMAL_SUCCESS)
//    {
//        QLOGW("failed to set MMAL_PARAMETER_VIDEO_ENCODE_SEI_ENABLE");
//        return Component_ptr();
//    }

    {
        MMAL_PARAMETER_UINT32_T param = {{ MMAL_PARAMETER_INTRAPERIOD, sizeof(param)}, 30};
        if (mmal_port_parameter_set(output, &param.hdr) != MMAL_SUCCESS)
        {
            QLOGW("failed to set MMAL_PARAMETER_INTRAPERIOD");
            return Component_ptr();
        }
    }

//    {
//        MMAL_PARAMETER_VIDEO_RATECONTROL_T param = {{ MMAL_PARAMETER_RATECONTROL, sizeof(param)}, MMAL_VIDEO_RATECONTROL_VARIABLE_SKIP_FRAMES};
//        if (mmal_port_parameter_set(output, &param.hdr) != MMAL_SUCCESS)
//        {
//            QLOGW("failed to set MMAL_PARAMETER_RATECONTROL");
//            return Component_ptr();
//        }
//    }

//    {
//        MMAL_PARAMETER_VIDEO_ENCODE_RC_MODEL_T param = {{ MMAL_PARAMETER_VIDEO_ENCODE_RC_MODEL, sizeof(param)}, MMAL_VIDEO_ENCODER_RC_MODEL_VOWIFI};
//        if (mmal_port_parameter_set(output, &param.hdr) != MMAL_SUCCESS)
//        {
//            QLOGW("failed to set MMAL_PARAMETER_VIDEO_ENCODE_RC_MODEL");
//            return Component_ptr();
//        }
//    }

    {
        MMAL_PARAMETER_VIDEO_INTRA_REFRESH_T  param;
        param.hdr.id = MMAL_PARAMETER_VIDEO_INTRA_REFRESH;
        param.hdr.size = sizeof(param);

        // Get first so we don't overwrite anything unexpectedly
        if (mmal_port_parameter_get(output, &param.hdr) != MMAL_SUCCESS)
        {
            QLOGW("failed to get INTRA REFRESH HEADER FLAG parameters");
            return Component_ptr();
        }

        param.refresh_mode = MMAL_VIDEO_INTRA_REFRESH_CYCLIC;

        if (mmal_port_parameter_set(output, &param.hdr) != MMAL_SUCCESS)
        {
            QLOGW("failed to set INTRA REFRESH HEADER FLAG parameters");
            return Component_ptr();
        }
    }

    if (!enable_component(encoder))
    {
        return Component_ptr();
    }

//    if (!enable_port(input, input_callback))
//    {
//        return Component_ptr();
//    }

    return encoder;
}

static void setup_video_format(MMAL_ES_FORMAT_T* format, math::vec2u32 const& resolution, bool align, size_t fps)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(format);
    QASSERT(resolution.x > 0 && resolution.y > 0);
    QASSERT(fps > 0);

    align = false;

    format->es->video.width = align ? VCOS_ALIGN_UP(resolution.x, 32) : resolution.x;
    format->es->video.height = align ? VCOS_ALIGN_UP(resolution.y, 16) : resolution.y;
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = resolution.x;
    format->es->video.crop.height = resolution.y;
    format->es->video.frame_rate.num = fps;
    format->es->video.frame_rate.den = 1;
    format->es->video.par.num = 1;
    format->es->video.par.den = 1;
}

static Component_ptr create_splitter_component(MMAL_PORT_T* src)
{
    //    SCOPED_PINS_GUARD;;

    Component_ptr splitter = create_component(MMAL_COMPONENT_DEFAULT_SPLITTER, 1, 3);
    if (!splitter)
    {
        return Component_ptr();
    }

    copy_format(splitter->input[0], src);
    if (!commit_format(splitter->input[0]))
    {
        return Component_ptr();
    }

//    for (size_t i = 0; i < splitter->output_num; i++)
//    {
//        copy_format(splitter->output[i], src);
//        if (!commit_format(splitter->output[i]))
//        {
//            return Component_ptr();
//        }
//    }

    if (!enable_component(splitter))
    {
        return Component_ptr();
    }

    return splitter;
}

static Component_ptr create_resizer_component(MMAL_PORT_T* src, math::vec2u32 const& resolution, size_t fps)
{
    //    SCOPED_PINS_GUARD;;

    Component_ptr resizer = create_component("vc.ril.resize", 1, 1);
    if (!resizer)
    {
        return Component_ptr();
    }

    copy_format(resizer->input[0], src);
    if (!commit_format(resizer->input[0]))
    {
        return Component_ptr();
    }

    copy_format(resizer->output[0], src);
    setup_video_format(resizer->output[0]->format, resolution, false, fps);

    auto* format = resizer->output[0]->format;
    format->es->video.width = resolution.x;
    format->es->video.height = resolution.y;
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = resolution.x;
    format->es->video.crop.height = resolution.y;
    format->es->video.frame_rate.num = 0;
    format->es->video.frame_rate.den = 1;
    format->es->video.par.num = 1;
    format->es->video.par.den = 1;
    if (!commit_format(resizer->output[0]))
    {
        return Component_ptr();
    }

    if (!enable_component(resizer))
    {
        return Component_ptr();
    }

//    if (!enable_port(resizer->input[0], input_callback))
//    {
//        return Component_ptr();
//    }
//    dump_port_info(1, resizer->input[0]);
//    dump_port_info(2, resizer->output[0]);

    return resizer;
}


///////////////////////////////////////////////////////////////////////////////////////

static void camera_control_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(port && buffer);
    //GCamera->OnCameraControlCallback(port, buffer);
    QLOGI("Camera control callback called: {}, {}", port, buffer);
    mmal_buffer_header_release(buffer);
}

static void encoder_buffer_callback_fn(Raspicam::Impl& impl,
                                       Raspicam::Impl::Encoder_Data& encoder_data,
                                       MMAL_PORT_T* /*port*/,
                                       MMAL_BUFFER_HEADER_T* buffer)
{
    if (!encoder_data.callback)
    {
        MMAL_CALL(mmal_buffer_header_release(buffer));
        return;
    }

    auto mmal_flags = buffer->flags;
    bool is_end_frame = (mmal_flags == MMAL_BUFFER_HEADER_FLAG_FRAME_END || mmal_flags == (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_KEYFRAME));
    bool is_keyframe = (mmal_flags & MMAL_BUFFER_HEADER_FLAG_KEYFRAME) != 0;

    impl.frame_idx++;

    bool sent = false;

    MMAL_CALL(mmal_buffer_header_mem_lock(buffer));
    {
        size_t off = encoder_data.data.size();
        if (off == 0 && is_end_frame) //complete frame and the data is empty - send the buffer directly to avoid a copy
        {
            if (encoder_data.is_active)
            {
                encoder_data.callback(buffer->data, buffer->length, encoder_data.quality->resolution, is_keyframe);
            }
            sent = true;
        }
        else
        {
            std::lock_guard<std::mutex> lg(encoder_data.data_mutex);
            encoder_data.data.resize(encoder_data.data.size() + buffer->length);
            std::copy(buffer->data, buffer->data + buffer->length, encoder_data.data.data() + off);
        }
    }
    MMAL_CALL(mmal_buffer_header_mem_unlock(buffer));

    // release buffer back to the pool
    MMAL_CALL(mmal_buffer_header_release(buffer));

    if (!sent && is_end_frame)
    {
        std::lock_guard<std::mutex> lg(encoder_data.data_mutex);
        if (encoder_data.is_active)
        {
            encoder_data.callback(encoder_data.data.data(), encoder_data.data.size(), encoder_data.quality->resolution, is_keyframe);
        }
        encoder_data.data.clear();
    }

//    if (is_end_frame)
//    {
//        size_t ms = (q::Clock::now() - encoder_data.start).count() / 1000;
//        LOG_INFO("ENCODER {}: {}", encoder_data.resolution, ms);
//        encoder_data.start = q::Clock::now();
//    }
}

static void recording_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    //    SCOPED_PINS_GUARD;;

    QASSERT(port && buffer);
    Raspicam::Impl* impl = reinterpret_cast<Raspicam::Impl*>(port->userdata);
    QASSERT(impl);
    std::lock_guard<std::mutex> lg(impl->mutex);

    encoder_buffer_callback_fn(*impl, impl->recording, port, buffer);
}

static void high_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    QASSERT(port && buffer);
    Raspicam::Impl* impl = reinterpret_cast<Raspicam::Impl*>(port->userdata);
    QASSERT(impl);
    std::lock_guard<std::mutex> lg(impl->mutex);

    encoder_buffer_callback_fn(*impl, impl->high, port, buffer);
}

static void low_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    QASSERT(port && buffer);
    Raspicam::Impl* impl = reinterpret_cast<Raspicam::Impl*>(port->userdata);
    QASSERT(impl);
    std::lock_guard<std::mutex> lg(impl->mutex);

    encoder_buffer_callback_fn(*impl, impl->low, port, buffer);
}

#endif

///////////////////////////////////////////////////////////////////////////////////////

auto Raspicam::create_components() -> bool
{
#if defined RASPBERRY_PI
    //    SCOPED_PINS_GUARD;;

    m_impl->camera = create_component(MMAL_COMPONENT_DEFAULT_CAMERA, 0, 3);
    if (!m_impl->camera)
    {
        return false;
    }

    // Enable the camera, and tell it its control callback function
    if (!enable_port(m_impl->camera->control, camera_control_callback))
    {
        QLOGE("Cannot enable camera control port");
        return false;
    }

    auto camera_video_port = m_impl->camera->output[MMAL_CAMERA_VIDEO_PORT];
    auto camera_preview_port = m_impl->camera->output[MMAL_CAMERA_PREVIEW_PORT];
    auto camera_capture_port = m_impl->camera->output[MMAL_CAMERA_CAPTURE_PORT];

    //  set up the camera configuration
    {
        MMAL_PARAMETER_CAMERA_CONFIG_T cam_config;
        cam_config.hdr.id = MMAL_PARAMETER_CAMERA_CONFIG;
        cam_config.hdr.size = sizeof(cam_config);
        cam_config.max_stills_w = m_descriptor->recording.resolution.x;
        cam_config.max_stills_h = m_descriptor->recording.resolution.y;
        cam_config.stills_yuv422 = 0;
        cam_config.one_shot_stills = 0;
        cam_config.max_preview_video_w = m_descriptor->recording.resolution.x;
        cam_config.max_preview_video_h = m_descriptor->recording.resolution.y;
        cam_config.num_preview_video_frames = 3;
        cam_config.stills_capture_circular_buffer_height = 0;
        cam_config.fast_preview_resume = 0;
        cam_config.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC;
        MMAL_STATUS_T status = MMAL_CALL(mmal_port_parameter_set(m_impl->camera->control, &cam_config.hdr));
        if (status != MMAL_SUCCESS)
        {
            QLOGE("Couldn't set camera config: error {}", status);
            return false;
        }
    }

    {
        // setup preview port format -> used for streaming
        auto* format = camera_preview_port->format;
        format->encoding = MMAL_ENCODING_I420;
        format->encoding_variant = MMAL_ENCODING_VARIANT_DEFAULT;
        setup_video_format(format, m_descriptor->recording.resolution, true, m_descriptor->fps);
        MMAL_STATUS_T status = MMAL_CALL(mmal_port_format_commit(camera_preview_port));
        if (status != MMAL_SUCCESS)
        {
            QLOGE("Couldn't set preview port format : error {}", status);
            return false;
        }
    }

    {
        //setup video port format -> used for recording
        auto* format = camera_video_port->format;
        format->encoding = MMAL_ENCODING_OPAQUE;
        format->encoding_variant = MMAL_ENCODING_VARIANT_DEFAULT;
        setup_video_format(format, m_descriptor->recording.resolution, true, m_descriptor->fps);
        MMAL_STATUS_T status = MMAL_CALL(mmal_port_format_commit(camera_video_port));
        if (status != MMAL_SUCCESS)
        {
            QLOGE("Couldn't set video port format : error {}", status);
            return false;
        }
    }

    {
        //setup still port format
        auto* format = camera_capture_port->format;
        format->encoding = MMAL_ENCODING_I420;
        format->encoding_variant = MMAL_ENCODING_VARIANT_DEFAULT;
        format->es->video.width = VCOS_ALIGN_UP(m_descriptor->recording.resolution.x, 32);
        format->es->video.height = VCOS_ALIGN_UP(m_descriptor->recording.resolution.y, 16);
        format->es->video.crop.x = 0;
        format->es->video.crop.y = 0;
        format->es->video.crop.width = m_descriptor->recording.resolution.x;
        format->es->video.crop.height = m_descriptor->recording.resolution.y;
        format->es->video.frame_rate.num = 0;
        format->es->video.frame_rate.den = 1;
        format->es->video.par.num = 1;
        format->es->video.par.den = 1;
        MMAL_STATUS_T status = MMAL_CALL(mmal_port_format_commit(camera_capture_port));
        if (status != MMAL_SUCCESS)
        {
            QLOGE("Couldn't set still port format : error {}", status);
            return false;
        }
    }

    {
        //setup use case
        MMAL_PARAMETER_CAMERA_USE_CASE_T param;
        param.hdr.id = MMAL_PARAMETER_CAMERA_USE_CASE;
        param.hdr.size = sizeof(param);
        param.use_case = MMAL_PARAM_CAMERA_USE_CASE_VIDEO_CAPTURE;
        MMAL_STATUS_T status = MMAL_CALL(mmal_port_parameter_set(m_impl->camera->control, &param.hdr));
        if (status != MMAL_SUCCESS)
        {
            QLOGE("Couldn't set camera config: error {}", status);
            return false;
        }
    }


    static uint32_t buffer_count = 3;
    auto userdata = reinterpret_cast<MMAL_PORT_USERDATA_T*>(m_impl.get());

    //enable the camera
    if (!enable_component(m_impl->camera))
    {
        QLOGE("Cannot enable camera component");
        return false;
    }

    m_impl->camera_splitter = create_splitter_component(camera_preview_port);
    if (!m_impl->camera_splitter)
    {
        QLOGE("Cannot create camera splitter component");
        return false;
    }

    m_impl->camera_splitter_connection = connect_ports(camera_preview_port, m_impl->camera_splitter->input[0]);
    if (!m_impl->camera_splitter_connection || !set_connection_enabled(m_impl->camera_splitter_connection, true))
    {
        QLOGE("Cannot enable camera splitter");
        return false;
    }


    //recording
    auto& recording = m_impl->recording;
    recording.encoder = create_encoder_component_for_recording(camera_video_port, m_descriptor->recording.resolution, m_descriptor->recording.bitrate);
    recording.encoder_connection = connect_ports(camera_video_port, recording.encoder->input[0]);
    if (!recording.encoder_connection || !set_connection_enabled(recording.encoder_connection, true))
    {
        QLOGE("Cannot create recording bitrate encoder");
        return false;
    }

    recording.output_pool = create_output_port_pool(recording.encoder->output[0], userdata, recording_encoder_buffer_callback, buffer_count);
    if (!recording.output_pool)
    {
        QLOGE("Cannot create recording bitrate encoder port");
        return false;
    }

    //high
    auto& high = m_impl->high;
    high.resizer = create_resizer_component(m_impl->camera_splitter->output[1], m_descriptor->high.resolution, m_descriptor->fps);
    high.resizer_connection = connect_ports(m_impl->camera_splitter->output[1], high.resizer->input[0]);
    if (!high.resizer_connection)
    {
        QLOGE("Cannot create high bitrate resizer");
        return false;
    }
    high.encoder = create_encoder_component_for_streaming(high.resizer->output[0], m_descriptor->high.resolution, m_descriptor->high.bitrate);
    high.encoder_connection = connect_ports(high.resizer->output[0], high.encoder->input[0]);
    if (!high.encoder_connection || !set_connection_enabled(high.encoder_connection, true))
    {
        QLOGE("Cannot create high bitrate encoder");
        return false;
    }
    high.output_pool = create_output_port_pool(high.encoder->output[0], userdata, high_encoder_buffer_callback, buffer_count);
    if (!high.output_pool)
    {
        QLOGE("Cannot create high bitrate encoder port");
        return false;
    }

    //low
    auto& low = m_impl->low;
    low.resizer = create_resizer_component(m_impl->camera_splitter->output[2], m_descriptor->low.resolution, m_descriptor->fps);
    low.resizer_connection = connect_ports(m_impl->camera_splitter->output[2], low.resizer->input[0]);
    if (!low.resizer_connection)
    {
        QLOGE("Cannot create low bitrate resizer");
        return false;
    }
    low.encoder = create_encoder_component_for_streaming(low.resizer->output[0], m_descriptor->low.resolution, m_descriptor->low.bitrate);
    low.encoder_connection = connect_ports(low.resizer->output[0], low.encoder->input[0]);
    if (!low.encoder_connection || !set_connection_enabled(low.encoder_connection, true))
    {
        QLOGE("Cannot create low bitrate encoder");
        return false;
    }
    low.output_pool = create_output_port_pool(low.encoder->output[0], userdata, low_encoder_buffer_callback, buffer_count);
    if (!low.output_pool)
    {
        QLOGE("Cannot create low bitrate encoder port");
        return false;
    }


    {
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1));
        if (status != MMAL_SUCCESS)
        {
            QLOGE("failed to start capturing");
            return false;
        }
    }
#endif
    return true;
}



}
}

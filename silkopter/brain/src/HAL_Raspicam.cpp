#include "BrainStdAfx.h"
#include "HAL_Raspicam.h"
#include "common/input/Camera_Input.h"

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

#define _QUOTE(str) #str
#define QUOTE(str) _QUOTE(str)
//#define MMAL_CALL(x) x, SILK_INFO("MMAL: {}", QUOTE(x))
#define MMAL_CALL(x) x

// Standard port setting for the camera component
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

using namespace silk;

typedef std::shared_ptr<MMAL_COMPONENT_T> Component_ptr;
typedef std::shared_ptr<MMAL_CONNECTION_T> Connection_ptr;
typedef std::shared_ptr<MMAL_POOL_T> Pool_ptr;

//////////////////////////////////////////////////////////////////////////
/// \brief The Video_Server::Camera_Private_Data struct
/// connection:
/// - camera output
///     - high encoder
///     - medium resizer
///         - medium encoder
///         - low resizer
///             - low encoder

struct HAL_Raspicam::Impl
{
    //camera
    Component_ptr camera;
    Component_ptr camera_splitter;
    Connection_ptr camera_splitter_connection;

    struct Encoder_Data
    {
        Component_ptr encoder;
        Connection_ptr encoder_connection;
        Pool_ptr output_pool;
        Component_ptr resizer;
        Connection_ptr resizer_connection;

        std::vector<uint8_t> data;
        std::mutex data_mutex;

        std::atomic<bool> is_active{false};
        HAL_Raspicam::Quality quality;

        q::Clock::time_point start;
    };

    Encoder_Data recording;
    Encoder_Data high;
    Encoder_Data medium;
    Encoder_Data low;

    HAL_Raspicam::Data_Available_Callback file_callback;
    HAL_Raspicam::Data_Available_Callback stream_callback;

    size_t frame_idx = 0;
};

static bool set_connection_enabled(Connection_ptr const& connection, bool yes)
{
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

HAL_Raspicam::HAL_Raspicam()
{
    m_impl.reset(new Impl);

    m_impl->recording.is_active = false;
    m_impl->high.is_active = false;
    m_impl->medium.is_active = false;
    m_impl->low.is_active = false;

    m_fps = 30;
    m_impl->recording.quality.resolution.set(1280, 960);
    m_impl->recording.quality.bitrate = 16000000;
    m_impl->high.quality.resolution.set(1280, 960);
    m_impl->high.quality.bitrate = 4000000;
    m_impl->medium.quality.resolution.set(640, 480);
    m_impl->medium.quality.bitrate = 2000000;
    m_impl->low.quality.resolution.set(320, 240);
    m_impl->low.quality.bitrate = 160000;


    m_impl->file_callback = std::bind(&HAL_Raspicam::file_callback, this, std::placeholders::_1, std::placeholders::_2);
}
HAL_Raspicam::~HAL_Raspicam()
{
}

auto HAL_Raspicam::init() -> bool
{
    if (m_impl->camera)
    {
        return true;
    }

    auto res = create_components();
    if (res)
    {
        set_active_streams(false,
                           true,
                           false);
    }
    return res;
}

void HAL_Raspicam::process()
{

}


void HAL_Raspicam::file_callback(uint8_t const* data, size_t size)
{
    QASSERT(data && size);
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

void HAL_Raspicam::set_data_callback(Data_Available_Callback cb)
{
    m_impl->stream_callback = cb;
}

void HAL_Raspicam::set_active_streams(bool high, bool medium, bool low)
{
    if (m_impl->high.is_active == high &&
        m_impl->medium.is_active == medium &&
        m_impl->low.is_active == low)
    {
        return;
    }

    SILK_INFO("activating streams high {}, medium {}, low {}", high, medium, low);

    if (set_connection_enabled(m_impl->high.encoder_connection, high))
    {
        m_impl->high.is_active = high;
    }
    else
    {
        SILK_WARNING("Cannot {} high bitrate encoder", high ? "enable" : "disable");
    }

    if (set_connection_enabled(m_impl->medium.resizer_connection, medium))
    {
        m_impl->medium.is_active = medium;
    }
    else
    {
        SILK_WARNING("Cannot {} medium bitrate encoder", medium ? "enable" : "disable");
    }

    if (set_connection_enabled(m_impl->low.resizer_connection, low))
    {
        m_impl->low.is_active = low;
    }
    else
    {
        SILK_WARNING("Cannot {} low bitrate encoder", low ? "enable" : "disable");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


static void dump_format_info(size_t tabs, MMAL_ES_FORMAT_T* format)
{
    QASSERT(format);
    std::string prefix(tabs, '\t');
    char encoding[5] = {0};
    char encoding_variant[5] = {0};
    char color_space[5] = {0};
    memcpy(encoding, &format->encoding, 4);
    memcpy(encoding_variant, &format->encoding_variant, 4);
    memcpy(color_space, &format->es->video.color_space, 4);

    SILK_INFO("{}type: {}", prefix, format->type);
    SILK_INFO("{}encoding: {} / variant: {}", prefix, encoding, encoding_variant);
    SILK_INFO("{}bitrate: {}", prefix, format->bitrate);
    SILK_INFO("{}flags: {}", prefix, format->flags);
    SILK_INFO("{}extradata_size: {}", prefix, format->extradata_size);
    SILK_INFO("{}width: {} / height: {}", prefix, format->es->video.width, format->es->video.height);
    SILK_INFO("{}crop (x: {}, y: {}, w: {}, h: {})", prefix, format->es->video.crop.x, format->es->video.crop.y, format->es->video.crop.width, format->es->video.crop.height);
    SILK_INFO("{}frame_rate: num: {} / den: {}", prefix, format->es->video.frame_rate.num, format->es->video.frame_rate.den);
    SILK_INFO("{}par: num: {} / den: {}", prefix, format->es->video.par.num, format->es->video.par.den);
    SILK_INFO("{}color_space: {}", prefix, color_space);
}

static void dump_port_info(size_t tabs, MMAL_PORT_T* port)
{
    QASSERT(port);
    std::string prefix(tabs, '\t');
    SILK_INFO("{}name: {}", prefix, port->name);
    SILK_INFO("{}type: {}", prefix, port->type);
    SILK_INFO("{}index: {} / index_all: {}", prefix, port->index, port->index_all);
    SILK_INFO("{}is_enabled: {}", prefix, port->is_enabled);
    SILK_INFO("{}buffer_num_min: {} / buffer_size_min: {} / buffer_alignment_min: {}",
               prefix, port->buffer_num_min, port->buffer_size_min, port->buffer_alignment_min);
    SILK_INFO("{}buffer_num_recommended: {} / buffer_size_recommended: {}",
               prefix, port->buffer_num_recommended, port->buffer_size_recommended);
    SILK_INFO("{}buffer_num: {} / buffer_size: {}", prefix, port->buffer_num, port->buffer_size);
    SILK_INFO("{}capabilities: {}", prefix, port->capabilities);
    SILK_INFO("{}format:", prefix);
    dump_format_info(tabs + 1, port->format);
}

static Connection_ptr connect_ports(MMAL_PORT_T* output, MMAL_PORT_T* input)
{
    MMAL_CONNECTION_T* connection = nullptr;
    MMAL_STATUS_T status = mmal_connection_create(&connection, output, input, MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
    return (status == MMAL_SUCCESS) ? Connection_ptr(connection, mmal_connection_destroy) : Connection_ptr();
}


static Component_ptr create_component(char const* name, size_t min_input_count, size_t min_output_count)
{
    QASSERT(name);

    SILK_INFO("Creating component {}", name);
    MMAL_COMPONENT_T* component = nullptr;
    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_component_create(name, &component));
    if (status != MMAL_SUCCESS)
    {
        SILK_WARNING("\tCannot create component {}: {}", name, status);
        return Component_ptr();
    }
    SILK_INFO("\t{} has {} inputs and {} outputs:", name, component->input_num, component->output_num);
    if (component->input_num < min_input_count || component->output_num < min_output_count)
    {
        SILK_WARNING("\t{} has an invalid number of inputs and/or outputs: {}<{}, {}<{}", name,
                      component->input_num, min_input_count, component->output_num, min_output_count);
        return Component_ptr();
    }

//    for (size_t i = 0; i < component->input_num; i++)
//    {
//        SILK_INFO("\tinitial input port config {}", i);
//        dump_port_info(2, component->input[i]);
//    }
//    for (size_t i = 0; i < component->output_num; i++)
//    {
//        SILK_INFO("\tinitial output port config {}", i);
//        dump_port_info(2, component->output[i]);
//    }

    return Component_ptr(component, mmal_component_destroy);
}

static bool enable_port(MMAL_PORT_T* port, MMAL_PORT_BH_CB_T cb)
{
    QASSERT(port);
    QASSERT(cb);

    SILK_INFO("Enabling port {} from {}. CB: {}", port->name, port->component->name, cb);
    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_port_enable(port, cb));
    if (status != MMAL_SUCCESS)
    {
        SILK_WARNING("Unable to enable port {} from {}: {}", port->name, port->component->name, status);
    }
    return status == MMAL_SUCCESS;
}
static bool enable_component(Component_ptr component)
{
    QASSERT(component);
    SILK_INFO("Enabling component {}", component->name);
    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_component_enable(component.get()));
    if (status != MMAL_SUCCESS)
    {
        SILK_WARNING("Unable to enable component {}: {}", component->name, status);
    }
    return status == MMAL_SUCCESS;
}
static void copy_format(MMAL_PORT_T* dst, MMAL_PORT_T* src)
{
    QASSERT(dst);
    QASSERT(src);

    SILK_INFO("Copying format from {} to {}:", src->name, dst->name);

    MMAL_CALL(mmal_format_copy(dst->format, src->format));

//    SILK_INFO("src format:");
//    dump_format_info(1, src->format);
//    SILK_INFO("dst format:");
//    dump_format_info(1, dst->format);
}
static bool commit_format(MMAL_PORT_T* port)
{
    QASSERT(port);

    SILK_INFO("Trying to set new format to port {}:", port->name);
    //dump_format_info(1, port->format);

    MMAL_STATUS_T status;
    status = MMAL_CALL(mmal_port_format_commit(port));
    if (status != MMAL_SUCCESS)
    {
        SILK_WARNING("Unable to commit format on port {}", port->name);
        return false;
    }

    port->buffer_size = math::max(port->buffer_size_recommended, port->buffer_size_min);
    port->buffer_num = math::max(port->buffer_num_recommended, port->buffer_num_min);
//    dump_format_info(1, port->format);

    return true;
}

MMAL_BOOL_T return_buffer_to_pool_callback(MMAL_POOL_T* pool, MMAL_BUFFER_HEADER_T* buffer, void* userdata)
{
    QASSERT(pool);
    QASSERT(buffer);

    MMAL_PORT_T* port = reinterpret_cast<MMAL_PORT_T*>(userdata);
    QASSERT(port);

    //SILK_INFO("{}: RETURNING buffer {} / port {} size {}k", port->name, buffer, port, buffer->length / 1024.f);

    // and send one back to the port (if still open)
    if (port->is_enabled)
    {
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_send_buffer(port, buffer));
        if (status != MMAL_SUCCESS)
        {
            SILK_WARNING("Unable to return a buffer to the encoder port: {}", status);
        }
    }
    else
    {
        SILK_WARNING("Port is disabled");
    }
    return MMAL_TRUE;
}

static Pool_ptr create_output_port_pool(MMAL_PORT_T* port, MMAL_PORT_USERDATA_T* userdata, MMAL_PORT_BH_CB_T cb, uint32_t buffer_count)
{
    //setup video port buffer and a pool to hold them
    port->buffer_size = math::max(port->buffer_size_recommended, port->buffer_size_min);
    port->buffer_num = math::max(port->buffer_num_recommended, port->buffer_num_min);
    port->buffer_num = math::max(port->buffer_num, buffer_count);

    SILK_INFO("Creating port {} pool with {} buffers of size {}", port->name, port->buffer_num, port->buffer_size);

    MMAL_POOL_T* p = nullptr;
    p = MMAL_CALL(mmal_port_pool_create(port, port->buffer_num, port->buffer_size));
    Pool_ptr o_pool(p, [=](MMAL_POOL_T* pool) { mmal_port_pool_destroy(port, pool); } );
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
        SILK_INFO("enabling {} buffers for {} port {}", num, port->component->name, port->name);
        for (int i = 0; i < num; i++)
        {
            MMAL_BUFFER_HEADER_T* buffer = nullptr;
            buffer = MMAL_CALL(mmal_queue_get(o_pool->queue));
            if (!buffer)
            {
                SILK_WARNING("Unable to get a required buffer {} from pool queue", i);
                return Pool_ptr();
            }
            if (mmal_port_send_buffer(port, buffer) != MMAL_SUCCESS)
            {
                SILK_WARNING("Unable to send a buffer to encoder output port ({})", i);
                return Pool_ptr();
            }
        }
    }

    return o_pool;
}

static bool setup_encoder_component(Component_ptr encoder, MMAL_PORT_T* src, math::vec2u32 const& resolution, size_t bitrate)
{
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
    SILK_INFO("setting format for encoder component {}", encoder->name);
    bool res = commit_format(output);

    //output->buffer_num = 3;

    return res;
}

static Component_ptr create_encoder_component_for_saving(MMAL_PORT_T* src, math::vec2u32 const& resolution, size_t bitrate)
{
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
            SILK_WARNING("Failed to set H264 profile");
            return Component_ptr();
        }
    }

    if (mmal_port_parameter_set_boolean(encoder->input[0], MMAL_PARAMETER_VIDEO_IMMUTABLE_INPUT, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set IMMUTABLE INPUT parameters");
        return Component_ptr();
    }
    //set INLINE HEADER flag to generate SPS and PPS for every IDR if requested
    if (mmal_port_parameter_set_boolean(encoder->output[0], MMAL_PARAMETER_VIDEO_ENCODE_INLINE_HEADER, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set INLINE HEADER FLAG parameters");
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

        param.profile[0].profile = MMAL_VIDEO_PROFILE_H264_BASELINE;//MMAL_VIDEO_PROFILE_H264_HIGH;
        param.profile[0].level = MMAL_VIDEO_LEVEL_H264_4; // This is the only value supported

        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_parameter_set(output, &param.hdr));
        if (status != MMAL_SUCCESS)
        {
            SILK_WARNING("Failed to set H264 profile");
            return Component_ptr();
        }
    }

    if (mmal_port_parameter_set_boolean(input, MMAL_PARAMETER_VIDEO_IMMUTABLE_INPUT, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set IMMUTABLE INPUT parameters");
        return Component_ptr();
    }
    //set INLINE HEADER flag to generate SPS and PPS for every IDR if requested
    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_INLINE_HEADER, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set INLINE HEADER FLAG parameters");
        return Component_ptr();
    }

    //fails with I420
//    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_H264_LOW_LATENCY, 1) != MMAL_SUCCESS)
//    {
//        SILK_WARNING("failed to set MMAL_PARAMETER_VIDEO_ENCODE_H264_LOW_LATENCY");
//        return Component_ptr();
//    }
    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_H264_AU_DELIMITERS, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set MMAL_PARAMETER_VIDEO_ENCODE_H264_AU_DELIMITERS");
        return Component_ptr();
    }
    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_H264_VCL_HRD_PARAMETERS, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set MMAL_PARAMETER_VIDEO_ENCODE_H264_VCL_HRD_PARAMETERS");
        return Component_ptr();
    }
    if (mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_VIDEO_ENCODE_SEI_ENABLE, 1) != MMAL_SUCCESS)
    {
        SILK_WARNING("failed to set MMAL_PARAMETER_VIDEO_ENCODE_SEI_ENABLE");
        return Component_ptr();
    }

    {
        MMAL_PARAMETER_VIDEO_INTRA_REFRESH_T  param;
        param.hdr.id = MMAL_PARAMETER_VIDEO_INTRA_REFRESH;
        param.hdr.size = sizeof(param);

        param.refresh_mode = MMAL_VIDEO_INTRA_REFRESH_CYCLIC;
        param.cir_mbs = 20;
        param.air_mbs = 5;
        param.air_ref = 5;
        param.pir_mbs = 5;

        if (mmal_port_parameter_set(output, &param.hdr) != MMAL_SUCCESS)
        {
            SILK_WARNING("failed to set INTRA REFRESH HEADER FLAG parameters");
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
    QASSERT(port && buffer);
    //GCamera->OnCameraControlCallback(port, buffer);
    SILK_INFO("Camera control callback called: {}, {}", port, buffer);
    mmal_buffer_header_release(buffer);
}

static void encoder_buffer_callback_fn(HAL_Raspicam::Impl& impl,
                                       HAL_Raspicam::Impl::Encoder_Data& encoder_data,
                                       HAL_Raspicam::Data_Available_Callback const& callback,
                                       MMAL_PORT_T* port,
                                       MMAL_BUFFER_HEADER_T* buffer)
{
    if (!callback)
    {
        MMAL_CALL(mmal_buffer_header_release(buffer));
        return;
    }

    auto mmal_flags = buffer->flags;
    bool is_end_frame = (mmal_flags == MMAL_BUFFER_HEADER_FLAG_FRAME_END || mmal_flags == (MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_KEYFRAME));

    impl.frame_idx++;

    bool sent = false;

    {
        MMAL_CALL(mmal_buffer_header_mem_lock(buffer));

        size_t off = encoder_data.data.size();
        if (off == 0 && is_end_frame) //complete frame and the data is empty - send the buffer directly to avoid a copy
        {
            if (encoder_data.is_active)
            {
                callback(buffer->data, buffer->length);
            }
            sent = true;
        }
        else
        {
            std::lock_guard<std::mutex> lg(encoder_data.data_mutex);
            encoder_data.data.resize(encoder_data.data.size() + buffer->length);
            std::copy(buffer->data, buffer->data + buffer->length, encoder_data.data.data() + off);
        }

        MMAL_CALL(mmal_buffer_header_mem_unlock(buffer));
    }

    // release buffer back to the pool
    MMAL_CALL(mmal_buffer_header_release(buffer));

    if (!sent && is_end_frame)
    {
        std::lock_guard<std::mutex> lg(encoder_data.data_mutex);
        if (encoder_data.is_active)
        {
            callback(encoder_data.data.data(), encoder_data.data.size());
        }
        encoder_data.data.clear();
    }

//    if (is_end_frame)
//    {
//        size_t ms = (q::Clock::now() - encoder_data.start).count() / 1000;
//        SILK_INFO("ENCODER {}: {}", encoder_data.resolution, ms);
//        encoder_data.start = q::Clock::now();
//    }
}

static void high_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    QASSERT(port && buffer);
    HAL_Raspicam::Impl* impl = reinterpret_cast<HAL_Raspicam::Impl*>(port->userdata);
    QASSERT(impl);
    encoder_buffer_callback_fn(*impl, impl->high, impl->file_callback, port, buffer);
}

static void medium_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    QASSERT(port && buffer);
    HAL_Raspicam::Impl* impl = reinterpret_cast<HAL_Raspicam::Impl*>(port->userdata);
    QASSERT(impl);
    encoder_buffer_callback_fn(*impl, impl->medium, impl->stream_callback, port, buffer);
}

static void low_encoder_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    QASSERT(port && buffer);
    HAL_Raspicam::Impl* impl = reinterpret_cast<HAL_Raspicam::Impl*>(port->userdata);
    QASSERT(impl);
    encoder_buffer_callback_fn(*impl, impl->low, impl->stream_callback, port, buffer);
}

///////////////////////////////////////////////////////////////////////////////////////

auto HAL_Raspicam::create_components() -> bool
{
    m_impl->camera = create_component(MMAL_COMPONENT_DEFAULT_CAMERA, 0, 3);
    if (!m_impl->camera)
    {
        return false;
    }

    // Enable the camera, and tell it its control callback function
    if (!enable_port(m_impl->camera->control, camera_control_callback))
    {
        SILK_ERR("Cannot enable camera control port");
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
        cam_config.max_stills_w = m_impl->high.quality.resolution.x;
        cam_config.max_stills_h = m_impl->high.quality.resolution.y;
        cam_config.stills_yuv422 = 0;
        cam_config.one_shot_stills = 0;
        cam_config.max_preview_video_w = m_impl->high.quality.resolution.x;
        cam_config.max_preview_video_h = m_impl->high.quality.resolution.y;
        cam_config.num_preview_video_frames = 3;
        cam_config.stills_capture_circular_buffer_height = 0;
        cam_config.fast_preview_resume = 0;
        cam_config.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC;
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_parameter_set(m_impl->camera->control, &cam_config.hdr));
        if (status != MMAL_SUCCESS)
        {
            SILK_ERR("Couldn't set camera config: error {}", status);
            return false;
        }
    }

    {
        // setup preview port format
        auto* format = camera_preview_port->format;
        format->encoding = MMAL_ENCODING_I420;
        format->encoding_variant = MMAL_ENCODING_VARIANT_DEFAULT;
        setup_video_format(format, m_impl->high.quality.resolution, true, m_fps);
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_format_commit(camera_preview_port));
        if (status != MMAL_SUCCESS)
        {
            SILK_ERR("Couldn't set preview port format : error {}", status);
            return false;
        }
    }

    {
        //setup video port format
        auto* format = camera_video_port->format;
        format->encoding = MMAL_ENCODING_OPAQUE;
        format->encoding_variant = MMAL_ENCODING_I420;
        setup_video_format(format, m_impl->high.quality.resolution, true, m_fps);
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_format_commit(camera_video_port));
        if (status != MMAL_SUCCESS)
        {
            SILK_ERR("Couldn't set video port format : error {}", status);
            return false;
        }
    }

    {
        //setup still port format
        auto* format = camera_capture_port->format;
        format->encoding = MMAL_ENCODING_I420;
        format->encoding_variant = MMAL_ENCODING_VARIANT_DEFAULT;
        format->es->video.width = VCOS_ALIGN_UP(m_impl->high.quality.resolution.x, 32);
        format->es->video.height = VCOS_ALIGN_UP(m_impl->high.quality.resolution.y, 16);
        format->es->video.crop.x = 0;
        format->es->video.crop.y = 0;
        format->es->video.crop.width = m_impl->high.quality.resolution.x;
        format->es->video.crop.height = m_impl->high.quality.resolution.y;
        format->es->video.frame_rate.num = 0;
        format->es->video.frame_rate.den = 1;
        format->es->video.par.num = 1;
        format->es->video.par.den = 1;
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_format_commit(camera_capture_port));
        if (status != MMAL_SUCCESS)
        {
            SILK_ERR("Couldn't set still port format : error {}", status);
            return false;
        }
    }

    static uint32_t buffer_count = 3;
    auto userdata = reinterpret_cast<MMAL_PORT_USERDATA_T*>(m_impl.get());

    //enable the camera
    if (!enable_component(m_impl->camera))
    {
        SILK_ERR("Cannot enable camera component");
        return false;
    }

    m_impl->camera_splitter = create_splitter_component(camera_preview_port);
    if (!m_impl->camera_splitter)
    {
        SILK_ERR("Cannot create camera splitter component");
        return false;
    }

    m_impl->camera_splitter_connection = connect_ports(camera_preview_port, m_impl->camera_splitter->input[0]);
    if (!m_impl->camera_splitter_connection || !set_connection_enabled(m_impl->camera_splitter_connection, true))
    {
        SILK_ERR("Cannot enable camera splitter");
        return false;
    }


    //high
    m_impl->high.encoder = create_encoder_component_for_saving(camera_video_port, m_impl->high.quality.resolution, m_impl->high.quality.bitrate);
    m_impl->high.encoder_connection = connect_ports(camera_video_port, m_impl->high.encoder->input[0]);
    if (!m_impl->high.encoder_connection || !set_connection_enabled(m_impl->high.encoder_connection, true))
    {
        SILK_ERR("Cannot create high bitrate encoder");
        return false;
    }

    m_impl->high.output_pool = create_output_port_pool(m_impl->high.encoder->output[0], userdata, high_encoder_buffer_callback, buffer_count);
    if (!m_impl->high.output_pool)
    {
        SILK_ERR("Cannot create high bitrate encoder port");
        return false;
    }

    //medium
    m_impl->medium.resizer = create_resizer_component(m_impl->camera_splitter->output[1], m_impl->medium.quality.resolution, m_fps);
    m_impl->medium.resizer_connection = connect_ports(m_impl->camera_splitter->output[1], m_impl->medium.resizer->input[0]);
    if (!m_impl->medium.resizer_connection)
    {
        SILK_ERR("Cannot create medium bitrate resizer");
        return false;
    }
    m_impl->medium.encoder = create_encoder_component_for_streaming(m_impl->medium.resizer->output[0], m_impl->medium.quality.resolution, m_impl->medium.quality.bitrate);
    m_impl->medium.encoder_connection = connect_ports(m_impl->medium.resizer->output[0], m_impl->medium.encoder->input[0]);
    if (!m_impl->medium.encoder_connection || !set_connection_enabled(m_impl->medium.encoder_connection, true))
    {
        SILK_ERR("Cannot create medium bitrate encoder");
        return false;
    }
    m_impl->medium.output_pool = create_output_port_pool(m_impl->medium.encoder->output[0], userdata, medium_encoder_buffer_callback, buffer_count);
    if (!m_impl->medium.output_pool)
    {
        SILK_ERR("Cannot create medium bitrate encoder port");
        return false;
    }

    //low
    m_impl->low.resizer = create_resizer_component(m_impl->camera_splitter->output[2], m_impl->low.quality.resolution, m_fps);
    m_impl->low.resizer_connection = connect_ports(m_impl->camera_splitter->output[2], m_impl->low.resizer->input[0]);
    if (!m_impl->low.resizer_connection)
    {
        SILK_ERR("Cannot create low bitrate resizer");
        return false;
    }
    m_impl->low.encoder = create_encoder_component_for_streaming(m_impl->low.resizer->output[0], m_impl->low.quality.resolution, m_impl->low.quality.bitrate);
    m_impl->low.encoder_connection = connect_ports(m_impl->low.resizer->output[0], m_impl->low.encoder->input[0]);
    if (!m_impl->low.encoder_connection || !set_connection_enabled(m_impl->low.encoder_connection, true))
    {
        SILK_ERR("Cannot create low bitrate encoder");
        return false;
    }
    m_impl->low.output_pool = create_output_port_pool(m_impl->low.encoder->output[0], userdata, low_encoder_buffer_callback, buffer_count);
    if (!m_impl->low.output_pool)
    {
        SILK_ERR("Cannot create low bitrate encoder port");
        return false;
    }


    {
        MMAL_STATUS_T status;
        status = MMAL_CALL(mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1));
        if (status != MMAL_SUCCESS)
        {
            SILK_ERR("failed to start capturing");
            return false;
        }
    }
    return true;
}

void HAL_Raspicam::create_file_sink()
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
        q::util::format(filepath, "capture/{}-{}", mbstr, file_idx);
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

    SILK_WARNING("Failed to create capture file.");
    m_file_sink.reset();
}


auto HAL_Raspicam::start_recording() -> bool
{
    if (!!q::util::fs::is_folder(q::Path("capture")) && !q::util::fs::create_folder(q::Path("capture")))
    {
        SILK_WARNING("Cannot create capture folder");
        return false;
    }

    if (!m_file_sink)
    {
        create_file_sink();
    }
    if (m_impl->camera)
    {
//        set_active_streams(m_file_sink != nullptr,
//                           m_stream_quality == camera_input::Stream_Quality::MEDIUM,
//                           m_stream_quality == camera_input::Stream_Quality::LOW);
    }
    return true;
}
void HAL_Raspicam::stop_recording()
{
    m_file_sink.reset();
    if (m_impl->camera)
    {
//        set_active_streams(m_file_sink != nullptr,
//                           m_stream_quality == camera_input::Stream_Quality::MEDIUM,
//                           m_stream_quality == camera_input::Stream_Quality::LOW);
    }
}

void HAL_Raspicam::set_iso(camera_input::Iso iso)
{
    m_iso = iso;
}

void HAL_Raspicam::set_shutter_speed(camera_input::Shutter_Speed ss)
{
    m_shutter_speed = ss;
}

void HAL_Raspicam::set_quality(camera_input::Stream_Quality sq)
{
    m_stream_quality = sq;
    if (m_impl->camera)
    {
//        set_active_streams(m_file_sink != nullptr,
//                           m_stream_quality == camera_input::Stream_Quality::MEDIUM,
//                           m_stream_quality == camera_input::Stream_Quality::LOW);
    }
}

#endif

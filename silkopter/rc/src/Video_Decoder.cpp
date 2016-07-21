#include "Video_Decoder.h"

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
//#define MMAL_CALL(x) x, LOG_INFO("MMAL: {}", QUOTE(x))
#define MMAL_CALL(x) x

typedef std::shared_ptr<MMAL_COMPONENT_T> Component_ptr;
typedef std::shared_ptr<MMAL_CONNECTION_T> Connection_ptr;
typedef std::shared_ptr<MMAL_POOL_T> Pool_ptr;

#define CHECK_STATUS(status, msg) if (status != MMAL_SUCCESS) { QLOGE("{}: (status={} {})", msg, status, mmal_status_to_string(status)); }

#endif

struct Video_Decoder::Impl
{
//    Component_ptr decoder;
//    Pool_ptr input_pool;

//    Component_ptr renderer;
//    Connection_ptr renderer_connection;

    MMAL_COMPONENT_T* decoder = nullptr;
    MMAL_POOL_T* pool_in = nullptr;
//    MMAL_POOL_T* pool_out = 0;
//    MMAL_QUEUE_T* queue = 0;

    MMAL_COMPONENT_T* renderer = nullptr;
    MMAL_CONNECTION_T* decoder_renderer_connection = nullptr;
};

Video_Decoder::Video_Decoder()
    : m_impl(new Impl)
    , m_video_texture(new QOpenGLTexture(QOpenGLTexture::Target2D))
{

}

Video_Decoder::~Video_Decoder()
{

}

QOpenGLTexture* Video_Decoder::get_video_texture()
{
    return m_video_texture;
}

bool Video_Decoder::init()
{
    return create_components();
}

bool Video_Decoder::decode_samples(std::vector<silk::stream::IVideo::Sample> const& samples)
{
    for (silk::stream::IVideo::Sample const& sample: samples)
    {
        uint8_t const* data_ptr = sample.value.data.data();
        size_t data_size = sample.value.data.size();
        size_t buffer_count = 0;
        while (data_size > 0)
        {
            // Send data to decode to the input port of the video decoder
            if (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->pool_in->queue))
            {
                mmal_buffer_header_reset(buffer);
                buffer->cmd = 0;
                buffer->pts = MMAL_TIME_UNKNOWN;
                buffer->dts = MMAL_TIME_UNKNOWN;
                //buffer->flags = frame.is_keyframe ?

                buffer->length = std::min<size_t>(data_size, buffer->alloc_size);
                memcpy(buffer->data, data_ptr, buffer->length);
                data_size -= buffer->length;
                data_ptr += buffer->length;

                MMAL_STATUS_T status = mmal_port_send_buffer(m_impl->decoder->input[0], buffer);
                if (status != MMAL_SUCCESS)
                {
                    QLOGW("\tCannot send buffer: {}", mmal_status_to_string(status));
                }
                buffer_count++;
            }
        }
    }

//    while (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->queue))
//    {
//        /* We have a frame, do something with it (why not display it for instance?).
//          * Once we're done with it, we release it. It will automatically go back
//          * to its original pool so it can be reused for a new video frame.
//          */
//        bool eos_received = buffer->flags & MMAL_BUFFER_HEADER_FLAG_EOS;

//        if (buffer->cmd)
//            fprintf(stderr, "received event %4.4s", (char*)&buffer->cmd);
//        else
//        {
//            //fprintf(stderr, "decoded frame (flags %x)\n", buffer->flags);
//        }

//        mmal_buffer_header_release(buffer);
//    }

//    /* Send empty buffers to the output port of the decoder */
//    while (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->pool_out->queue))
//    {
//        MMAL_STATUS_T status = mmal_port_send_buffer(m_impl->decoder->output[0], buffer);
//        CHECK_STATUS(status, "failed to send buffer");
//    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//static void dump_format_info(size_t tabs, MMAL_ES_FORMAT_T* format)
//{
//    //    SCOPED_PINS_GUARD;

//    QASSERT(format);
//    std::string prefix(tabs, '\t');
//    char encoding[5] = {0};
//    char encoding_variant[5] = {0};
//    char color_space[5] = {0};
//    memcpy(encoding, &format->encoding, 4);
//    memcpy(encoding_variant, &format->encoding_variant, 4);
//    memcpy(color_space, &format->es->video.color_space, 4);

//    QLOGI("{}type: {}", prefix, format->type);
//    QLOGI("{}encoding: {} / variant: {}", prefix, encoding, encoding_variant);
//    QLOGI("{}bitrate: {}", prefix, format->bitrate);
//    QLOGI("{}flags: {}", prefix, format->flags);
//    QLOGI("{}extradata_size: {}", prefix, format->extradata_size);
//    QLOGI("{}width: {} / height: {}", prefix, format->es->video.width, format->es->video.height);
//    QLOGI("{}crop (x: {}, y: {}, w: {}, h: {})", prefix, format->es->video.crop.x, format->es->video.crop.y, format->es->video.crop.width, format->es->video.crop.height);
//    QLOGI("{}frame_rate: num: {} / den: {}", prefix, format->es->video.frame_rate.num, format->es->video.frame_rate.den);
//    QLOGI("{}par: num: {} / den: {}", prefix, format->es->video.par.num, format->es->video.par.den);
//    QLOGI("{}color_space: {}", prefix, color_space);
//}

////static void dump_port_info(size_t tabs, MMAL_PORT_T* port)
////{
////    //    SCOPED_PINS_GUARD;;

////    QASSERT(port);
////    std::string prefix(tabs, '\t');
////    QLOGI("{}name: {}", prefix, port->name);
////    QLOGI("{}type: {}", prefix, port->type);
////    QLOGI("{}index: {} / index_all: {}", prefix, port->index, port->index_all);
////    QLOGI("{}is_enabled: {}", prefix, port->is_enabled);
////    QLOGI("{}buffer_num_min: {} / buffer_size_min: {} / buffer_alignment_min: {}",
////               prefix, port->buffer_num_min, port->buffer_size_min, port->buffer_alignment_min);
////    QLOGI("{}buffer_num_recommended: {} / buffer_size_recommended: {}",
////               prefix, port->buffer_num_recommended, port->buffer_size_recommended);
////    QLOGI("{}buffer_num: {} / buffer_size: {}", prefix, port->buffer_num, port->buffer_size);
////    QLOGI("{}capabilities: {}", prefix, port->capabilities);
////    QLOGI("{}format:", prefix);
////    dump_format_info(tabs + 1, port->format);
////}

//static Connection_ptr connect_ports(MMAL_PORT_T* output, MMAL_PORT_T* input)
//{
//    MMAL_CONNECTION_T* connection = nullptr;
//    MMAL_STATUS_T status = mmal_connection_create(&connection, output, input, MMAL_CONNECTION_FLAG_TUNNELLING | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
//    return (status == MMAL_SUCCESS) ? Connection_ptr(connection, mmal_connection_destroy) : Connection_ptr();
//}

//static Component_ptr create_component(char const* name, size_t min_input_count, size_t min_output_count)
//{
//    QASSERT(name);

//    QLOGI("Creating component {}", name);
//    MMAL_COMPONENT_T* component = nullptr;
//    MMAL_STATUS_T status;
//    status = MMAL_CALL(mmal_component_create(name, &component));
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGW("\tCannot create component {}: {}", name, mmal_status_to_string(status));
//        return Component_ptr();
//    }
//    QLOGI("\t{} has {} inputs and {} outputs:", name, component->input_num, component->output_num);
//    if (component->input_num < min_input_count || component->output_num < min_output_count)
//    {
//        QLOGW("\t{} has an invalid number of inputs and/or outputs: {}<{}, {}<{}", name,
//              component->input_num, min_input_count, component->output_num, min_output_count);
//        return Component_ptr();
//    }

//    //    for (size_t i = 0; i < component->input_num; i++)
//    //    {
//    //        LOG_INFO("\tinitial input port config {}", i);
//    //        dump_port_info(2, component->input[i]);
//    //    }
//    //    for (size_t i = 0; i < component->output_num; i++)
//    //    {
//    //        LOG_INFO("\tinitial output port config {}", i);
//    //        dump_port_info(2, component->output[i]);
//    //    }

//    return Component_ptr(component, mmal_component_destroy);
//}

//static bool enable_port(MMAL_PORT_T* port, MMAL_PORT_BH_CB_T cb)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(port);
//    QASSERT(cb);

//    QLOGI("Enabling port {} from {}. CB: {}", port->name, port->component->name, cb);
//    MMAL_STATUS_T status;
//    status = MMAL_CALL(mmal_port_enable(port, cb));
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGW("Unable to enable port {} from {}: {}", port->name, port->component->name, mmal_status_to_string(status));
//    }
//    return status == MMAL_SUCCESS;
//}
//static bool enable_component(Component_ptr component)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(component);
//    QLOGI("Enabling component {}", component->name);
//    MMAL_STATUS_T status;
//    status = MMAL_CALL(mmal_component_enable(component.get()));
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGW("Unable to enable component {}: {}", component->name, mmal_status_to_string(status));
//    }
//    return status == MMAL_SUCCESS;
//}
//static void copy_format(MMAL_PORT_T* dst, MMAL_PORT_T* src)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(dst);
//    QASSERT(src);

//    QLOGI("Copying format from {} to {}:", src->name, dst->name);

//    MMAL_CALL(mmal_format_copy(dst->format, src->format));

//    //    LOG_INFO("src format:");
//    //    dump_format_info(1, src->format);
//    //    LOG_INFO("dst format:");
//    //    dump_format_info(1, dst->format);
//}
//static bool commit_format(MMAL_PORT_T* port)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(port);

//    QLOGI("Trying to set new format to port {}:", port->name);
//    //dump_format_info(1, port->format);

//    MMAL_STATUS_T status;
//    status = MMAL_CALL(mmal_port_format_commit(port));
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGW("Unable to commit format on port {}: {}", port->name, mmal_status_to_string(status));
//        return false;
//    }

//    port->buffer_size = math::max(port->buffer_size_recommended, port->buffer_size_min);
//    port->buffer_num = math::max(port->buffer_num_recommended, port->buffer_num_min);
//    //    dump_format_info(1, port->format);

//    return true;
//}

//MMAL_BOOL_T return_buffer_to_pool_callback(MMAL_POOL_T* pool, MMAL_BUFFER_HEADER_T* buffer, void* userdata)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(pool);
//    QASSERT(buffer);

//    MMAL_PORT_T* port = reinterpret_cast<MMAL_PORT_T*>(userdata);
//    QASSERT(port);

//    //LOG_INFO("{}: RETURNING buffer {} / port {} size {}k", port->name, buffer, port, buffer->length / 1024.f);

//    // and send one back to the port (if still open)
//    if (port->is_enabled)
//    {
//        MMAL_STATUS_T status;
//        status = MMAL_CALL(mmal_port_send_buffer(port, buffer));
//        if (status != MMAL_SUCCESS)
//        {
//            QLOGW("Unable to return a buffer to the encoder port: {}", mmal_status_to_string(status));
//        }
//    }
//    else
//    {
//        QLOGW("Port is disabled");
//    }
//    return MMAL_TRUE;
//}

//static void release_buffer_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
//{
//    // The decoder is done with the data, just recycle the buffer header into its pool
//    mmal_buffer_header_release(buffer);
//}

//static Pool_ptr create_port_pool(MMAL_PORT_T* port, MMAL_PORT_USERDATA_T* userdata, MMAL_PORT_BH_CB_T cb, uint32_t buffer_count)
//{
//    //    SCOPED_PINS_GUARD;;

//    //setup video port buffer and a pool to hold them
//    port->buffer_size = math::max(port->buffer_size_recommended, port->buffer_size_min);
//    port->buffer_num = math::max(port->buffer_num_recommended, port->buffer_num_min);
//    port->buffer_num = math::max(port->buffer_num, buffer_count);

//    QLOGI("Creating port {} pool with {} buffers of size {}", port->name, port->buffer_num, port->buffer_size);

//    MMAL_POOL_T* p = nullptr;
//    p = MMAL_CALL(mmal_port_pool_create(port, port->buffer_num, port->buffer_size));
//    Pool_ptr o_pool(p, [=](MMAL_POOL_T* pool) { mmal_port_disable(port); mmal_port_pool_destroy(port, pool); } );
//    if (!o_pool)
//    {
//        return Pool_ptr();
//    }

//    port->userdata = userdata;
//    if (!enable_port(port, cb))
//    {
//        return Pool_ptr();
//    }

//    mmal_pool_callback_set(o_pool.get(), return_buffer_to_pool_callback, port);

//    //send all the buffers in our pool to the video port ready for use
//    {
//        int num = 0;
//        num = MMAL_CALL(mmal_queue_length(o_pool->queue));
//        QLOGI("enabling {} buffers for {} port {}", num, port->component->name, port->name);
//        for (int i = 0; i < num; i++)
//        {
//            MMAL_BUFFER_HEADER_T* buffer = nullptr;
//            buffer = MMAL_CALL(mmal_queue_get(o_pool->queue));
//            if (!buffer)
//            {
//                QLOGW("Unable to get a required buffer {} from pool queue", i);
//                return Pool_ptr();
//            }
//            if (mmal_port_send_buffer(port, buffer) != MMAL_SUCCESS)
//            {
//                QLOGW("Unable to send a buffer to encoder port ({})", i);
//                return Pool_ptr();
//            }
//        }
//    }

//    return o_pool;
//}


//static bool setup_decoder_component(Component_ptr encoder, math::vec2u32 const& resolution)
//{
//    //    SCOPED_PINS_GUARD;;

//    QASSERT(encoder);


//    MMAL_STATUS_T status = mmal_port_enable(encoder->control, control_callback);
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGE("Failed to set control callback (status={} {})", status, mmal_status_to_string(status));
//        return false;
//    }

//    auto* input = encoder->input[0];
//    QASSERT(input);

//    //    copy_format(input, src);
//    input->format->type = MMAL_ES_TYPE_VIDEO;
//    input->format->encoding = MMAL_ENCODING_H264;
//    input->format->es->video.width = resolution.x;//VCOS_ALIGN_UP(resolution.x, 32);
//    input->format->es->video.height = resolution.y;//VCOS_ALIGN_UP(resolution.y, 16);
//    input->format->es->video.crop.x = 0;
//    input->format->es->video.crop.y = 0;
//    input->format->es->video.crop.width = resolution.x;
//    input->format->es->video.crop.height = resolution.y;
//    // We need to set the frame rate on input to 0, to ensure it gets
//    // updated correctly from the input framerate when port connected
//    input->format->es->video.frame_rate.num = 30;
//    input->format->es->video.frame_rate.den = 1;

//    input->format->flags |= MMAL_ES_FORMAT_FLAG_FRAMED;

//    //input->format->es->video.par.num = 1;
//    //input->format->es->video.par.den = 1;

//    status = mmal_port_parameter_set_boolean(input, MMAL_PARAMETER_VIDEO_DECODE_ERROR_CONCEALMENT, MMAL_FALSE);
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGE("Failed to disable error concealment on {} (status={} {})", input->name, status, mmal_status_to_string(status));
//        return false;
//    }
//    status = mmal_port_parameter_set_boolean(input, MMAL_PARAMETER_ZERO_COPY, MMAL_TRUE);
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGE("Failed to enable zero-copy on {} (status={} {})", input->name, status, mmal_status_to_string(status));
//        return false;
//    }

//    input->buffer_num = input->buffer_num_min;
//    input->buffer_size = input->buffer_size_min;

//    if (!commit_format(input))
//    {
//        return false;
//    }

//    auto* output = encoder->output[0];
//    QASSERT(output);

//    //output[0]->buffer_num = output[0]->buffer_num_min;
//    //output[0]->buffer_size = output[0]->buffer_size_min;

//    // We want same format on input and output
//    //copy_format(output, input);
//    //MMAL_CALL(mmal_format_copy(output->format, input->format));

//    // Only supporting H264 at the moment
//    output->format->encoding = MMAL_ENCODING_OPAQUE;

//    status = mmal_port_parameter_set_boolean(output, MMAL_PARAMETER_ZERO_COPY, MMAL_TRUE);
//    if (status != MMAL_SUCCESS)
//    {
//        QLOGE("Failed to enable zero-copy on {} (status={} {})", input->name, status, mmal_status_to_string(status));
//        return false;
//    }

//    // Commit the port changes to the output port
//    QLOGI("setting format for encoder component {}", encoder->name);
//    bool res = commit_format(output);

//    //output->buffer_num = 3;

//    return res;
//}

static void control_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    switch (buffer->cmd)
    {
    case MMAL_EVENT_EOS:
        /* Only sink component generate EOS events */
        break;
    case MMAL_EVENT_ERROR:
    {
        /* Something went wrong. Signal this to the application */
        MMAL_STATUS_T status = *(MMAL_STATUS_T*)buffer->data;
        QLOGW("Control callback reported an error: {} / {}", status, mmal_status_to_string(status));
        break;
    }
    default:
        break;
    }

    /* Done with the event, recycle it */
    mmal_buffer_header_release(buffer);
}

static void input_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
   //struct CONTEXT_T *ctx = (struct CONTEXT_T *)port->userdata;

   /* The decoder is done with the data, just recycle the buffer header into its pool */
   mmal_buffer_header_release(buffer);

   /* Kick the processing thread */
   //vcos_semaphore_post(&ctx->semaphore);
}

/** Callback from the output port.
 * Buffer has been produced by the port and is available for processing. */
static void output_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
   Video_Decoder::Impl* impl = (Video_Decoder::Impl*)port->userdata;

   /* Queue the decoded video frame */
   //mmal_queue_put(impl->queue, buffer);

   /* Kick the processing thread */
   //vcos_semaphore_post(&ctx->semaphore);
}


bool Video_Decoder::create_components()
{
    MMAL_STATUS_T status = MMAL_EINVAL;
    MMAL_BOOL_T eos_sent = MMAL_FALSE, eos_received = MMAL_FALSE;
    unsigned int count;

    /* Create the decoder component.
        * This specific component exposes 2 ports (1 input and 1 output). Like most components
        * its expects the format of its input port to be set by the client in order for it to
        * know what kind of data it will be fed. */
    status = mmal_component_create(MMAL_COMPONENT_DEFAULT_VIDEO_DECODER, &m_impl->decoder);
    CHECK_STATUS(status, "failed to create decoder");

    /* Enable control port so we can receive events from the component */
    //m_impl->decoder->control->userdata = (void *)&context;
    status = mmal_port_enable(m_impl->decoder->control, control_callback);
    CHECK_STATUS(status, "failed to enable control port");

    /* Get statistics on the input port */
    MMAL_PARAMETER_CORE_STATISTICS_T stats = {{0}};
    stats.hdr.id = MMAL_PARAMETER_CORE_STATISTICS;
    stats.hdr.size = sizeof(MMAL_PARAMETER_CORE_STATISTICS_T);
    status = mmal_port_parameter_get(m_impl->decoder->input[0], &stats.hdr);
    CHECK_STATUS(status, "failed to get stats");
    fprintf(stderr, "stats: %i, %i", stats.stats.buffer_count, stats.stats.max_delay);

    /* Set the zero-copy parameter on the input port */
    //MMAL_PARAMETER_BOOLEAN_T zc = {{MMAL_PARAMETER_ZERO_COPY, sizeof(zc)}, MMAL_TRUE};
    //status = mmal_port_parameter_set(m_impl->decoder->input[0], &zc.hdr);
    //fprintf(stderr, "status: %i\n", status);

    /* Set the zero-copy parameter on the output port */
    //status = mmal_port_parameter_set_boolean(m_impl->decoder->output[0], MMAL_PARAMETER_ZERO_COPY, MMAL_TRUE);
    //fprintf(stderr, "status: %i\n", status);

    /* Set format of video decoder input port */
    MMAL_ES_FORMAT_T* format_in = m_impl->decoder->input[0]->format;
    format_in->type = MMAL_ES_TYPE_VIDEO;
    format_in->encoding = MMAL_ENCODING_H264;
    format_in->es->video.width = 320;
    format_in->es->video.height = 240;
    format_in->es->video.frame_rate.num = 30;
    format_in->es->video.frame_rate.den = 1;
    format_in->es->video.par.num = 1;
    format_in->es->video.par.den = 1;
    /* If the data is known to be framed then the following flag should be set:
        * format_in->flags |= MMAL_ES_FORMAT_FLAG_FRAMED; */

    //SOURCE_READ_CODEC_CONFIG_DATA(codec_header_bytes, codec_header_bytes_size);
    //status = mmal_format_extradata_alloc(format_in, codec_header_bytes_size);
    //CHECK_STATUS(status, "failed to allocate extradata");
    //format_in->extradata_size = codec_header_bytes_size;
    //if (format_in->extradata_size)
    //    memcpy(format_in->extradata, codec_header_bytes, format_in->extradata_size);

    status = mmal_port_format_commit(m_impl->decoder->input[0]);
    CHECK_STATUS(status, "failed to commit format");

    /* Our decoder can do internal colour conversion, ask for a conversion to RGB565 */
    MMAL_ES_FORMAT_T* format_out = m_impl->decoder->output[0]->format;
    format_out->encoding = MMAL_ENCODING_RGB16;
    status = mmal_port_format_commit(m_impl->decoder->output[0]);
    CHECK_STATUS(status, "failed to commit format");

    /* Display the output port format */
    fprintf(stderr, "%s\n", m_impl->decoder->output[0]->name);
    fprintf(stderr, " type: %i, fourcc: %4.4s\n", format_out->type, (char *)&format_out->encoding);
    fprintf(stderr, " bitrate: %i, framed: %i\n", format_out->bitrate,
            !!(format_out->flags & MMAL_ES_FORMAT_FLAG_FRAMED));
    fprintf(stderr, " extra data: %i, %p\n", format_out->extradata_size, format_out->extradata);
    fprintf(stderr, " width: %i, height: %i, (%i,%i,%i,%i)\n",
            format_out->es->video.width, format_out->es->video.height,
            format_out->es->video.crop.x, format_out->es->video.crop.y,
            format_out->es->video.crop.width, format_out->es->video.crop.height);

    /* The format of both ports is now set so we can get their buffer requirements and create
        * our buffer headers. We use the buffer pool API to create these. */
    m_impl->decoder->input[0]->buffer_num = m_impl->decoder->input[0]->buffer_num_recommended;
    m_impl->decoder->input[0]->buffer_size = m_impl->decoder->input[0]->buffer_size_recommended;
    m_impl->decoder->output[0]->buffer_num = m_impl->decoder->output[0]->buffer_num_recommended;
    m_impl->decoder->output[0]->buffer_size = m_impl->decoder->output[0]->buffer_size_recommended;

    m_impl->pool_in = mmal_pool_create(m_impl->decoder->input[0]->buffer_num, m_impl->decoder->input[0]->buffer_size);
    //m_impl->pool_out = mmal_pool_create(m_impl->decoder->output[0]->buffer_num, m_impl->decoder->output[0]->buffer_size);

    /* Create a queue to store our decoded video frames. The callback we will get when
        * a frame has been decoded will put the frame into this queue. */
    //m_impl->queue = mmal_queue_create();

    /* Store a reference to our context in each port (will be used during callbacks) */
    m_impl->decoder->input[0]->userdata = (MMAL_PORT_USERDATA_T*)m_impl.get();
    m_impl->decoder->output[0]->userdata = (MMAL_PORT_USERDATA_T*)m_impl.get();

    /* Enable all the input port and the output port.
        * The callback specified here is the function which will be called when the buffer header
        * we sent to the component has been processed. */
    status = mmal_port_enable(m_impl->decoder->input[0], input_callback);
    CHECK_STATUS(status, "failed to enable input port");
//    status = mmal_port_enable(m_impl->decoder->output[0], output_callback);
//    CHECK_STATUS(status, "failed to enable output port");

    /* Component won't start processing data until it is enabled. */
    status = mmal_component_enable(m_impl->decoder);
    CHECK_STATUS(status, "failed to enable component");

    //////////////////////////////////////////////

    status = mmal_component_create(MMAL_COMPONENT_DEFAULT_VIDEO_RENDERER, &m_impl->renderer);
    CHECK_STATUS(status, "failed to create decoder");
    status = mmal_port_enable(m_impl->renderer->control, control_callback);
    CHECK_STATUS(status, "failed to enable control port");

    MMAL_DISPLAYREGION_T param;
    param.hdr.id = MMAL_PARAMETER_DISPLAYREGION;
    param.hdr.size = sizeof(MMAL_DISPLAYREGION_T);

    param.set = MMAL_DISPLAY_SET_LAYER;
    param.layer = 2;

//    param.set |= MMAL_DISPLAY_SET_ALPHA;
//    param.alpha = state->opacity;

    param.set |= MMAL_DISPLAY_SET_FULLSCREEN;
    param.fullscreen = 1;

    status = mmal_port_parameter_set(m_impl->renderer->input[0], &param.hdr);
    CHECK_STATUS(status, "failed to enable component");

    status = mmal_component_enable(m_impl->renderer);
    CHECK_STATUS(status, "failed to enable component");

    /* Create the connections between the components */
    status = mmal_connection_create(&m_impl->decoder_renderer_connection, m_impl->decoder->output[0], m_impl->renderer->input[0], MMAL_CONNECTION_FLAG_TUNNELLING);
    CHECK_STATUS(status, "failed to create connection between decoder / renderer");
    //m_impl->decoder_renderer_connection->callback = connection_callback;
    status = mmal_connection_enable(m_impl->decoder_renderer_connection);
    CHECK_STATUS(status, "failed to enable connection");

    /* Start decoding */
    fprintf(stderr, "start decoding\n");

    return true;
}

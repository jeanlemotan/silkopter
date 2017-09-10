#include "Video_Decoder.h"

#include <deque>
#include <QOpenGLContext>

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

#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
}

#define _QUOTE(str) #str
#define QUOTE(str) _QUOTE(str)
//#define MMAL_CALL(x) x, LOG_INFO("MMAL: {}", QUOTE(x))
#define MMAL_CALL(x) x

#define CHECK_STATUS(status, msg) if (status != MMAL_SUCCESS) { QLOGE("{}: (status={} {})", msg, status, mmal_status_to_string(status)); }

#endif


#define CHECK_GL_ERRORS

#if defined(CHECK_GL_ERRORS)
#define GLCHK(X) \
do { \
    GLenum err = GL_NO_ERROR; \
    X; \
   while ((err = glGetError())) \
   { \
      QLOGE("GL error {} in " #X "file {} line {}", err, __FILE__,__LINE__); \
   } \
} \
while(0)
#define EGLCHK \
do { \
    GLenum err = EGL_SUCCESS; \
   while ((err = eglGetError()) != EGL_SUCCESS) \
   { \
      QLOGE("EGL error {} file {} line {}", err, __FILE__,__LINE__); \
   } \
} \
while(0)
#else
#define GLCHK(X) X
#define EGLCHK()
#endif /* CHECK_GL_ERRORS */

struct Video_Decoder::Impl
{
    MMAL_COMPONENT_T* decoder = nullptr;
    MMAL_POOL_T* pool_in = nullptr;
    MMAL_POOL_T* pool_out = 0;
    MMAL_QUEUE_T* queue = 0;

    std::deque<MMAL_BUFFER_HEADER_T*> old_buffers;
    std::deque<EGLImageKHR> old_egl_images;

    EGLImageKHR egl_image = EGL_NO_IMAGE_KHR;
    EGLDisplay display = EGL_NO_DISPLAY;
};


static void control_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
    switch (buffer->cmd)
    {
    case MMAL_EVENT_EOS:
        break;
    case MMAL_EVENT_ERROR:
    {
        MMAL_STATUS_T status = *(MMAL_STATUS_T*)buffer->data;
        QLOGW("Control callback reported an error: {} / {}", status, mmal_status_to_string(status));
        break;
    }
    default:
        break;
    }

    mmal_buffer_header_release(buffer);
}

static void input_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
   mmal_buffer_header_release(buffer);
}

/** Callback from the output port.
 * Buffer has been produced by the port and is available for processing. */
static void output_callback(MMAL_PORT_T* port, MMAL_BUFFER_HEADER_T* buffer)
{
   Video_Decoder::Impl* impl = (Video_Decoder::Impl*)port->userdata;
   mmal_queue_put(impl->queue, buffer);
}




Video_Decoder::Video_Decoder()
    : m_impl(new Impl)
{
}

Video_Decoder::~Video_Decoder()
{
    if (m_video_texture != 0)
    {
        glDeleteTextures(1, &m_video_texture);
        m_video_texture = 0;
    }
}

bool Video_Decoder::init()
{
    m_impl->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    GLCHK(glGenTextures(1, &m_video_texture));
    GLCHK(glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_video_texture));
    GLCHK(glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCHK(glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCHK(glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCHK(glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    return true;
}

uint32_t Video_Decoder::get_video_texture_id()
{
    return m_video_texture;
}

void Video_Decoder::release_buffers()
{
    //the prevous buffers
    while (!m_impl->old_buffers.empty())
    {
        mmal_buffer_header_release(m_impl->old_buffers.front());
        m_impl->old_buffers.pop_front();
    }

    if (m_impl->pool_out)
    {
        while (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->pool_out->queue))
        {
            MMAL_STATUS_T status = mmal_port_send_buffer(m_impl->decoder->output[0], buffer);
            CHECK_STATUS(status, "failed to send buffer");
        }
    }
}

bool Video_Decoder::decode_data(void const* data, size_t size, math::vec2u16 const& resolution)
{
    process_output();

//    for (silk::stream::IVideo::Sample const& sample: samples)
//    {
//        uint8_t const* data_ptr = sample.value.data.data();
//        size_t data_size = sample.value.data.size();
        size_t buffer_count = 0;

        if (m_resolution != resolution)
        {
            m_resolution = resolution;
            if (!create_components(m_resolution))
            {
                QLOGE("Cannot create components for resolution: {}", m_resolution);
                m_resolution = math::vec2u16::zero;
                return false;
            }
        }

        uint8_t const* data_ptr = reinterpret_cast<uint8_t const*>(data);

        while (size > 0)
        {
            // Send data to decode to the input port of the video decoder
            if (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->pool_in->queue))
            {
                mmal_buffer_header_reset(buffer);
                buffer->cmd = 0;
                buffer->pts = MMAL_TIME_UNKNOWN;
                buffer->dts = MMAL_TIME_UNKNOWN;
                //buffer->flags = frame.is_keyframe ?

                buffer->length = std::min<size_t>(size, buffer->alloc_size);
                memcpy(buffer->data, data_ptr, buffer->length);
                size -= buffer->length;
                data_ptr += buffer->length;

                MMAL_STATUS_T status = mmal_port_send_buffer(m_impl->decoder->input[0], buffer);
                if (status != MMAL_SUCCESS)
                {
                    QLOGW("\tCannot send buffer: {}", mmal_status_to_string(status));
                }
                buffer_count++;
            }
            else
            {
                break;
            }

            process_output();
        }
//    }

    return true;
}

void Video_Decoder::process_output()
{
    static const PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES
            = reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(eglGetProcAddress("glEGLImageTargetTexture2DOES"));

    while (!m_impl->old_egl_images.empty())
    {
        eglDestroyImageKHR(m_impl->display, m_impl->old_egl_images.front());
        m_impl->old_egl_images.pop_front();
    }

    if (m_impl->queue)
    {
        size_t buffer_count = 0;

        while (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->queue))
        {
            if (buffer->cmd)
            {
                fprintf(stderr, "received event %4.4s", (char*)&buffer->cmd);
            }
            else
            {
                if (buffer_count == 0 && m_video_texture > 0)
                {
                    GLCHK(glActiveTexture(GL_TEXTURE0));
                    GLCHK(glBindTexture(GL_TEXTURE_EXTERNAL_OES, m_video_texture));

                    EGLImageKHR image = eglCreateImageKHR(m_impl->display, EGL_NO_CONTEXT, EGL_IMAGE_BRCM_MULTIMEDIA, (EGLClientBuffer)buffer->data, NULL);
                    EGLCHK;
                    QASSERT(image != EGL_NO_IMAGE_KHR);

                    GLCHK(glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, image));

                    m_impl->old_egl_images.push_back(image);

                    buffer_count++;
                }
            }

            m_impl->old_buffers.push_back(buffer);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Video_Decoder::create_components(math::vec2u16 const& resolution)
{
    MMAL_STATUS_T status = MMAL_EINVAL;

    if (m_impl->decoder)
    {
        MMAL_PORT_T* input_port = m_impl->decoder->input[0];
        MMAL_PORT_T* output_port = m_impl->decoder->output[0];

        // Disable all our ports that are not handled by connections
        if (input_port && input_port->is_enabled)
        {
            mmal_port_disable(input_port);
        }
        if (output_port && output_port->is_enabled)
        {
            mmal_port_disable(output_port);
        }

        mmal_component_disable(m_impl->decoder);

        while (!m_impl->old_buffers.empty())
        {
            mmal_buffer_header_release(m_impl->old_buffers.front());
            m_impl->old_buffers.pop_front();
        }

        if (m_impl->pool_in)
        {
            mmal_port_pool_destroy(input_port, m_impl->pool_in);
            m_impl->pool_in = nullptr;
        }
        if (m_impl->pool_out)
        {
            mmal_port_pool_destroy(output_port, m_impl->pool_out);
            m_impl->pool_out = nullptr;
        }

        if (m_impl->queue)
        {
            mmal_queue_destroy(m_impl->queue);
            m_impl->queue = nullptr;
        }

        status = mmal_component_destroy(m_impl->decoder);
        CHECK_STATUS(status, "failed to destroy decoder");

        m_impl->decoder = nullptr;
    }

    status = mmal_component_create(MMAL_COMPONENT_DEFAULT_VIDEO_DECODER, &m_impl->decoder);
    CHECK_STATUS(status, "failed to create decoder");

    MMAL_PORT_T* input_port = m_impl->decoder->input[0];
    MMAL_PORT_T* output_port = m_impl->decoder->output[0];

    /* Set the zero-copy parameter on the input port */
    MMAL_PARAMETER_BOOLEAN_T zc = {{MMAL_PARAMETER_ZERO_COPY, sizeof(zc)}, MMAL_TRUE};
    status = mmal_port_parameter_set(input_port, &zc.hdr);
    CHECK_STATUS(status, "failed to commit format");

    /* Set format of video decoder input port */
    MMAL_ES_FORMAT_T* format_in = input_port->format;
    format_in->type = MMAL_ES_TYPE_VIDEO;
    format_in->encoding = MMAL_ENCODING_H264;
    format_in->es->video.width = resolution.x;
    format_in->es->video.height = resolution.y;
    format_in->es->video.frame_rate.num = 30;
    format_in->es->video.frame_rate.den = 1;
    format_in->es->video.par.num = 1;
    format_in->es->video.par.den = 1;
    //If the data is known to be framed then the following flag should be set:
    //format_in->flags |= MMAL_ES_FORMAT_FLAG_FRAMED;

    status = mmal_port_format_commit(input_port);
    CHECK_STATUS(status, "failed to commit format");

    /* Set the zero-copy parameter on the output port */
    status = mmal_port_parameter_set_boolean(output_port, MMAL_PARAMETER_ZERO_COPY, MMAL_TRUE);
    CHECK_STATUS(status, "failed to enable zero-copy");

    /* Our decoder can do internal colour conversion, ask for a conversion to RGB565 */
    MMAL_ES_FORMAT_T* format_out = output_port->format;
    format_out->encoding = MMAL_ENCODING_OPAQUE;
    format_out->encoding_variant = MMAL_ENCODING_I420;
    status = mmal_port_format_commit(output_port);
    CHECK_STATUS(status, "failed to commit format");

//    fprintf(stderr, "%s\n", output_port->name);
//    fprintf(stderr, " type: %i, fourcc: %4.4s\n", format_out->type, (char *)&format_out->encoding);
//    fprintf(stderr, " bitrate: %i, framed: %i\n", format_out->bitrate,
//            !!(format_out->flags & MMAL_ES_FORMAT_FLAG_FRAMED));
//    fprintf(stderr, " extra data: %i, %p\n", format_out->extradata_size, format_out->extradata);
//    fprintf(stderr, " width: %i, height: %i, (%i,%i,%i,%i)\n",
//            format_out->es->video.width, format_out->es->video.height,
//            format_out->es->video.crop.x, format_out->es->video.crop.y,
//            format_out->es->video.crop.width, format_out->es->video.crop.height);

    /* The format of both ports is now set so we can get their buffer requirements and create
        * our buffer headers. We use the buffer pool API to create these. */
    input_port->buffer_num = input_port->buffer_num_recommended;
    input_port->buffer_size = input_port->buffer_size_recommended;
    output_port->buffer_num = output_port->buffer_num_min;
    output_port->buffer_size = output_port->buffer_size_recommended;

    m_impl->pool_in = mmal_port_pool_create(input_port, input_port->buffer_num, input_port->buffer_size);
    m_impl->pool_out = mmal_port_pool_create(output_port, output_port->buffer_num, output_port->buffer_size);

    /* Create a queue to store our decoded video frames. The callback we will get when
        * a frame has been decoded will put the frame into this queue. */
    m_impl->queue = mmal_queue_create();

    /* Store a reference to our context in each port (will be used during callbacks) */
    input_port->userdata = (MMAL_PORT_USERDATA_T*)m_impl.get();
    output_port->userdata = (MMAL_PORT_USERDATA_T*)m_impl.get();

    /* Enable all the input port and the output port.
        * The callback specified here is the function which will be called when the buffer header
        * we sent to the component has been processed. */
    status = mmal_port_enable(input_port, input_callback);
    CHECK_STATUS(status, "failed to enable input port");
    status = mmal_port_enable(output_port, output_callback);
    CHECK_STATUS(status, "failed to enable output port");

    /* Enable control port so we can receive events from the component */
    //m_impl->decoder->control->userdata = (void *)&context;
    status = mmal_port_enable(m_impl->decoder->control, control_callback);
    CHECK_STATUS(status, "failed to enable control port");

    /* Component won't start processing data until it is enabled. */
    status = mmal_component_enable(m_impl->decoder);
    CHECK_STATUS(status, "failed to enable component");

    //send all the buffers in our pool to the video port ready for use
    while (MMAL_BUFFER_HEADER_T* buffer = mmal_queue_get(m_impl->pool_out->queue))
    {
        MMAL_STATUS_T status = mmal_port_send_buffer(output_port, buffer);
        CHECK_STATUS(status, "failed to send buffer");
    }

    return true;
}

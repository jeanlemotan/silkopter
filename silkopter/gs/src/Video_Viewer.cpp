#include "Video_Viewer.h"
#include "HAL.h"
#include "Comms.h"

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


Video_Viewer::Video_Viewer(QWidget *parent)
    : QWidget(parent)
{
    av_register_all();
    avcodec_register_all();
    avformat_network_init();

    m_ffmpeg.codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!m_ffmpeg.codec)
    {
        QLOGE("Codec not found");
        exit(1);
    }

    m_ffmpeg.context = avcodec_alloc_context3(m_ffmpeg.codec);
    if (!m_ffmpeg.context)
    {
        QLOGE("Could not allocate video codec context");
        exit(1);
    }

    avcodec_get_context_defaults3(m_ffmpeg.context, m_ffmpeg.codec);

    // 	if (m_ffmpeg.codec->capabilities & CODEC_CAP_TRUNCATED)
    // 	{
    // 		m_ffmpeg.context->flags |= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */
    // 	}
    //
    m_ffmpeg.context->flags |= CODEC_FLAG_LOW_DELAY;
    m_ffmpeg.context->flags2 |= CODEC_FLAG2_CHUNKS;

    // 	m_ffmpeg.context->width = 640;
    // 	m_ffmpeg.context->height = 480;
    // 	m_ffmpeg.context->coded_width = m_ffmpeg.context->width;
    // 	m_ffmpeg.context->coded_height = m_ffmpeg.context->height;
    // 	m_ffmpeg.context->pix_fmt = AV_PIX_FMT_YUV420P;
    // 	m_ffmpeg.context->debug = 0xFFFFF;
    // 	m_ffmpeg.context->time_base = av_d2q(0.033, 999999999);
    m_ffmpeg.context->thread_count = 4;
    m_ffmpeg.context->thread_type = FF_THREAD_SLICE;
    m_ffmpeg.context->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    // 	m_ffmpeg.context->profile = FF_PROFILE_H264_BASELINE;

    if (avcodec_open2(m_ffmpeg.context, m_ffmpeg.codec, nullptr) < 0)
    {
        QLOGE("Could not open codec");
        exit(1);
    }

    m_ffmpeg.frame_yuv = avcodec_alloc_frame();//av_frame_alloc();
    if (!m_ffmpeg.frame_yuv)
    {
        QLOGE("Could not allocate video frame");
        exit(1);
    }

    m_ffmpeg.frame_rgb = avcodec_alloc_frame();//av_frame_alloc();
    if (!m_ffmpeg.frame_rgb)
    {
        QLOGE("Could not allocate video frame");
        exit(1);
    }

    m_ffmpeg.rgb.reset(new AVPicture);
    m_ffmpeg.rgb->linesize[0] = 0;
    m_ffmpeg.rgb->data[0] = nullptr;
}

Video_Viewer::~Video_Viewer()
{
}


void Video_Viewer::add_sample(silk::node::stream::Video::Sample const& sample)
{
    {
        AVPacket packet;
        av_init_packet(&packet);

        packet.pts = AV_NOPTS_VALUE;
        packet.dts = AV_NOPTS_VALUE;
        packet.data = const_cast<uint8_t*>(sample.value.data.data());
        packet.size = sample.value.data.size();

        int got_frame = 0;
        int len = avcodec_decode_video2(m_ffmpeg.context, m_ffmpeg.frame_yuv, &got_frame, &packet);
        if (len < 0)
        {
            QLOGE("Error while decoding frame");
            return;
        }
        if (got_frame)
        {
            int frame_w = m_ffmpeg.frame_yuv->width;
            int frame_h = m_ffmpeg.frame_yuv->height;

            float ar = (float)frame_w / frame_h;
            int img_w = width();
            int img_h = img_w / ar;
            if (img_h > height())
            {
                img_h = height();
                img_w = img_h * ar;
            }

            m_ffmpeg.sws_context = sws_getCachedContext(m_ffmpeg.sws_context,
                                                        frame_w, frame_h,
                                                        m_ffmpeg.context->pix_fmt,
                                                        img_w, img_h,
                                                        PIX_FMT_RGB32,
                                                        SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

            if (m_ffmpeg.sws_context)
            {
                if (img_w * 4 != m_ffmpeg.rgb->linesize[0])
                {
                    m_ffmpeg.rgb->linesize[0] = img_w * 4;
                    delete[] m_ffmpeg.rgb->data[0];
                    m_ffmpeg.rgb->data[0] = new uint8_t[(m_ffmpeg.rgb->linesize[0] + 1) * (img_h + 1)];
                    m_image = QImage(m_ffmpeg.rgb->data[0], img_w, img_h, QImage::Format_ARGB32_Premultiplied);
                }

                sws_scale(m_ffmpeg.sws_context,
                          m_ffmpeg.frame_yuv->data, m_ffmpeg.frame_yuv->linesize,
                          0, frame_h,
                          m_ffmpeg.rgb->data, m_ffmpeg.rgb->linesize);

                m_image_flipped = m_image.mirrored(false, false);
            }

            update();
        }
        else
        {
            //QLOGW("Discarded frame {}", m_frame.header.idx);
        }
    }
}

void Video_Viewer::paintEvent(QPaintEvent* ev)
{
    m_painter.begin(this);
    m_painter.setCompositionMode(QPainter::CompositionMode_Source);
    m_painter.drawImage(QRectF(0, 0, m_image.width(), m_image.height()), m_image_flipped);
    m_painter.end();
}


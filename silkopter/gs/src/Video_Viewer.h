#pragma once

#include "HAL.h"
#include "Comms.h"

extern "C"
{
struct AVCodec;
struct AVCodecContext;
struct SwsContext;
struct AVFrame;
struct AVPicture;
}


class Video_Viewer : public QWidget
{
public:
    Video_Viewer(QWidget *parent);
    virtual ~Video_Viewer();
    void add_sample(silk::node::stream::gs::Video::Sample const& sample);

private:
    void paintEvent(QPaintEvent* ev);


    QPainter m_painter;
    QImage m_image;
    QImage m_image_flipped;
    math::vec2u16 m_resolution;
    std::vector<uint8_t> m_data;

    struct FFMPEG
    {
        AVCodec* codec = nullptr;
        AVCodecContext* context = nullptr;
        SwsContext* sws_context = nullptr;
        AVFrame* frame_yuv = nullptr;
        AVFrame* frame_rgb = nullptr;
        std::shared_ptr<AVPicture> rgb;
    } m_ffmpeg;
};

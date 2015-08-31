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


class Video_Decoder : q::util::Noncopyable
{
public:
    Video_Decoder();
    virtual ~Video_Decoder();

    enum class Format
    {
        RGBA,
        BGRA
    };

    auto decode_frame(silk::stream::gs::Video::Sample const& frame, math::vec2u32 const& size, std::vector<uint8_t>& out_data, Format format) -> bool;

private:
    static bool s_codecs_registered;

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

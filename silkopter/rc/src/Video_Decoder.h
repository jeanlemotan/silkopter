#pragma once

#include "common/stream/IVideo.h"
#include <memory>
#include <QtGui/QOpenGLTexture>

class Video_Decoder
{
public:
    Video_Decoder();
    ~Video_Decoder();


    bool init();
    bool decode_samples(std::vector<silk::stream::IVideo::Sample> const& samples);

    QOpenGLTexture* get_video_texture();

    struct Impl;

private:
    bool create_components();

    std::unique_ptr<Impl> m_impl;
    QOpenGLTexture* m_video_texture = nullptr;
};

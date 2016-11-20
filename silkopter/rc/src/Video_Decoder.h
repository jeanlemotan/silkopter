//#pragma once

//#include "common/stream/IVideo.h"
//#include <memory>
//#include <QOpenGLFunctions>
//#include <QOffscreenSurface>


//class Video_Decoder
//{
//public:
//    Video_Decoder();
//    ~Video_Decoder();


//    bool decode_data(std::vector<uint8_t> const& data, math::vec2u16 const& resolution);

//    bool init();

//    void release_buffers();

//    uint32_t get_video_texture_id();

//    struct Impl;

//private:
//    bool create_components(math::vec2u16 const& resolution);
//    void process_output();

//    uint32_t m_video_texture = 0;
//    std::unique_ptr<Impl> m_impl;
//    math::vec2u16 m_resolution;
//};

#pragma once

#include "IStream_Viewer_Widget.h"
#include "video/Video_Decoder.h"
#include "boost/signals2.hpp"

#include <QPainter>

namespace silk
{
class Comms;
}

class Video_Stream_Viewer_Widget : public IStream_Viewer_Widget
{
public:
    Video_Stream_Viewer_Widget(QWidget* parent);
    ~Video_Stream_Viewer_Widget();

    void init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type) override;

private:
    void paintEvent(QPaintEvent* ev);

    silk::Comms* m_comms = nullptr;
    std::string m_stream_path;
    uint32_t m_stream_rate;
    silk::stream::Type m_stream_type;

    boost::signals2::scoped_connection m_connection;

    Video_Decoder m_decoder;
    QPainter m_painter;
    QImage m_image;
    QImage m_image_flipped;
    std::vector<uint8_t> m_data;
};

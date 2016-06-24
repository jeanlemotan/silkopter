#pragma once

#include "HAL.h"
#include "Comms.h"
#include "Video_Decoder.h"

class Video_Viewer : public QWidget
{
public:
    Video_Viewer(QWidget *parent);
    virtual ~Video_Viewer();
    void add_sample(silk::stream::gs::Video::Sample const& sample);

private:
    void paintEvent(QPaintEvent* ev);

    Video_Decoder m_decoder;
    QPainter m_painter;
    QImage m_image;
    QImage m_image_flipped;
    std::vector<uint8_t> m_data;
};

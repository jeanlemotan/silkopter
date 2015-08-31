#include "Video_Viewer.h"
#include "HAL.h"
#include "Comms.h"

Video_Viewer::Video_Viewer(QWidget *parent)
    : QWidget(parent)
{

}

Video_Viewer::~Video_Viewer()
{
}


void Video_Viewer::add_sample(silk::stream::gs::Video::Sample const& sample)
{
    float ar = (float)sample.value.resolution.x / sample.value.resolution.y;
    int img_w = math::max(16, width());
    int img_h = img_w / ar;
    if (img_h > height())
    {
        img_h = height();
        img_w = img_h * ar;
    }

    if (m_decoder.decode_frame(sample, math::vec2u32(img_w, img_h), m_data))
    {
        m_image = QImage(m_data.data(), img_w, img_h, QImage::Format_ARGB32_Premultiplied);
        m_image_flipped = m_image;//.mirrored(false, false);
        update();
    }
}

void Video_Viewer::paintEvent(QPaintEvent* ev)
{
    m_painter.begin(this);
    m_painter.setCompositionMode(QPainter::CompositionMode_Source);
    m_painter.drawImage(QRectF(0, 0, m_image.width(), m_image.height()), m_image_flipped);
    m_painter.end();
}


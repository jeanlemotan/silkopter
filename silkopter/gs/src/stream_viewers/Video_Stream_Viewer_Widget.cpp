#include "Video_Stream_Viewer_Widget.h"
#include "Comms.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "Numeric_Viewer_Widget.h"

using namespace QtCharts;

Video_Stream_Viewer_Widget::Video_Stream_Viewer_Widget(QWidget* parent)
{
    setParent(parent);
}

Video_Stream_Viewer_Widget::~Video_Stream_Viewer_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, false);
    if (result != ts::success)
    {
        QLOGE("Failed to disable stream '{}' telemetry: {}", m_stream_path, result.error().what());
    }
}

void Video_Stream_Viewer_Widget::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    Numeric_Viewer_Widget* stats_widget = new Numeric_Viewer_Widget(nullptr);
    stats_widget->show();
    stats_widget->init("x", m_stream_rate);

    stats_widget->add_graph("Frame Size", "KB", QColor(0xe74c3c));

    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    QASSERT(result == ts::success);

    m_connection = m_comms->sig_telemetry_samples_available.connect([this, stats_widget](silk::Comms::ITelemetry_Stream const& _stream)
    {
        if (_stream.stream_path == m_stream_path)
        {
            auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IVideo> const*>(&_stream);
            if (stream)
            {
                for (silk::stream::IVideo::Sample const& sample: stream->samples)
                {
                    float values[1] = { static_cast<float>(sample.value.data.size()) / 1024.f };
                    stats_widget->add_samples(values, true);

                    float ar = (float)sample.value.resolution.x / sample.value.resolution.y;
                    int img_w = math::max(16, width());
                    int img_h = img_w / ar;
                    if (img_h > height())
                    {
                        img_h = height();
                        img_w = img_h * ar;
                    }

                    if (m_decoder.decode_frame(sample.value, math::vec2u32(img_w, img_h), m_data, Video_Decoder::Format::BGRA))
                    {
                        m_image = QImage(m_data.data(), img_w, img_h, QImage::Format_ARGB32_Premultiplied);
                        m_image_flipped = m_image;//.mirrored(false, false);
                        update();
                    }
                }
            }
        }
    });
}

void Video_Stream_Viewer_Widget::paintEvent(QPaintEvent* ev)
{
    m_painter.begin(this);
    m_painter.setCompositionMode(QPainter::CompositionMode_Source);
    m_painter.drawImage(QRectF(0, 0, m_image.width(), m_image.height()), m_image_flipped);
    m_painter.end();
}

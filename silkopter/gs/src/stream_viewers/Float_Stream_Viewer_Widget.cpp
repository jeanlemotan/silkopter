#include "Float_Stream_Viewer_Widget.h"
#include "Comms.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "Numeric_Viewer_Widget.h"

using namespace QtCharts;

Float_Stream_Viewer_Widget::Float_Stream_Viewer_Widget(QWidget* parent)
{
    setParent(parent);
}

Float_Stream_Viewer_Widget::~Float_Stream_Viewer_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, false);
    if (result != ts::success)
    {
        QLOGE("Failed to disable stream '{}' telemetry: {}", m_stream_path, result.error().what());
    }
}

void Float_Stream_Viewer_Widget::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    QASSERT(result == ts::success);

    Numeric_Viewer_Widget* widget = new Numeric_Viewer_Widget(this);
    widget->init("x", m_stream_rate);

    widget->add_graph("value", "", Qt::red);

    setLayout(new QVBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(widget);

    m_connection = m_comms->sig_telemetry_samples_available.connect([this, widget](silk::Comms::ITelemetry_Stream const& _stream)
    {
        if (_stream.stream_path == m_stream_path)
        {
            auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IFloat> const*>(&_stream);
            if (stream)
            {
                for (auto const& sample: stream->samples)
                {
                    float values[1] = { sample.value };
                    widget->add_samples(values, sample.is_healthy);
                }
            }
        }
    });
}

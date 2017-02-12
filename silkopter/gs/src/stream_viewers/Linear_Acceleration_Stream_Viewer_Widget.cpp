#include "Linear_Acceleration_Stream_Viewer_Widget.h"
#include "Comms.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "Numeric_Viewer_Widget.h"

using namespace QtCharts;

Linear_Acceleration_Stream_Viewer_Widget::Linear_Acceleration_Stream_Viewer_Widget(QWidget* parent)
{
    setParent(parent);
}

Linear_Acceleration_Stream_Viewer_Widget::~Linear_Acceleration_Stream_Viewer_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, false);
    if (result != ts::success)
    {
        QLOGE("Failed to disable stream '{}' telemetry: {}", m_stream_path, result.error().what());
    }
}

void Linear_Acceleration_Stream_Viewer_Widget::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    QASSERT(result == ts::success);

    Numeric_Viewer_Widget* widget = new Numeric_Viewer_Widget(this);
    widget->init("x", m_stream_rate);

    widget->add_graph("x", "m/s^2", Qt::red);
    widget->add_graph("y", "m/s^2", Qt::green);
    widget->add_graph("z", "m/s^2", Qt::blue);

    setLayout(new QVBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(widget);

    m_connection = m_comms->sig_telemetry_samples_available.connect([this, widget](silk::Comms::ITelemetry_Stream const& _stream)
    {
        if (_stream.stream_path == m_stream_path)
        {
            if (auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::ILinear_Acceleration> const*>(&_stream))
            {
                for (auto const& sample: stream->samples)
                {
                    float values[3] = { sample.value.x, sample.value.y, sample.value.z };
                    widget->add_samples(values, sample.is_healthy);
                }
            }
            else if (auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IECEF_Linear_Acceleration> const*>(&_stream))
            {
                for (auto const& sample: stream->samples)
                {
                    float values[3] = { sample.value.x, sample.value.y, sample.value.z };
                    widget->add_samples(values, sample.is_healthy);
                }
            }
            else if (auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IENU_Linear_Acceleration> const*>(&_stream))
            {
                for (auto const& sample: stream->samples)
                {
                    float values[3] = { sample.value.x, sample.value.y, sample.value.z };
                    widget->add_samples(values, sample.is_healthy);
                }
            }
        }
    });
}

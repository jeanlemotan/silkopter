#include "Angular_Velocity_Stream_Viewer_Widget.h"
#include "Comms.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "Numeric_Viewer_Widget.h"

using namespace QtCharts;

Angular_Velocity_Stream_Viewer_Widget::Angular_Velocity_Stream_Viewer_Widget(QWidget* parent)
{
    setParent(parent);
}

Angular_Velocity_Stream_Viewer_Widget::~Angular_Velocity_Stream_Viewer_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, false);
    QASSERT(result == ts::success);
}

void Angular_Velocity_Stream_Viewer_Widget::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    QASSERT(result == ts::success);

    Numeric_Viewer_Widget* widget = new Numeric_Viewer_Widget(this);
    widget->init("x", m_stream_rate);

    widget->add_graph("x", "°/s", Qt::red);
    widget->add_graph("y", "°/s", Qt::green);
    widget->add_graph("z", "°/s", Qt::blue);

    setLayout(new QVBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(widget);

    m_connection = m_comms->sig_telemetry_samples_available.connect([this, widget](silk::Comms::ITelemetry_Stream const& _stream)
    {
        if (_stream.stream_path == m_stream_path)
        {
            auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IAngular_Velocity> const*>(&_stream);
            if (stream)
            {
                for (auto const& sample: stream->samples)
                {
                    float values[3] = { math::degrees(sample.value.x), math::degrees(sample.value.y), math::degrees(sample.value.z) };
                    widget->add_samples(values, sample.is_healthy);
                }

                widget->process();
            }
        }
    });
}

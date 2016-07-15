#include "Acceleration_Stream_Viewer_Widget.h"
#include "Comms.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

using namespace QtCharts;

Acceleration_Stream_Viewer_Widget::Acceleration_Stream_Viewer_Widget(QWidget* parent)
{
    setParent(parent);
    m_ui.setupUi(this);
}

Acceleration_Stream_Viewer_Widget::~Acceleration_Stream_Viewer_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, false);
    QASSERT(result == ts::success);
}

void Acceleration_Stream_Viewer_Widget::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    QASSERT(result == ts::success);

    QLineSeries* xseries = new QLineSeries();
    xseries->setUseOpenGL(true);
    QLineSeries* yseries = new QLineSeries();
    yseries->setUseOpenGL(true);
    QLineSeries* zseries = new QLineSeries();
    zseries->setUseOpenGL(true);

    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(xseries);
    chart->addSeries(yseries);
    chart->addSeries(zseries);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout()->addWidget(chartView);

    //m_start_time_s = std::chrono::duration<float>(std::chrono::high_resolution_clock::now().tim).count();

    m_connection = m_comms->sig_telemetry_samples_available.connect([this, chart, xseries, yseries, zseries](silk::Comms::ITelemetry_Stream const& _stream)
    {
        if (_stream.stream_path == m_stream_path)
        {
            auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IAcceleration> const*>(&_stream);
            if (stream)
            {
                for (auto const& sample: stream->samples)
                {
                    xseries->append(m_start_time_s, sample.value.x);
                    yseries->append(m_start_time_s, sample.value.y);
                    zseries->append(m_start_time_s, sample.value.z);
                    m_start_time_s += 1.f / m_stream_rate;
                }
                chart->axisX()->setRange(std::max(0.f, m_start_time_s - 10.f), m_start_time_s);
                chart->axisY()->setRange(-0.1, 0.1);
            }
        }
    });
}

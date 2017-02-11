#include "Position_Stream_Viewer_Widget.h"
#include "Comms.h"

#include <QQuickWidget>
#include <QQuickItem>
//#include <QtCharts/QChartView>
//#include <QtCharts/QLineSeries>
#include <QtPositioning/QGeoCoordinate>
#include "Numeric_Viewer_Widget.h"

using namespace QtCharts;

Position_Stream_Viewer_Widget::Position_Stream_Viewer_Widget(QWidget* parent)
{
    setParent(parent);
}

Position_Stream_Viewer_Widget::~Position_Stream_Viewer_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, false);
    if (result != ts::success)
    {
        QLOGE("Failed to disable stream '{}' telemetry: {}", m_stream_path, result.error().what());
    }
}

void Position_Stream_Viewer_Widget::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    auto result = m_comms->set_stream_telemetry_enabled(m_stream_path, true);
    QASSERT(result == ts::success);

    QQuickWidget* quick_view = new QQuickWidget(this);
    quick_view->setSource(QUrl("qrc:/qml/map.qml"));
    quick_view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quick_view->show();

    QQuickItem* root = quick_view->rootObject();
    Q_ASSERT(root);

    setLayout(new QHBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(quick_view);

    m_connection = m_comms->sig_telemetry_samples_available.connect([this, root](silk::Comms::ITelemetry_Stream const& _stream)
    {
        if (_stream.stream_path == m_stream_path)
        {
            auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IECEF_Position> const*>(&_stream);
            if (stream)
            {
                for (auto const& sample: stream->samples)
                {
                    if (root)
                    {
                        util::coordinates::LLA lla = util::coordinates::ecef_to_lla(sample.value);
                        root->setProperty("locationLatitude", math::degrees(lla.latitude));
                        root->setProperty("locationLongitude", math::degrees(lla.longitude));
                        root->setProperty("locationAltitude", lla.altitude);
                    }

            //                    float values[3] = { sample.value.x, sample.value.y, sample.value.z };
            //                    widget->add_samples(values, sample.is_healthy);
                }

            //                widget->process();
            }
        }
    });
}

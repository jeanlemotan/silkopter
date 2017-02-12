#include "Internal_Telemetry_Widget.h"
#include "Comms.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "stream_viewers/Numeric_Viewer_Widget.h"

using namespace QtCharts;

Internal_Telemetry_Widget::Internal_Telemetry_Widget(QWidget* parent)
{
    setParent(parent);
}

Internal_Telemetry_Widget::~Internal_Telemetry_Widget()
{
    auto result = m_comms->set_stream_telemetry_enabled("#hal", false);
    if (result != ts::success)
    {
        QLOGE("Failed to disable internal stream telemetry: {}", result.error().what());
    }
}

void Internal_Telemetry_Widget::init(silk::Comms& comms, std::vector<std::string> const& node_names)
{
    m_comms = &comms;

    auto result = m_comms->set_stream_telemetry_enabled("#hal", true);
    QASSERT_MSG(result == ts::success, "{}", result.error().what());

    m_node_count = node_names.size();

    struct RGB { uint8_t r, g, b; };
    std::array<RGB, 26> palette =
    {{
        {240,163,255},
        {0,117,220},
        {153,63,0},
        {76,0,92},
        {25,25,25},
        {0,92,49},
        {43,206,72},
        {255,204,153},
        {128,128,128},
        {148,255,181},
        {143,124,0},
        {157,204,0},
        {194,0,136},
        {0,51,128},
        {255,164,5},
        {255,168,187},
        {66,102,0},
        {255,0,16},
        {94,241,242},
        {0,153,143},
        {224,255,102},
        {116,10,255},
        {153,0,0},
        {255,255,128},
        {255,255,0},
        {255,80,5}
    }};

    Numeric_Viewer_Widget* widget = new Numeric_Viewer_Widget(this);
    widget->init("average", 10, false);

    Numeric_Viewer_Widget* max_widget = new Numeric_Viewer_Widget(this);
    max_widget->init("max", 10, false);

    uint32_t index = 0;
    for (std::string const& node_name: node_names)
    {
        RGB rgb = palette[index % palette.size()];
        widget->add_graph(node_name, "%", QColor(rgb.r, rgb.g, rgb.b));
        max_widget->add_graph(node_name, "s", QColor(rgb.r, rgb.g, rgb.b));
        m_node_indices[node_name] = index;
        index++;
    }

    setLayout(new QVBoxLayout());
    layout()->setMargin(0);
    layout()->addWidget(widget);
    layout()->addWidget(max_widget);

    m_connection = m_comms->sig_internal_telemetry_samples_available.connect([this, widget, max_widget](std::vector<silk::Comms::Internal_Telementry_Sample> const& samples)
    {
        for (silk::Comms::Internal_Telementry_Sample const& sample: samples)
        {
            m_data.clear(); //to rest everything to 0
            m_data.resize(std::min(m_node_count, sample.nodes.size()));
            for (size_t i = 0; i < sample.nodes.size(); i++)
            {
                auto it = m_node_indices.find(sample.nodes[i].name);
                if (it != m_node_indices.end())
                {
                    //convet the duration per second to a percent
                    m_data[it->second] = std::chrono::duration<float>(sample.nodes[i].duration).count() * 100.f;
                }
            }
            widget->add_samples(m_data.data(), true);

            m_data.clear(); //to rest everything to 0
            m_data.resize(std::min(m_node_count, sample.nodes.size()));
            for (size_t i = 0; i < sample.nodes.size(); i++)
            {
                auto it = m_node_indices.find(sample.nodes[i].name);
                if (it != m_node_indices.end())
                {
                    m_data[it->second] = std::chrono::duration_cast<std::chrono::microseconds>(sample.nodes[i].max_duration).count() / 1000000.f;
                }
            }
            max_widget->add_samples(m_data.data(), true);
        }
    });
}

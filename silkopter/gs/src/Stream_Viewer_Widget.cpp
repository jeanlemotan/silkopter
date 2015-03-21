#include "Stream_Viewer_Widget.h"

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/processor/ILPF.h"
#include "common/node/processor/IResampler.h"
#include "common/node/processor/IPilot.h"

#include "Numeric_Viewer.h"
//#include "Battery_State_Viewer.h"
//#include "Cardinal_Points_Viewer.h"
#include "Map_Viewer.h"
//#include "Reference_Frame_Stream_Viewer.h"
//#include "Video_Stream_Viewer.h"
//#include "Generic_Stream_Viewer.h"


Stream_Viewer_Widget::Stream_Viewer_Widget(silk::HAL& hal, silk::Comms& comms, QWidget *parent)
    : QWidget(parent)
    , m_hal(hal)
    , m_comms(comms)
{
    setLayout(new QVBoxLayout(this));
    setAttribute(Qt::WA_DeleteOnClose);
}

Stream_Viewer_Widget::~Stream_Viewer_Widget()
{
    m_hal.set_stream_telemetry_active(m_stream_name, false, [](silk::HAL::Result) {});
}

void Stream_Viewer_Widget::set_stream_name(std::string const& stream_name)
{
    if (!m_stream_name.empty())
    {
        m_hal.set_stream_telemetry_active(m_stream_name, false, [](silk::HAL::Result) {});
        m_stream_name.clear();
    }

    auto label = new QLabel(this);
    layout()->addWidget(label);

    auto stream = m_hal.get_streams().find_by_name(stream_name);
    if (!stream)
    {
        label->setText(q::util::format2<std::string>("Cannot find Stream {}", stream_name).c_str());
        return;
    }

    m_stream = stream;
    m_stream_name = stream_name;
    label->setText(q::util::format2<std::string>("Activating {}...", m_stream_name).c_str());

    m_hal.set_stream_telemetry_active(m_stream_name, true, [this, label](silk::HAL::Result result)
    {
        if (result == silk::HAL::Result::FAILED)
        {
            label->setText("Failed");
        }
        else if (result == silk::HAL::Result::TIMEOUT)
        {
            label->setText("Timeout");
        }
        else
        {
            delete label;
            create_viewer();
        }
    });
}

void Stream_Viewer_Widget::create_viewer()
{
    using namespace silk::node::stream;

    //removing all widgets
    setLayout(new QVBoxLayout(this));

    auto stream = m_stream.lock();
    if (!stream)
    {
        auto label = new QLabel(this);
        layout()->addWidget(label);
        label->setText(q::util::format2<std::string>("Cannot find Stream {}", m_stream_name).c_str());
        return;
    }

    auto type = stream->type;
    if (type == IAcceleration::TYPE)
    {
        auto viewer = new Numeric_Viewer("m/s^2", stream->rate, this);
        viewer->add_graph("X", "m/s^2", QColor(0xe74c3c));
        viewer->add_graph("Y", "m/s^2", QColor(0x2ecc71));
        viewer->add_graph("Z", "m/s^2", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Acceleration>(stream)->samples_available_signal.connect([this, viewer](Acceleration& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IAngular_Velocity::TYPE)
    {
        auto viewer = new Numeric_Viewer("°/s", stream->rate, this);
        viewer->add_graph("X", "°/s", QColor(0xe74c3c));
        viewer->add_graph("Y", "°/s", QColor(0x2ecc71));
        viewer->add_graph("Z", "°/s", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Angular_Velocity>(stream)->samples_available_signal.connect([this, viewer](Angular_Velocity& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IMagnetic_Field::TYPE)
    {
        auto viewer = new Numeric_Viewer("Teslas", stream->rate, this);
        viewer->add_graph("X", "Teslas", QColor(0xe74c3c));
        viewer->add_graph("Y", "Teslas", QColor(0x2ecc71));
        viewer->add_graph("Z", "Teslas", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Magnetic_Field>(stream)->samples_available_signal.connect([this, viewer](Magnetic_Field& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IPressure::TYPE)
    {
        auto viewer = new Numeric_Viewer("Pascals", stream->rate, this);
        viewer->add_graph("Pressure", "Pascals", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Pressure>(stream)->samples_available_signal.connect([this, viewer](Pressure& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IBattery_State::TYPE)
    {
//        layout()->addWidget(new Battery_State_Viewer(stream, this));
    }
    else if (type == ILinear_Acceleration::TYPE)
    {
        auto viewer = new Numeric_Viewer("m/s^2", stream->rate, this);
        viewer->add_graph("X", "m/s^2", QColor(0xe74c3c));
        viewer->add_graph("Y", "m/s^2", QColor(0x2ecc71));
        viewer->add_graph("Z", "m/s^2", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Linear_Acceleration>(stream)->samples_available_signal.connect([this, viewer](Linear_Acceleration& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == ICurrent::TYPE)
    {
        auto viewer = new Numeric_Viewer("A", stream->rate, this);
        viewer->add_graph("Current", "A", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Current>(stream)->samples_available_signal.connect([this, viewer](Current& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IVoltage::TYPE)
    {
        auto viewer = new Numeric_Viewer("V", stream->rate, this);
        viewer->add_graph("Voltage", "V", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Voltage>(stream)->samples_available_signal.connect([this, viewer](Voltage& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IDistance::TYPE)
    {
        auto viewer = new Numeric_Viewer("m", stream->rate, this);
        viewer->add_graph("Distance", "m", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Distance>(stream)->samples_available_signal.connect([this, viewer](Distance& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { math::length(s.value) };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IECEF_Location::TYPE)
    {
        auto viewer = new Map_Viewer(this);
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<ECEF_Location>(stream)->samples_available_signal.connect([this, viewer](ECEF_Location& stream)
        {
            for (auto const& s: stream.samples)
            {
                viewer->add_sample(s.tp, s.value.position, s.value.position_accuracy);
            }
            viewer->process();
        });
    }
//    else if (type == IECEF::TYPE)
//    {
//        auto viewer = new Map_Viewer(this);
//        layout()->addWidget(viewer);
//        m_connection = std::static_pointer_cast<ECEF>(stream)->samples_available_signal.connect([this, viewer](ECEF& stream)
//        {
//            for (auto const& s: stream.samples)
//            {
//                viewer->add_sample(s.tp, s.value.lat_lon);
//            }
//            viewer->process();
//        });
//    }
    else if (type == IPWM::TYPE)
    {
        auto viewer = new Numeric_Viewer("pwm", stream->rate, this);
        viewer->add_graph("PWM", "pwm", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<PWM>(stream)->samples_available_signal.connect([this, viewer](PWM& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IFrame::TYPE)
    {
//        layout()->addWidget(new Reference_Frame_Stream_Viewer(stream, this));
    }
    else if (type == ITemperature::TYPE)
    {
        auto viewer = new Numeric_Viewer("°C", stream->rate, this);
        viewer->add_graph("Temperature", "°C", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Temperature>(stream)->samples_available_signal.connect([this, viewer](Temperature& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IADC::TYPE)
    {
        auto viewer = new Numeric_Viewer("adc", stream->rate, this);
        viewer->add_graph("ADC", "adc", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<ADC>(stream)->samples_available_signal.connect([this, viewer](ADC& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data());
            }
            viewer->process();
        });
    }
    else if (type == IVideo::TYPE)
    {
//        layout()->addWidget(new Video_Stream_Viewer(stream, this));
    }
    else
    {
//        layout()->addWidget(new Generic_Stream_Viewer(this));
    }
}


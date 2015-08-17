#include "Stream_Viewer_Widget.h"

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/ILPF.h"
#include "common/node/IResampler.h"
#include "common/node/IPilot.h"

#include "Numeric_Viewer.h"
//#include "Battery_State_Viewer.h"
//#include "Cardinal_Points_Viewer.h"
#include "Map_Viewer.h"
//#include "Reference_Frame_Stream_Viewer.h"
#include "Video_Viewer.h"
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
    m_hal.set_stream_telemetry_active(m_stream_name, false);
}

void Stream_Viewer_Widget::set_stream_name(std::string const& stream_name)
{
    if (!m_stream_name.empty())
    {
        m_hal.set_stream_telemetry_active(m_stream_name, false);
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

    m_hal.set_stream_telemetry_active(m_stream_name, true);

    delete label;
    create_viewer();
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IBattery_State::TYPE)
    {
        auto viewer = new Numeric_Viewer("Battery", stream->rate, this);
        viewer->add_graph("Charge Used", "Ah", QColor(0xe74c3c));
        viewer->add_graph("Capacity Left", "%", QColor(0x2ecc71));
        viewer->add_graph("Average Current", "A", QColor(0x3498db));
        viewer->add_graph("Average Voltage", "V", QColor(0x9834db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Battery_State>(stream)->samples_available_signal.connect([this, viewer](Battery_State& stream)
        {
            std::array<double, 4> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.charge_used, s.value.capacity_left, s.value.average_current, s.value.average_voltage };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == ITorque::TYPE)
    {
        auto viewer = new Numeric_Viewer("Nm", stream->rate, this);
        viewer->add_graph("X", "Nm", QColor(0xe74c3c));
        viewer->add_graph("Y", "Nm", QColor(0x2ecc71));
        viewer->add_graph("Z", "Nm", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Torque>(stream)->samples_available_signal.connect([this, viewer](Torque& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IForce::TYPE)
    {
        auto viewer = new Numeric_Viewer("N", stream->rate, this);
        viewer->add_graph("X", "N", QColor(0xe74c3c));
        viewer->add_graph("Y", "N", QColor(0x2ecc71));
        viewer->add_graph("Z", "N", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Torque>(stream)->samples_available_signal.connect([this, viewer](Torque& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IVelocity::TYPE)
    {
        auto viewer = new Numeric_Viewer("m/s", stream->rate, this);
        viewer->add_graph("X", "m/s", QColor(0xe74c3c));
        viewer->add_graph("Y", "m/s", QColor(0x2ecc71));
        viewer->add_graph("Z", "m/s", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Velocity>(stream)->samples_available_signal.connect([this, viewer](Velocity& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.x, s.value.y, s.value.z };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IThrottle::TYPE)
    {
        auto viewer = new Numeric_Viewer("%", stream->rate, this);
        viewer->add_graph("Throttle", "%", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Throttle>(stream)->samples_available_signal.connect([this, viewer](Throttle& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value * 100.0 };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IECEF_Position::TYPE)
    {
        auto viewer = new Map_Viewer(this);
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<ECEF_Position>(stream)->samples_available_signal.connect([this, viewer](ECEF_Position& stream)
        {
            for (auto const& s: stream.samples)
            {
                viewer->add_sample(s.tp, s.value);
            }
            viewer->process();
        });
    }
    else if (type == IGPS_Info::TYPE)
    {
        auto viewer = new Numeric_Viewer("Info", stream->rate, this);
        viewer->add_graph("Sats", "#", QColor(0xe74c3c));
        viewer->add_graph("PDOP", "m", QColor(0x2ecc71));
        viewer->add_graph("PAcc", "m", QColor(0x3498db));
        viewer->add_graph("VAcc", "m/s", QColor(0x9834db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<GPS_Info>(stream)->samples_available_signal.connect([this, viewer](GPS_Info& stream)
        {
            std::array<double, 4> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value.fix_satellites, s.value.pdop, s.value.position_accuracy, s.value.velocity_accuracy };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IFloat::TYPE)
    {
        auto viewer = new Numeric_Viewer("f", stream->rate, this);
        viewer->add_graph("Float", "f", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Float>(stream)->samples_available_signal.connect([this, viewer](Float& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IBool::TYPE)
    {
        auto viewer = new Numeric_Viewer("v", stream->rate, this);
        viewer->add_graph("Bool", "v", QColor(0xe74c3c));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Bool>(stream)->samples_available_signal.connect([this, viewer](Bool& stream)
        {
            std::array<double, 1> data;
            for (auto const& s: stream.samples)
            {
                data = { s.value ? 1.0 : 0.0 };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IFrame::TYPE)
    {
        auto viewer = new Numeric_Viewer("Frame", stream->rate, this);
        viewer->add_graph("X", "", QColor(0xe74c3c));
        viewer->add_graph("Y", "", QColor(0x2ecc71));
        viewer->add_graph("Z", "", QColor(0x3498db));
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Frame>(stream)->samples_available_signal.connect([this, viewer](Frame& stream)
        {
            std::array<double, 3> data;
            for (auto const& s: stream.samples)
            {
                math::mat3f mat = s.value.rotation.get_as_mat3();
                data = { math::dot(mat.get_axis_x(), math::vec3f(1, 0, 0)), math::dot(mat.get_axis_y(), math::vec3f(0, 1, 0)), math::dot(mat.get_axis_z(), math::vec3f(0, 0, 1)) };
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
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
                viewer->add_samples(s.tp, data.data(), s.is_healthy);
            }
            viewer->process();
        });
    }
    else if (type == IVideo::TYPE)
    {
        auto viewer = new Video_Viewer(this);
        layout()->addWidget(viewer);
        m_connection = std::static_pointer_cast<Video>(stream)->samples_available_signal.connect([this, viewer](Video& stream)
        {
            for (auto const& s: stream.samples)
            {
                viewer->add_sample(s);
            }
        });
    }
    else
    {
        QASSERT(0);
//        layout()->addWidget(new Generic_Stream_Viewer(this));
    }
}


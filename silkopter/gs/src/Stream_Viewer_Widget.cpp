#include "stdafx.h"
#include "Stream_Viewer_Widget.h"

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/processor/ILPF.h"
#include "common/node/processor/IResampler.h"
#include "common/node/processor/IMultirotor_Pilot.h"

//#include "Vec3_Stream_Viewer.h"
#include "Float_Stream_Viewer.h"
//#include "Battery_State_Viewer.h"
//#include "Cardinal_Points_Viewer.h"
//#include "Location_Stream_Viewer.h"
//#include "Reference_Frame_Stream_Viewer.h"
//#include "Video_Stream_Viewer.h"
//#include "Generic_Stream_Viewer.h"


Stream_Viewer_Widget::Stream_Viewer_Widget(silk::HAL& hal, silk::Comms& comms, QWidget *parent)
    : QWidget(parent)
    , m_hal(hal)
    , m_comms(comms)
{
    setLayout(new QVBoxLayout(this));
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

    auto class_id = stream->class_id;
    if (class_id == q::rtti::get_class_id<IAcceleration>())
    {
//        layout()->addWidget(new Vec3_Stream_Viewer("m/s^2", stream, this));
    }
    else if (class_id == q::rtti::get_class_id<IAngular_Velocity>())
    {
//        layout()->addWidget(new Vec3_Stream_Viewer("°/s", stream, this));
    }
    else if (class_id == q::rtti::get_class_id<IMagnetic_Field>())
    {
//        layout()->addWidget(new Vec3_Stream_Viewer("T", stream, this));
    }
    else if (class_id == q::rtti::get_class_id<IPressure>())
    {
        layout()->addWidget(new Float_Stream_Viewer<Pressure>("P", std::static_pointer_cast<Pressure>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<IBattery_State>())
    {
//        layout()->addWidget(new Battery_State_Viewer(stream, this));
    }
    else if (class_id == q::rtti::get_class_id<ILinear_Acceleration>())
    {
//        layout()->addWidget(new Vec3_Stream_Viewer("m/s^2", stream, this));
    }
    else if (class_id == q::rtti::get_class_id<ICardinal_Points>())
    {
//        layout()->addWidget(new Cardinal_Points_Viewer(stream, this));
    }
    else if (class_id == q::rtti::get_class_id<ICurrent>())
    {
        layout()->addWidget(new Float_Stream_Viewer<Current>("A", std::static_pointer_cast<Current>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<IVoltage>())
    {
        layout()->addWidget(new Float_Stream_Viewer<Voltage>("V", std::static_pointer_cast<Voltage>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<IDistance>())
    {
        layout()->addWidget(new Float_Stream_Viewer<Distance>("m", std::static_pointer_cast<Distance>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<ILocation>())
    {
//        layout()->addWidget(new Location_Stream_Viewer(stream, this));
    }
    else if (class_id == q::rtti::get_class_id<IPWM_Value>())
    {
        layout()->addWidget(new Float_Stream_Viewer<PWM_Value>("PWM", std::static_pointer_cast<PWM_Value>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<IReference_Frame>())
    {
//        layout()->addWidget(new Reference_Frame_Stream_Viewer(stream, this));
    }
    else if (class_id == q::rtti::get_class_id<ITemperature>())
    {
        layout()->addWidget(new Float_Stream_Viewer<Temperature>("°C", std::static_pointer_cast<Temperature>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<IADC_Value>())
    {
        layout()->addWidget(new Float_Stream_Viewer<ADC_Value>("ADC", std::static_pointer_cast<ADC_Value>(stream), this));
    }
    else if (class_id == q::rtti::get_class_id<IVideo>())
    {
//        layout()->addWidget(new Video_Stream_Viewer(stream, this));
    }
    else
    {
//        layout()->addWidget(new Generic_Stream_Viewer(this));
    }
}


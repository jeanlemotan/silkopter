#include "stdafx.h"
#include "Stream_Viewer_Widget.h"

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/processor/ILPF.h"
#include "common/node/processor/IResampler.h"
#include "common/node/processor/IMultirotor_Pilot.h"

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
    m_hal.set_stream_telemetry_active(m_stream_name, false, [](silk::HAL::Result) {});

    m_stream_name = stream_name;

    auto label = new QLabel("Activating stream...", this);
    layout()->addWidget(label);

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
    //removing all widgets
    setLayout(new QVBoxLayout(this));
}


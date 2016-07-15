#include "Stream_Viewer_Window.h"
#include "Comms.h"

#include "stream_viewers/Acceleration_Stream_Viewer_Widget.h"
#include "stream_viewers/ADC_Stream_Viewer_Widget.h"
#include "stream_viewers/Angular_Velocity_Stream_Viewer_Widget.h"
#include "stream_viewers/Battery_State_Stream_Viewer_Widget.h"
#include "stream_viewers/Bool_Stream_Viewer_Widget.h"
#include "stream_viewers/Current_Stream_Viewer_Widget.h"
#include "stream_viewers/Distance_Stream_Viewer_Widget.h"
#include "stream_viewers/Float_Stream_Viewer_Widget.h"
#include "stream_viewers/Force_Stream_Viewer_Widget.h"
#include "stream_viewers/Frame_Stream_Viewer_Widget.h"
#include "stream_viewers/GPS_Info_Stream_Viewer_Widget.h"
#include "stream_viewers/Linear_Acceleration_Stream_Viewer_Widget.h"
#include "stream_viewers/Magnetic_Field_Stream_Viewer_Widget.h"
#include "stream_viewers/Multirotor_Commands_Stream_Viewer_Widget.h"
#include "stream_viewers/Multirotor_State_Stream_Viewer_Widget.h"
#include "stream_viewers/Position_Stream_Viewer_Widget.h"
#include "stream_viewers/Pressure_Stream_Viewer_Widget.h"
#include "stream_viewers/Proximity_Stream_Viewer_Widget.h"
#include "stream_viewers/PWM_Stream_Viewer_Widget.h"
#include "stream_viewers/Temperature_Stream_Viewer_Widget.h"
#include "stream_viewers/Throttle_Stream_Viewer_Widget.h"
#include "stream_viewers/Torque_Stream_Viewer_Widget.h"
#include "stream_viewers/Velocity_Stream_Viewer_Widget.h"
#include "stream_viewers/Video_Stream_Viewer_Widget.h"
#include "stream_viewers/Voltage_Stream_Viewer_Widget.h"

Stream_Viewer_Window::Stream_Viewer_Window(QWidget* parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

void Stream_Viewer_Window::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    setWindowTitle(("Stream - " + m_stream_path).c_str());

    IStream_Viewer_Widget* viewer = nullptr;
    switch (m_stream_type.get_semantic())
    {
    case silk::stream::Semantic::ACCELERATION: viewer = new Acceleration_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::ADC: viewer = new ADC_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::ANGULAR_VELOCITY: viewer = new Angular_Velocity_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::BATTERY_STATE: viewer = new Battery_State_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::BOOL: viewer = new Bool_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::CURRENT: viewer = new Current_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::DISTANCE: viewer = new Distance_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::FLOAT: viewer = new Float_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::FORCE: viewer = new Force_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::FRAME: viewer = new Frame_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::GPS_INFO: viewer = new GPS_Info_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::LINEAR_ACCELERATION: viewer = new Linear_Acceleration_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::MAGNETIC_FIELD: viewer = new Magnetic_Field_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::MULTIROTOR_COMMANDS: viewer = new Multirotor_Commands_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::MULTIROTOR_STATE: viewer = new Multirotor_State_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::POSITION: viewer = new Position_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::PRESSURE: viewer = new Pressure_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::PROXIMITY: viewer = new Proximity_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::PWM: viewer = new PWM_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::TEMPERATURE: viewer = new Temperature_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::TORQUE: viewer = new Torque_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::THROTTLE: viewer = new Throttle_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::VELOCITY: viewer = new Velocity_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::VIDEO: viewer = new Video_Stream_Viewer_Widget(this); break;
    case silk::stream::Semantic::VOLTAGE: viewer = new Voltage_Stream_Viewer_Widget(this); break;
    }

    if (viewer)
    {
        viewer->init(comms, stream_path, stream_rate, stream_type);
        layout()->addWidget(viewer);
    }
}

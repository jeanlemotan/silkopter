#pragma once

#include <QMainWindow>
#include "ui_Simulator.h"
#include "simulator/Render_Widget.h"
#include "simulator/Camera_Controller_3D.h"

#include "Comms.h"
#include "messages.def.h"

#include "common/stream/IMultirotor_Simulator_State.h"

class Simulator : public QMainWindow
{
    Q_OBJECT
public:
    explicit Simulator(QWidget* parent = 0);

    void init(silk::Comms& comms, std::string const& node_name);

    void process();

private:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    Render_Context m_context;
    void init_graphics();

    void init_world();
    void message_received(std::string const& node_name, silk::messages::INode_Message const& message);
    void telemetry_received(silk::Comms::ITelemetry_Stream const& stream);

    void render_ground();
    void render_uav(math::trans3df const& trans);
    void render_brain_state();
    void render_world_axis();

    Ui::Simulator m_ui;

    silk::Comms* m_comms = nullptr;
    std::string m_node_name;
    std::string m_stream_path;

    boost::signals2::scoped_connection m_message_connection;
    boost::signals2::scoped_connection m_telemetry_connection;

    typedef silk::stream::IMultirotor_Simulator_State::Value UAV_State;
    UAV_State m_sim_state;

    Camera_Controller_3D m_camera_controller;
    math::vec3f m_camera_position_target;
};

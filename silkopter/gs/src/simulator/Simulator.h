#pragma once

#include <QMainWindow>
#include "ui_Simulator.h"

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>

#include "Comms.h"
#include "messages.def.h"

#include "common/stream/IMultirotor_Simulator_State.h"

class Simulator : public QMainWindow
{
    Q_OBJECT
public:
    explicit Simulator(QWidget* parent = 0);

    void init(silk::Comms& comms, std::string const& node_name);

Q_SIGNALS:

public Q_SLOTS:
    void process_logic(float);

private:
    void init_world();
    void create_axis(Qt3DCore::QEntity* parent);
    void message_received(std::string const& node_name, silk::messages::INode_Message const& message);
    void telemetry_received(silk::Comms::ITelemetry_Stream const& stream);

    Ui::Simulator m_ui;
    Qt3DExtras::Qt3DWindow* m_view = nullptr;
    Qt3DCore::QEntity* m_root_entity = nullptr;
    Qt3DRender::QCamera* m_camera_entity = nullptr;

    Qt3DCore::QEntity* m_uav_entity = nullptr;
    Qt3DCore::QTransform* m_uav_transform = nullptr;
    Qt3DCore::QEntity* m_ground_entity = nullptr;

    silk::Comms* m_comms = nullptr;
    std::string m_node_name;
    std::string m_stream_path;

    boost::signals2::scoped_connection m_message_connection;
    boost::signals2::scoped_connection m_telemetry_connection;

    typedef silk::stream::IMultirotor_Simulator_State::Value UAV_State;
    UAV_State m_state;
};

#pragma once

#include <QMainWindow>
#include "ui_Simulator.h"

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>

#include "Comms.h"
#include "messages.def.h"

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
    void message_received(std::string const& node_name, silk::messages::INode_Message const& message);

    Ui::Simulator m_ui;
    Qt3DExtras::Qt3DWindow* m_view = nullptr;
    Qt3DCore::QEntity* m_root_entity = nullptr;
    Qt3DRender::QCamera* m_camera_entity = nullptr;

    Qt3DCore::QEntity* m_uav_entity = nullptr;
    Qt3DCore::QTransform* m_uav_transform = nullptr;
    Qt3DCore::QEntity* m_ground_entity = nullptr;

    silk::Comms* m_comms = nullptr;
    std::string m_node_name;

    bool m_send_message = true;
    Clock::time_point m_last_message_send_tp = Clock::now();

    std::shared_ptr<silk::messages::Multirotor_Simulator_Get_State_Message> m_get_state_message;
    silk::messages::Multirotor_Simulation_State m_state;
};

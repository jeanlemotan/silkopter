#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include "ui_Sim_Window.h"

#include "Comms.h"
#include "Render_Widget.h"
#include "Camera_Controller_3D.h"
#include "common/node/processor/ISimulator.h"

class Sim_Window : public QMainWindow
{
public:
    explicit Sim_Window(silk::HAL& hal, silk::node::Node_ptr sim_node, silk::Comms& comms, Render_Context& context, QWidget* parent = 0);
    ~Sim_Window();

    void process();

private:
    void read_config();

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void contextMenu(QGraphicsSceneMouseEvent* event);

    Ui::Sim_Window m_ui;

    silk::HAL& m_hal;
    silk::node::Node_ptr m_sim_node;
    silk::Comms& m_comms;
    Render_Context& m_context;
    Camera_Controller_3D m_camera_controller;

    void render_uav();
    void render_ground();

    struct UAV
    {
        silk::node::ISimulator::UAV_Config config;
        silk::node::ISimulator::UAV_State state;
    } m_uav;

    q::Clock::time_point m_last_state_request_tp = q::Clock::now();
    int m_state_requests = 10;

    math::vec3f m_camera_position_target;
};


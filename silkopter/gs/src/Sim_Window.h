#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include "ui_Sim_Window.h"

#include "Comms.h"
#include "Render_Widget.h"
#include "Camera_Controller_3D.h"

class Sim_Window : public QMainWindow
{
public:
    explicit Sim_Window(silk::HAL& hal, silk::Comms& comms, Render_Context& context, QWidget* parent = 0);
    ~Sim_Window();

    void process();

private:
    void refresh_nodes();

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void contextMenu(QGraphicsSceneMouseEvent* event);

    Ui::Sim_Window m_ui;

    silk::HAL& m_hal;
    silk::Comms& m_comms;
    Render_Context& m_context;
    Camera_Controller_3D m_camera_controller;

    void render_uav();
    void render_ground();

    struct UAV
    {
        math::vec3f position;
        math::quatf local_to_enu;
    } m_uav;
};


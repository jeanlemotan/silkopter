#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Simulator.h"
#include "ui_Test_Ahrs.h"
#include "qcustomplot.h"
#include "World.h"
#include "Render_Widget.h"
#include "Camera_Controller3D.h"
#include "Select_Gizmo.h"
#include "Move_Gizmo.h"
#include "Rotate_Gizmo.h"
#include "Config.h"

class Simulator : public QMainWindow
{
public:
	Simulator(QWidget *parent = 0);
	~Simulator();

private:
    void mouse_press_event(Mouse_Event const& event);
    void mouse_release_event(Mouse_Event const& event);
    void mouse_move_event(Mouse_Event const& event);
    void mouse_wheel_event(Mouse_Event const& event);

    void move_uav(math::vec3f const& delta, q::Clock::duration dt);
    void rotate_uav(math::vec3f const& delta, q::Clock::duration dt);

    void setup_test_ahrs();

    void update();

	std::unique_ptr<World> m_world;

	q::Clock::time_point m_last_time;

	Render_Widget* m_render_widget;
	Camera_Controller3D m_camera_controller;
    math::vec3f m_camera_position_target;
    Render_Context m_context;

    enum class Action
    {
        MOVE,
        ROTATE,
    };
    Action m_action = Action::MOVE;

    struct Gizmos
    {
        Select_Gizmo select = Select_Gizmo(50);
        Move_Gizmo move = Move_Gizmo(50);
        Rotate_Gizmo rotate = Rotate_Gizmo(50);
        bool started = false;
        math::quatf local_base;
        math::vec3f rotate_start;
        math::quatf snap_base;
        math::vec3f snap_origin;
        math::vec3f point;
        math::vec3f delta;
    } m_gizmos;

    struct Test_Data
    {
        enum class Test
        {
            NONE,
            ROTATE,
            MOVE
        } test = Test::NONE;

        math::vec3f target;
        math::vec3f current;
        q::Clock::duration duration{0};
        q::Clock::duration current_duration{0};
    } m_test_data;

    bool m_stop_io_service_thread = false;
    std::thread m_io_service_thread;
    boost::asio::io_service m_io_service;

    Config m_config;

    std::mutex m_update_mutex;

    Ui::Simulator_Widget m_ui;
};




#include "stdafx.h"
#include "Simulator.h"
#include "World.h"

Simulator::Simulator(QWidget *parent)
	: QMainWindow(parent)
    , m_camera_controller(m_context.camera)
    , m_input_mgr("")
{
    m_ui.setupUi(this);

    m_ui.action_simulation->setChecked(m_config.environment.is_simulation_enabled);
    QObject::connect(m_ui.action_simulation, &QAction::toggled, [this](bool v) { m_config.environment.is_simulation_enabled = v != 0; });
    QObject::connect(m_ui.action_stop_motion, &QAction::triggered, [this](bool)
    {
        if (m_world)
        {
            m_world->get_uav().stop();
        }
    });
    QObject::connect(m_ui.action_reset, &QAction::triggered, [this](bool)
    {
        if (m_world)
        {
            m_world->reset();
        }
    });

    QActionGroup* ag = new QActionGroup(this);
    m_ui.action_move->setActionGroup(ag);
    m_ui.action_rotate->setActionGroup(ag);
    QObject::connect(m_ui.action_move, &QAction::triggered, [this](bool) { m_action = Action::MOVE; });
    QObject::connect(m_ui.action_rotate, &QAction::triggered, [this](bool) { m_action = Action::ROTATE; });

    QObject::connect(m_ui.action_test_ahrs, &QAction::triggered, [this](bool) { setup_test_ahrs(); });

    setMinimumHeight(200);

    m_render_widget = new Render_Widget(this);
	m_ui.centralWidget->layout()->addWidget(m_render_widget);

	auto s = new QShortcut(QKeySequence("CTRL+R"), this, 0, 0, Qt::WindowShortcut);
	connect(s, &QShortcut::activated, [&]() { m_world->reset(); });


    {
        math::vec2f positions[] = {math::vec2f(0, 1), math::vec2f(-1, 0), math::vec2f(0, -1), math::vec2f(1, 0)};
        bool clockwise[] = {false, true, false, true};

        q::util::Rand rnd;
        for (uint8_t i = 0; i < 4; i++)
        {
            Config::Motor motor;
            motor.position = positions[i];
            motor.clockwise = clockwise[i];
            motor.max_rpm = 7470 + rnd.get_float()*500.f;
            motor.acceleration = 20000 + rnd.get_float()*1000.f;
            motor.decceleration = 15000 + rnd.get_float()*2000.f;
            motor.drag = rnd.get_positive_float()*0.1f;

            float max_amps = 9.5f + rnd.get_float()*1.5f;
            float max_thrust = 0.756f + rnd.get_float()*0.17f;

            motor.amps_curve.push_back(std::make_pair(0.f, 0.f));
            motor.amps_curve.push_back(std::make_pair(1.f, max_amps));
            motor.thrust_curve.push_back(std::make_pair(0.f, 0.f));
            motor.thrust_curve.push_back(std::make_pair(1.f, max_thrust));
            m_config.uav.motors.push_back(motor);
        }
    }

    m_ui.environment->set_config(&m_config);
    m_ui.sensors->set_config(&m_config);

    m_context.camera.set_perspective_vertical_fov(math::anglef(math::radians(60.f)));
    m_context.camera.set_near_distance(0.05f);
    m_context.camera.set_far_distance(20000.f);

	{
		QSettings settings;
		math::vec3f position;
		math::from_string(position, std::string(settings.value("camera/position", "0,0,0").toString().toLatin1().data()));
        m_context.camera.set_position(position);
		math::quatf rotation;
		math::from_string(rotation, std::string(settings.value("camera/rotation", "0,0,0,1").toString().toLatin1().data()));
        m_context.camera.set_rotation(rotation);
	}

    m_context.scene.set_camera(m_context.camera);
    m_context.painter.set_camera(m_context.camera);

    m_io_service_thread = std::thread([this]()
    {
        while (!m_stop_io_service_thread)
        {
            m_io_service.run_one();
            m_io_service.reset();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    });


	show();


    //////////////////////////////////////////////////////////////////////////
    // load resources
    auto technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/primitive_3d.technique"));
    QASSERT(technique);
    m_context.materials.primitive.set_technique(technique);
    m_context.materials.primitive.get_render_state(0).set_depth_test(true);
    m_context.materials.primitive.get_render_state(0).set_depth_write(true);
    m_context.materials.primitive.get_render_state(0).set_culling(false);

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/primitive_2d.technique"));
    QASSERT(technique);
    m_context.materials.primitive_2d.set_technique(technique);
    m_context.materials.primitive_2d.get_render_state(0).set_depth_test(true);
    m_context.materials.primitive_2d.get_render_state(0).set_depth_write(true);
    m_context.materials.primitive_2d.get_render_state(0).set_culling(false);

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/font.technique"));
    QASSERT(technique);
    m_context.materials.font.set_technique(technique);
    m_context.materials.font.get_render_state(0).set_depth_test(true);
    m_context.materials.font.get_render_state(0).set_depth_write(true);
    m_context.materials.font.get_render_state(0).set_culling(false);

    //////////////////////////////////////////////////////////////////////////

    m_world.reset(new World(m_io_service, m_config));
	m_world->init();
    m_world->connect(boost::asio::ip::address::from_string("127.0.0.1"), 52523);

	m_render_widget->init();

    m_render_widget->mouse_move_signal.connect(std::bind(&Simulator::mouse_move_event, this, std::placeholders::_1));
    m_render_widget->mouse_press_signal.connect(std::bind(&Simulator::mouse_press_event, this, std::placeholders::_1));
    m_render_widget->mouse_release_signal.connect(std::bind(&Simulator::mouse_release_event, this, std::placeholders::_1));
    m_render_widget->mouse_wheel_signal.connect(std::bind(&Simulator::mouse_wheel_event, this, std::placeholders::_1));

	m_last_time = q::Clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
	timer->start(16);
	connect(timer, &QTimer::timeout, this, &Simulator::update);
}

Simulator::~Simulator()
{
    m_stop_io_service_thread = true;
    if (m_io_service_thread.joinable())
    {
        m_io_service_thread.join();
    }

	delete m_render_widget;
	m_render_widget = nullptr;
// 	delete m_ui.widget;
// 	m_ui.widget = nullptr;
}

math::quatf xxx_rot;

void Simulator::update()
{
    std::unique_lock<std::mutex> lg(m_update_mutex, std::try_to_lock);
    if (!lg.owns_lock())
    {
        return;
    }

	q::System::inst().get_renderer()->begin_frame();
	m_render_widget->begin_rendering();

    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_render_widget->width(), m_render_widget->height())));

	q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(math::vec4f(math::vec3f(m_render_widget->width() / 3024.f, 0, 0), 1));
	q::System::inst().get_renderer()->get_render_target()->clear_all();

    auto& uav = m_world->get_uav();

	auto now = q::Clock::now();
	auto dt = now - m_last_time;
	m_last_time = now;

    auto camera_offset = m_context.camera.get_position() - m_camera_position_target;

    m_world->process(dt);

    m_ui.status->process(uav);

    {
        m_input_mgr.update(dt);
        auto gamepads = m_input_mgr.get_all_gamepads();
        if (!gamepads.empty())
        {
            auto gamepad = gamepads[0];

            auto ls = gamepad->get_stick_data(qinput::Gamepad::Stick::LEFT).value;
            auto rs = gamepad->get_stick_data(qinput::Gamepad::Stick::RIGHT).value;

            xxx_rot.set_from_euler_xyz(-rs.y*3.1415f, rs.x*3.1415f, -ls.x*3.1415f);
        }
    }

    if (m_gizmos.started)
    {
        if (m_action == Action::MOVE)
        {
            move_uav(m_gizmos.delta, dt);
        }
        else if (m_action == Action::ROTATE)
        {
            rotate_uav(math::vec3f(m_gizmos.delta), dt);
        }
        m_gizmos.delta = math::vec3f::zero;
    }
    else
    {
        m_camera_position_target = math::lerp(m_camera_position_target, uav.get_position(), 0.9f);
        m_context.camera.set_position(m_camera_position_target + camera_offset);
    }

    if (m_test_data.test == Test_Data::Test::ROTATE)
    {
        float mu = q::Seconds(m_test_data.current_duration).count() / q::Seconds(m_test_data.duration).count();
        auto value = math::lerp(math::vec3f::zero, m_test_data.rotate_target, mu);
        auto delta = value - m_test_data.rotate_current;
        m_test_data.rotate_current = value;
        rotate_uav(delta, dt);
        m_test_data.current_duration += dt;
        if (m_test_data.current_duration > m_test_data.duration)
        {
            m_world->get_uav().get_body().setAngularVelocity(btVector3(0, 0, 0));
            m_test_data.test = Test_Data::Test::NONE;
        }
    }

    m_camera_controller.set_focus_point(m_camera_position_target);
    m_context.painter.set_camera(m_context.camera);

    m_world->render(m_context);

    {
        math::trans3df trans(uav.get_position(), uav.get_rotation(), math::vec3f::one);
        if (m_action == Action::MOVE)
        {
            m_gizmos.move.set_transform(trans);
            m_gizmos.move.render(m_context, m_camera_controller.get_pointer_2d());
        }
        else if (m_action == Action::ROTATE)
        {
            m_gizmos.rotate.set_transform(trans);
            m_gizmos.rotate.render(m_context, m_camera_controller.get_pointer_2d());
        }
        else
        {
            m_gizmos.select.set_transform(trans);
            m_gizmos.select.render(m_context);
        }
    }

    //test
    {
        math::trans3df trans(uav.get_position() + math::vec3f(3, 0, 0), xxx_rot, math::vec3f::one);
        m_gizmos.move.set_transform(trans);
        m_gizmos.move.render(m_context, m_camera_controller.get_pointer_2d());

        auto& cr = uav.get_rotation();
        auto diff = math::quatf::from_a_to_b(cr, xxx_rot);

        math::vec3f diff_euler;
        diff.get_as_euler_xyz(diff_euler);

        diff_euler = (diff_euler) * math::radians(1.f);
        math::quatf q;
        q.set_from_euler_xyz(diff_euler);

        uav.set_rotation(uav.get_rotation() * q);
    }

    m_context.painter.flush();

	m_render_widget->end_rendering();
	m_render_widget->update();
	q::System::inst().get_renderer()->end_frame();
}

void Simulator::mouse_press_event(Mouse_Event const& event)
{
    m_camera_controller.mouse_press_event(event);
}
void Simulator::mouse_release_event(Mouse_Event const& event)
{
    m_camera_controller.mouse_release_event(event);

    if (m_action == Action::MOVE || m_action == Action::ROTATE)
    {
        m_gizmos.started = false;
        m_gizmos.delta.set(0, 0, 0);
        m_gizmos.move.set_override_axis_filter(boost::none);
        m_gizmos.rotate.set_override_axis_filter(boost::none);
        m_world->get_uav().get_body().setAngularFactor(btVector3(1, 1, 1));
        m_world->get_uav().get_body().setLinearFactor(btVector3(1, 1, 1));
        m_world->get_uav().get_body().setAngularVelocity(btVector3(0, 0, 0));
        m_world->get_uav().get_body().setLinearVelocity(btVector3(0, 0, 0));
        m_world->get_uav().set_force(math::vec3f::zero);
    }
}
void Simulator::mouse_move_event(Mouse_Event const& event)
{
    m_camera_controller.mouse_move_event(event);

    if (event.buttons.test(Mouse_Event::Button::LEFT) &&
            (m_action == Action::MOVE || m_action == Action::ROTATE))
    {
        if (!m_gizmos.started)
        {
            m_gizmos.move.set_override_axis_filter(m_gizmos.move.get_axis_filter());
            m_gizmos.rotate.set_override_axis_filter(m_gizmos.rotate.get_axis_filter());
            m_gizmos.point = m_world->get_uav().get_position();
            m_gizmos.local_base = m_world->get_uav().get_rotation();
            m_gizmos.local_base.get_as_euler_xyz(m_gizmos.rotate_start.x, m_gizmos.rotate_start.y, m_gizmos.rotate_start.z);
            m_world->get_uav().get_body().setAngularVelocity(btVector3(0, 0, 0));
            m_world->get_uav().get_body().clearForces();
            if (m_action == Action::MOVE)
            {
                m_world->get_uav().get_body().setAngularFactor(btVector3(0, 0, 0));
            }
            else if (m_action == Action::ROTATE)
            {
                m_world->get_uav().get_body().setLinearFactor(btVector3(0, 0, 0));
            }
        }

        if (m_action == Action::MOVE)
        {
            math::vec3f filter = m_gizmos.move.get_axis_filter();

            math::line3f ray = m_context.camera.get_ray_from_screen_coordinates(m_camera_controller.get_pointer_2d());
            math::vec3f dir = ray.get_vector_normalized();
            math::vec3f delta;

            {
                math::vec3f normals[2];
                if (filter.x > 0.5f)
                {
                    normals[0] = math::rotate(m_gizmos.local_base, math::vec3f(0, 1, 0));
                    normals[1] = math::rotate(m_gizmos.local_base, math::vec3f(0, 0, 1));
                }
                else if (filter.y > 0.5f)
                {
                    normals[0] = math::rotate(m_gizmos.local_base, math::vec3f(1, 0, 0));
                    normals[1] = math::rotate(m_gizmos.local_base, math::vec3f(0, 0, 1));
                }
                else if (filter.z > 0.5f)
                {
                    normals[0] = math::rotate(m_gizmos.local_base, math::vec3f(1, 0, 0));
                    normals[1] = math::rotate(m_gizmos.local_base, math::vec3f(0, 1, 0));
                }

                math::vec3f line_axis = math::rotate(m_gizmos.local_base, filter) * 1000.0f;
                math::line3f line(m_gizmos.point - line_axis, m_gizmos.point + line_axis);
                math::vec3f point = m_gizmos.point;
                float min_len_sq = FLT_MAX;
                for (size_t i = 0; i < 2; i++)
                {
                    math::planef plane(m_gizmos.point, normals[i]);
                    math::vec3f tmp;
                    if (plane.get_intersection_with_line(ray.start, dir, tmp))
                    {
                        math::vec3f p = line.get_closest_point(tmp);
                        float len_sq = math::distance_sq(tmp,p);
                        if (len_sq < min_len_sq)
                        {
                            min_len_sq = len_sq;
                            point = p;
                        }
                    }
                }
                delta = point - m_gizmos.point;
                m_gizmos.point = point;
            }
            if (m_gizmos.started)
            {
                m_gizmos.delta = delta;
            }

            m_gizmos.started = true;
        }
        else if (m_action == Action::ROTATE)
        {
            auto tangent = m_gizmos.rotate.get_tangent();
            if (!math::equals(tangent.start, tangent.end))
            {
                math::vec2f p = tangent.get_closest_point(m_camera_controller.get_pointer_2d());
                float sign = math::sgn(math::dot(tangent.get_vector_normalized(), p - tangent.get_middle()));
                float dist = 0.005f * sign * math::distance(p, tangent.get_middle());
                auto euler = math::vec3f(dist, dist, dist) * m_gizmos.rotate.get_axis_filter();
                if (euler != m_gizmos.rotate_start)
                {
                    auto delta = euler - m_gizmos.rotate_start;
                    if (m_gizmos.started)
                    {
                        m_gizmos.delta.set(delta.x, delta.y, delta.z);
                    }

                    m_gizmos.started = true;
                    m_gizmos.rotate_start = euler;
                }
            }
        }

    }
}
void Simulator::mouse_wheel_event(Mouse_Event const& event)
{
    m_camera_controller.mouse_wheel_event(event);
}

void Simulator::move_uav(math::vec3f const& delta, q::Clock::duration /*dt*/)
{
    auto& uav = m_world->get_uav();
    if (m_config.environment.is_simulation_enabled)
    {
        auto v = (m_gizmos.point - uav.get_position());
        uav.set_force(v);
    }
    else
    {
        uav.set_position(uav.get_position() + delta);
    }
}

void Simulator::rotate_uav(math::vec3f const& delta, q::Clock::duration dt)
{
    if (math::is_zero(delta))
    {
        return;
    }
    auto& uav = m_world->get_uav();
    if (m_config.environment.is_simulation_enabled)
    {
        auto d = math::rotate(uav.get_rotation(), math::vec3f(delta));
        uav.get_body().setAngularVelocity(vec3f_to_bt(d / q::Seconds(dt).count()));
    }
    else
    {
        math::quatf r;
        r.set_from_euler_xyz(delta.x, delta.y, delta.z);
        uav.set_rotation(uav.get_rotation() * r);
    }
}


void Simulator::setup_test_ahrs()
{
    QDialog* dialog = new QDialog(this);
    Ui::Test_Ahrs ui;
    ui.setupUi(dialog);

    auto res = dialog->exec();
    if (res != QDialog::Accepted)
    {
        return;
    }

    if (!ui.test->currentItem())
    {
        return;
    }

    if (ui.test->currentItem()->data(Qt::DisplayRole) == QString("Rotate"))
    {
        m_test_data.test = Test_Data::Test::ROTATE;
    }
    else if (ui.test->currentItem()->data(Qt::DisplayRole) == QString("Move"))
    {
        m_test_data.test = Test_Data::Test::MOVE;
    }
    else
    {
        return;
    }

    m_test_data.rotate_current = math::vec3f::zero;
    m_test_data.rotate_target = math::radians(math::vec3f(ui.target_x->value(), ui.target_y->value(), ui.target_z->value()));
    m_test_data.duration = std::chrono::microseconds(static_cast<int64_t>(ui.duration->value() * 1000000.0));
    m_test_data.current_duration = std::chrono::seconds(0);

    delete dialog;
}

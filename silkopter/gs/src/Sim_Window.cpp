#include "Sim_Window.h"

#include "sz_math.hpp"
#include "sz_Multi_Simulator_Structs.hpp"

Sim_Window::Sim_Window(silk::HAL& hal, silk::node::Node_ptr sim_node, silk::Comms& comms, Render_Context& context, QWidget *parent)
    : QMainWindow(parent)
    , m_hal(hal)
    , m_sim_node(sim_node)
    , m_comms(comms)
    , m_context(context)
    , m_camera_controller(context.camera)
{
    setWindowTitle("Simulator");
    setMouseTracking(true);

    m_ui.setupUi(this);

    //m_ui.action_simulation_on->setChecked(m_config.environment.is_simulation_enabled);
    QObject::connect(m_ui.action_simulation, &QAction::toggled, [this](bool v)
    {
        auto* vj = jsonutil::find_value(m_sim_node->config, std::string("Simulation Enabled"));
        if (vj)
        {
            vj->SetBool(v);
            m_hal.set_node_config(m_sim_node, m_sim_node->config);
        }
    });
    QObject::connect(m_ui.action_stop_motion, &QAction::triggered, [this](bool)
    {
        rapidjson::Document message;
        jsonutil::add_value(message, std::string("message"), rapidjson::Value("stop motion"), message.GetAllocator());
        m_hal.send_node_message(m_sim_node, std::move(message), [](silk::HAL::Result, rapidjson::Document) {});
    });
    QObject::connect(m_ui.action_reset, &QAction::triggered, [this](bool)
    {
        rapidjson::Document message;
        jsonutil::add_value(message, std::string("message"), rapidjson::Value("reset"), message.GetAllocator());
        m_hal.send_node_message(m_sim_node, std::move(message), [](silk::HAL::Result, rapidjson::Document) {});
    });
    QObject::connect(m_ui.action_ground, &QAction::triggered, [this](bool v)
    {
        auto* vj = jsonutil::find_value(m_sim_node->config, std::string("Ground Enabled"));
        if (vj)
        {
            vj->SetBool(v);
            m_hal.set_node_config(m_sim_node, m_sim_node->config);
        }
    });
    QObject::connect(m_ui.action_gravity, &QAction::triggered, [this](bool v)
    {
        auto* vj = jsonutil::find_value(m_sim_node->config, std::string("Gravity Enabled"));
        if (vj)
        {
            vj->SetBool(v);
            m_hal.set_node_config(m_sim_node, m_sim_node->config);
        }
    });

    //    QActionGroup* ag = new QActionGroup(this);
    //    m_ui.action_move->setActionGroup(ag);
    //    m_ui.action_rotate->setActionGroup(ag);
    //    QObject::connect(m_ui.action_move, &QAction::triggered, [this](bool) { m_action = Action::MOVE; });
    //    QObject::connect(m_ui.action_rotate, &QAction::triggered, [this](bool) { m_action = Action::ROTATE; });

    //    QObject::connect(m_ui.action_test_ahrs, &QAction::triggered, [this](bool) { setup_test_ahrs(); });

    setMinimumHeight(200);

    //    m_render_widget = new Render_Widget(this);
    //    m_ui.centralWidget->layout()->addWidget(m_render_widget);

    auto s = new QShortcut(QKeySequence("CTRL+R"), this, 0, 0, Qt::WindowShortcut);
    connect(s, &QShortcut::activated, [&]()
    {
    });


    sim_node->changed_signal.connect([this](silk::node::Node&) { read_config(); });
    read_config();

    //////////////////////////////////////////////////////////////////////////

    m_ui.render_widget->init();
}

Sim_Window::~Sim_Window()
{
}

void Sim_Window::read_config()
{
    auto* vj = jsonutil::find_value(m_sim_node->config, std::string("Simulation Enabled"));
    if (vj)
    {
        m_ui.action_simulation->setChecked(vj->GetBool());
    }
    vj = jsonutil::find_value(m_sim_node->config, std::string("Gravity Enabled"));
    if (vj)
    {
        m_ui.action_gravity->setChecked(vj->GetBool());
    }
    vj = jsonutil::find_value(m_sim_node->config, std::string("Ground Enabled"));
    if (vj)
    {
        m_ui.action_ground->setChecked(vj->GetBool());
    }

//    vj = jsonutil::find_value(m_sim_node->config, std::string("UAV Config"));
//    if (vj)
//    {
//        autojsoncxx::error::ErrorStack result;
//        m_uav.config = silk::node::ISimulator::UAV_Config();
//        if (!autojsoncxx::from_value(m_uav.config, *vj, result))
//        {
//            std::ostringstream ss;
//            ss << result;
//            QLOGE("Cannot deserialize position data: {}", ss.str());
//        }
//    }
}


void Sim_Window::render_ground()
{
    math::vec3s32 offset(m_uav.state.enu_position);
    offset.z = 0;

    math::trans3df trans;
    trans.set_translation(math::vec3f(offset));
    m_context.painter.push_post_clip_transform(trans);

    auto mat = m_context.materials.primitive;
    mat.get_render_state(0).set_depth_test(true);
    mat.get_render_state(0).set_depth_write(false);
    mat.get_render_state(0).set_culling(false);
    mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
    m_context.painter.set_material(mat);

    const float k_size = 10000;
    const int k_line_count = 300;
    const int k_half_line_size = k_line_count / 2;

    //m_context.painter.fill_rectangle(q::draw::Vertex(math::vec3f(-k_size, -k_size, 0), 0x20FFFFFF), q::draw::Vertex(math::vec3f(k_size, k_size, 0), 0x20FFFFFF));

    float max_alpha = 20.f;
    float color_inc = 2.f / float(k_line_count);
    for (int i = 0; i < k_line_count; i++)
    {
        float x = float(i) - k_half_line_size;
        uint32_t color = 0x00FFFFFF;
        uint32_t color2 = color | static_cast<int>((1.f - math::abs(x) * color_inc) * max_alpha) << 24;

        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(-k_half_line_size, x, 0), color), q::draw::Vertex(math::vec3f(0, x, 0), color2));
        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(0, x, 0), color2), q::draw::Vertex(math::vec3f(k_half_line_size, x, 0), color));

        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(x, -k_half_line_size, 0), color), q::draw::Vertex(math::vec3f(x, 0, 0), color2));
        m_context.painter.draw_line(q::draw::Vertex(math::vec3f(x, 0, 0), color2), q::draw::Vertex(math::vec3f(x, k_half_line_size, 0), color));
    }

    m_context.painter.pop_post_clip_transform();
}

void Sim_Window::render_uav()
{
    math::trans3df trans;
    trans.set_rotation(m_uav.state.local_to_enu_rotation);
    trans.set_translation(m_uav.state.enu_position);
    m_context.painter.push_post_clip_transform(trans);

    m_context.painter.set_material(m_context.materials.primitive);
    //render_axes(m_context.painter, 1.f);

    //render motors
    {
        //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
        //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
        //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);

        const float motor_radius = 0.12f;
//        if (m_uav.state.motors.size() == m_uav.config.motors.size())
//        {
//            for (size_t i = 0; i < m_uav.config.motors.size(); i++)
//            {
//                auto const& mc = m_uav.config.motors[i];
//                auto const& m = m_uav.state.motors[i];
//                m_context.painter.draw_circle(q::draw::Vertex(mc.position*m_uav.config.radius, 0xFFFFFFFF), motor_radius);
//                float ratio = m.rpm / float(mc.max_rpm);
//                m_context.painter.fill_circle(q::draw::Vertex(mc.position*m_uav.config.radius, 0xFF00FF00), motor_radius * ratio);
//            }
//        }
    }

    m_context.painter.pop_post_clip_transform();

//	//RENDER IMU DATA
//	{
//		math::trans3df trans;
//        trans.set_rotation(m_uav.get_rotation());
//        trans.set_translation(m_uav.m_position);
//        m_context.painter.push_post_clip_transform(trans);

//        m_context.painter.set_material(m_context.materials.primitive);
//        render_axes(m_context.painter, 0.5f);

//        m_context.painter.pop_post_clip_transform();
//	}
}


void Sim_Window::process()
{
    if (!isVisible())
    {
        return;
    }

    auto now = q::Clock::now();

    //request state
    if (now - m_last_state_request_tp > std::chrono::milliseconds(33) && m_state_requests > 0)
    {
        m_state_requests--;
        rapidjson::Document message;
        jsonutil::add_value(message, std::string("message"), rapidjson::Value("get state"), message.GetAllocator());
        m_hal.send_node_message(m_sim_node, std::move(message), [this](silk::HAL::Result result, rapidjson::Document response)
        {
            if (result == silk::HAL::Result::OK)
            {
                silk::node::IMulti_Simulator::UAV_State state;
                autojsoncxx::error::ErrorStack result;
                if (!autojsoncxx::from_value(state, response, result))
                {
                    std::ostringstream ss;
                    ss << result;
                    QLOGE("Cannot deserialize position data: {}", ss.str());
                }
                else
                {
                    m_uav.state = state;
//                    QASSERT(m_uav.state.motors.size() == m_uav.config.motors.size());
                }
            }
            m_state_requests++;
        });
    };


    m_ui.render_widget->begin_rendering();

    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_ui.render_widget->width(), m_ui.render_widget->height())));

    auto color = math::color::u32_to_rgba<float>(0x2c3e50);
    q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(color);
    q::System::inst().get_renderer()->get_render_target()->clear_all();

    auto camera_offset = m_context.camera.get_position() - m_camera_position_target;
    m_camera_position_target = math::lerp(m_camera_position_target, m_uav.state.enu_position, 0.9f);
    m_context.camera.set_position(m_camera_position_target + camera_offset);
    m_camera_controller.set_focus_point(m_camera_position_target);

    m_context.painter.set_camera(m_context.camera);

//    m_context.painter.set_material(m_context.materials.primitive);
//    m_context.painter.fill_sphere(q::draw::Vertex(math::vec3f(0, 0, 0), 0xFF00FF), 0.1, 0);
    render_ground();
    render_uav();

    m_context.painter.flush();

    m_ui.render_widget->end_rendering();
    m_ui.render_widget->update();
}

/////////////////////////////////////////////////////////////////////////

void Sim_Window::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    m_camera_controller.mouse_press_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Sim_Window::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    m_camera_controller.mouse_release_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Sim_Window::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
    m_camera_controller.mouse_move_event(event);
}

//////////////////////////////////////////////////////////////////////////

void Sim_Window::wheelEvent(QWheelEvent* event)
{
    QWidget::wheelEvent(event);
    m_camera_controller.wheel_event(event);
}

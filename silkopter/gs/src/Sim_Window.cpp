#include "Sim_Window.h"

Sim_Window::Sim_Window(silk::HAL& hal, silk::Comms& comms, Render_Context& context, QWidget *parent)
    : QMainWindow(parent)
    , m_hal(hal)
    , m_comms(comms)
    , m_context(context)
    , m_camera_controller(context.camera)
{
    setWindowTitle("Simulator");
    setMouseTracking(true);

    m_ui.setupUi(this);

    //m_ui.action_simulation_on->setChecked(m_config.environment.is_simulation_enabled);
    QObject::connect(m_ui.action_simulation_on, &QAction::toggled, [this](bool v)
    {
    });
    QObject::connect(m_ui.action_stop_motion, &QAction::triggered, [this](bool)
    {
    });
    QObject::connect(m_ui.action_reset, &QAction::triggered, [this](bool)
    {
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


    //////////////////////////////////////////////////////////////////////////

    m_ui.render_widget->init();
}

Sim_Window::~Sim_Window()
{
}

void Sim_Window::render_ground()
{
    math::vec3s32 offset(m_uav.position);
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
    trans.set_rotation(m_uav.local_to_enu);
    trans.set_translation(m_uav.position);
    m_context.painter.push_post_clip_transform(trans);

    m_context.painter.set_material(m_context.materials.primitive);
    //render_axes(m_context.painter, 1.f);

    //render motors
    {
        //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
        //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
        //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);

//        const float motor_radius = 0.12f;
//        for (auto const& m : m_uav.m_motors)
//        {
//            auto const& config = m.get_config();

//            m_context.painter.draw_circle(q::draw::Vertex(config.position*m_config.uav.radius, 0xFFFFFFFF), motor_radius);
//            float ratio = m.compute_rpm() / (config.Kv * m_config.battery.volts);
//            m_context.painter.fill_circle(q::draw::Vertex(config.position*m_config.uav.radius, 0xFF00FF00), motor_radius * ratio);
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

    m_ui.render_widget->begin_rendering();

    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_ui.render_widget->width(), m_ui.render_widget->height())));

    auto color = math::color::u32_to_rgba<float>(0x2c3e50);
    q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(color);
    q::System::inst().get_renderer()->get_render_target()->clear_all();


    m_camera_controller.set_focus_point(math::vec3f::zero);
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

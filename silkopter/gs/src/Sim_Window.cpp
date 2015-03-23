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

void Sim_Window::process()
{
    m_ui.render_widget->begin_rendering();

    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_ui.render_widget->width(), m_ui.render_widget->height())));

    q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(math::vec4f(math::vec3f(m_ui.render_widget->width() / 3024.f, 0, 0), 1));
    q::System::inst().get_renderer()->get_render_target()->clear_all();


    m_camera_controller.set_focus_point(math::vec3f::zero);
    m_context.painter.set_camera(m_context.camera);

    m_context.painter.set_material(m_context.materials.primitive);
    m_context.painter.fill_sphere(q::draw::Vertex(math::vec3f(0, 0, 0), 0xFF00FF), 0.1, 0);

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

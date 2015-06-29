#include "GS.h"

#include "HAL_Window.h"
#include "Multi_Config_Window.h"

GS::GS(QWidget *parent)
	: QMainWindow(parent)
    , m_hal(m_comms)
    , m_comms(m_io_service, m_hal)
{
    m_ui.setupUi(this);

    m_last_tp = q::Clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
    timer->start(33);
    connect(timer, &QTimer::timeout, this, &GS::process, Qt::QueuedConnection);

	show();

//    m_input_mgr.reset(new qinput::Input_Mgr(q::util::format2<q::String>("{}", uint64_t(winId()))));

    m_io_service_thread = std::thread([this]()
    {
		while (!m_stop_io_service_thread)
		{
            m_io_service.run();
            m_io_service.reset();
            std::this_thread::yield();
        }
	});

    //m_comm_channel.connect(boost::asio::ip::address::from_string("127.0.0.1"), 52524);

    {
        QSettings settings;
        m_remote_address = settings.value("address", "").toString().toLatin1().data();
        if (m_remote_address.size() > 15)
        {
            m_remote_address.clear();
        }
    }

    init_graphics();

    //set_remote_address("10.10.10.10");
    set_remote_address("127.0.0.1");

//    connect(m_ui.action_connect_uav, &QAction::triggered, [this](bool) { set_uav_address("192.168.1.110"); });
//    connect(m_ui.action_connect_uav_ah, &QAction::triggered, [this](bool) { set_uav_address("10.10.10.10"); });
//    connect(m_ui.action_connect_simulator, &QAction::triggered, [this](bool) { set_uav_address("127.0.0.1"); });

    m_hal_window = new HAL_Window(m_hal, m_comms, m_context, this);
    m_multi_config_window = new Multi_Config_Window(m_hal, m_comms, this);


    connect(m_ui.action_hal_editor, &QAction::triggered, [this](bool) { m_hal_window->show(); });
    connect(m_ui.action_multi_config_window, &QAction::triggered, [this](bool) { m_multi_config_window->show(); });


    read_settings();
}

GS::~GS()
{
//    m_comms.disconnect();

	m_stop_io_service_thread = true;
    m_io_service.stop();
    if (m_io_service_thread.joinable())
    {
        m_io_service_thread.join();
    }
}

void GS::init_graphics()
{
    m_context.camera.set_perspective_vertical_fov(math::anglef(math::radians(60.f)));
    m_context.camera.set_near_distance(0.05f);
    m_context.camera.set_far_distance(20000.f);
    m_context.scene.set_camera(m_context.camera);
    m_context.painter.set_camera(m_context.camera);

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
}

void GS::closeEvent(QCloseEvent* event)
{
    m_stop_io_service_thread = true;
    m_io_service.stop();

    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("hal_window/geometry", m_hal_window->saveGeometry());
    settings.setValue("hal_window/windowState", m_hal_window->saveState());
    settings.setValue("multi_config_window/geometry", m_multi_config_window->saveGeometry());
    settings.setValue("multi_config_window/windowState", m_multi_config_window->saveState());
//    settings.setValue("sim_window/geometry", m_sim_window->saveGeometry());
//    settings.setValue("sim_window/windowState", m_sim_window->saveState());

    QMainWindow::closeEvent(event);
}

void GS::read_settings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    m_hal_window->restoreGeometry(settings.value("hal_window/geometry").toByteArray());
    m_hal_window->restoreState(settings.value("hal_window/windowState").toByteArray());
    m_multi_config_window->restoreGeometry(settings.value("multi_config_window/geometry").toByteArray());
    m_multi_config_window->restoreState(settings.value("multi_config_window/windowState").toByteArray());
//    m_sim_window->restoreGeometry(settings.value("sim_window/geometry").toByteArray());
//    m_sim_window->restoreState(settings.value("sim_window/windowState").toByteArray());
}


void GS::set_remote_address(std::string const& address)
{
    m_remote_address = address;
    QSettings settings;
    settings.setValue("address", address.c_str());

//    m_ui.statusBar->showMessage(q::util::format2<std::string>("Connecting to {}", address).c_str(), 2000);
}

void GS::process()
{
    if (!m_comms.is_connected() && !m_remote_address.empty())
    {
        m_comms.start_udp(boost::asio::ip::address::from_string(m_remote_address), 8001, 8000);
    }

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    m_last_tp = now;

    m_comms.process();

    q::System::inst().get_renderer()->begin_frame();

    m_hal_window->process();
    m_multi_config_window->process();


//	m_render_widget->begin_rendering();
//    m_context.camera.set_viewport_and_aspect_ratio(q::video::Viewport(math::vec2u32::zero, math::vec2u32(m_render_widget->width(), m_render_widget->height())));

//	q::System::inst().get_renderer()->get_render_target()->set_color_clear_value(math::vec4f(math::vec3f(m_render_widget->width() / 3024.f, 0, 0), 1));
//	q::System::inst().get_renderer()->get_render_target()->clear_all();
//    m_context.painter.set_camera(m_context.camera);

//    m_context.painter.flush();

//	m_render_widget->end_rendering();
//	m_render_widget->update();
    q::System::inst().get_renderer()->end_frame();
}


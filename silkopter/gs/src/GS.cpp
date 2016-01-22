#include "GS.h"

#include "HAL_Window.h"
#include "Multi_Config_Window.h"
#include "hud/Multi_HUD_Widget.h"

GS::GS(QWidget *parent)
	: QMainWindow(parent)
    , m_hal(m_comms)
    , m_comms(m_hal)
{
    m_ui.setupUi(this);

    m_process_last_tp = q::Clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
    timer->start(1);
    connect(timer, &QTimer::timeout, this, &GS::process, Qt::QueuedConnection);

	show();

    m_input_mgr.reset(new qinput::Input_Mgr(q::util::format2<q::String>("{}", uint64_t(winId()))));

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
    connect(m_ui.action_multi_hud, &QAction::triggered, [this](bool)
    {
        delete m_hud;
        m_hud = new Multi_HUD_Widget(m_hal, m_comms, *m_input_mgr, m_context, this);
        m_ui.centralwidget->layout()->addWidget(m_hud);
        m_ui.label->hide();
    });


    read_settings();
}

GS::~GS()
{
//    m_comms.disconnect();
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

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/textured_2d.technique"));
    QASSERT(technique);
    m_context.materials.textured_2d.set_technique(technique);
    m_context.materials.textured_2d.get_render_state(0).set_depth_test(true);
    m_context.materials.textured_2d.get_render_state(0).set_depth_write(true);
    m_context.materials.textured_2d.get_render_state(0).set_culling(false);

    technique = q::System::inst().get_factory().load<q::video::Technique>(q::Path("techniques/font.technique"));
    QASSERT(technique);
    m_context.materials.font.set_technique(technique);
    m_context.materials.font.get_render_state(0).set_depth_test(false);
    m_context.materials.font.get_render_state(0).set_depth_write(false);
    m_context.materials.font.get_render_state(0).set_culling(false);
    m_context.materials.font.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);

    m_context.font = q::System::inst().get_factory().load<q::text::Font>(q::Path("fonts/DroidSans.ttf"));
    QASSERT(m_context.font);
}

qinput::Keyboard_Event GS::get_keyboard_event(QKeyEvent* event) const
{
    qinput::Keyboard_Event ev;
    ev.timestamp = q::Clock::now();
    int key = event->key();

         if (key == Qt::Key_Escape) { ev.key_code = qinput::Key_Code::ESCAPE; }
    else if (key == Qt::Key_Control) { ev.key_code = qinput::Key_Code::CONTROL; }
    else if (key == Qt::Key_Alt) { ev.key_code = qinput::Key_Code::ALT; }
    else if (key == Qt::Key_Shift) { ev.key_code = qinput::Key_Code::SHIFT; }
    else if (key == Qt::Key_Insert) { ev.key_code = qinput::Key_Code::INSERT; }
    else if (key == Qt::Key_Home) { ev.key_code = qinput::Key_Code::HOME; }
    else if (key == Qt::Key_End) { ev.key_code = qinput::Key_Code::END; }
    else if (key == Qt::Key_PageUp) { ev.key_code = qinput::Key_Code::PAGE_UP; }
    else if (key == Qt::Key_PageDown) { ev.key_code = qinput::Key_Code::PAGE_DOWN; }
    else if (key == Qt::Key_Delete) { ev.key_code = qinput::Key_Code::DEL; }
    else if (key == Qt::Key_Backspace) { ev.key_code = qinput::Key_Code::BACKSPACE; }

    else if (key == Qt::Key_F1) { ev.key_code = qinput::Key_Code::F1; }
    else if (key == Qt::Key_F2) { ev.key_code = qinput::Key_Code::F2; }
    else if (key == Qt::Key_F3) { ev.key_code = qinput::Key_Code::F3; }
    else if (key == Qt::Key_F4) { ev.key_code = qinput::Key_Code::F4; }
    else if (key == Qt::Key_F5) { ev.key_code = qinput::Key_Code::F5; }
    else if (key == Qt::Key_F6) { ev.key_code = qinput::Key_Code::F6; }
    else if (key == Qt::Key_F7) { ev.key_code = qinput::Key_Code::F7; }
    else if (key == Qt::Key_F8) { ev.key_code = qinput::Key_Code::F8; }
    else if (key == Qt::Key_F9) { ev.key_code = qinput::Key_Code::F9; }
    else if (key == Qt::Key_F10) { ev.key_code = qinput::Key_Code::F10; }
    else if (key == Qt::Key_F11) { ev.key_code = qinput::Key_Code::F11; }
    else if (key == Qt::Key_F12) { ev.key_code = qinput::Key_Code::F12; }

    else if (key == Qt::Key_Up) { ev.key_code = qinput::Key_Code::UP; }
    else if (key == Qt::Key_Down) { ev.key_code = qinput::Key_Code::DOWN; }
    else if (key == Qt::Key_Left) { ev.key_code = qinput::Key_Code::LEFT; }
    else if (key == Qt::Key_Right) { ev.key_code = qinput::Key_Code::RIGHT; }

    else ev.key_code = key;

    return ev;
}

void GS::keyPressEvent(QKeyEvent* event)
{
    qinput::Keyboard_Event ev = get_keyboard_event(event);
    ev.type = qinput::Keyboard_Event::Type::PRESS;
    m_input_mgr->add_event(ev);
}

void GS::keyReleaseEvent(QKeyEvent* event)
{
    qinput::Keyboard_Event ev = get_keyboard_event(event);
    ev.type = qinput::Keyboard_Event::Type::RELEASE;
    m_input_mgr->add_event(ev);
}

void GS::closeEvent(QCloseEvent* event)
{
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
        //m_comms.start_rfmon("wlp0s20u1u4", 3);
    }

    m_comms.process_rcp();

    auto now = q::Clock::now();
    auto dt = now - m_process_last_tp;
    if (dt >= std::chrono::milliseconds(33))
    {
        m_process_last_tp = now;

        m_comms.process();
        m_input_mgr->process(dt);

        q::System::inst().get_renderer()->begin_frame();

        m_hal_window->process();
        m_multi_config_window->process();

        if (m_hud)
        {
            m_hud->process();
        }

        q::System::inst().get_renderer()->end_frame();
    }
}


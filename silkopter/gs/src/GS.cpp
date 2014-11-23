#include "stdafx.h"
#include "GS.h"


static const float k_calibration_sample_count = 100;

GS::GS(QWidget *parent)
	: QMainWindow(parent)
    //, m_gyro_calibrarion_samples(k_calibration_sample_count)
    , m_comms(m_io_service)
{
	m_ui.setupUi(this);

    m_last_time = q::Clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
    timer->start(10);
	connect(timer, &QTimer::timeout, this, &GS::process);

	show();

    m_input_mgr.reset(new qinput::Input_Mgr(q::util::format2<q::String>("{}", uint64_t(winId()))));

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

    m_video_client.reset(new Video_Client(m_comms.get_rudp()));

    m_ui.sensors->init(&m_comms);
    m_ui.uav_inertial->init(&m_comms);
    m_ui.pids->init(&m_comms);
    m_ui.video_tab->init(m_video_client);
    m_ui.input->init(m_input_mgr.get());

    {
        QSettings settings;
        m_uav_address = settings.value("address", "").toString().toLatin1().data();
        if (m_uav_address.size() > 15)
        {
            m_uav_address.clear();
        }
    }

    set_uav_address("192.168.1.110");

    connect(m_ui.action_connect_uav, &QAction::triggered, [this](bool) { set_uav_address("192.168.1.110"); });
    connect(m_ui.action_connect_simulator, &QAction::triggered, [this](bool) { set_uav_address("127.0.0.1"); });

    read_settings();
}

GS::~GS()
{
    m_comms.disconnect();

    m_video_client.reset();

	m_stop_io_service_thread = true;
    m_io_service.stop();
    if (m_io_service_thread.joinable())
    {
        m_io_service_thread.join();
    }
//    if (m_io_service_thread.joinable())
//	{
//		m_io_service_thread.join();
//	}
    //m_protocol.stop();
}

void GS::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}

void GS::read_settings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}


void GS::set_uav_address(std::string const& address)
{
    m_uav_address = address;
    QSettings settings;
    settings.setValue("address", address.c_str());

    m_ui.statusBar->showMessage(q::util::format2<std::string>("Connecting to {}", address).c_str(), 2000);
}

void GS::process()
{
    if (!m_comms.is_connected() && !m_uav_address.empty())
    {
        m_comms.start(boost::asio::ip::address::from_string(m_uav_address), 52521, 52520);
    }

    m_comms.process();
    //m_protocol.process();
	m_video_client->process();

    auto now = q::Clock::now();
    auto dt = now - m_last_time;
	m_last_time = now;

	q::System::inst().get_renderer()->begin_frame();

    m_ui.sensors->process();
    m_ui.uav_inertial->process();
    m_ui.video_tab->process();

    //m_comms.send_camera_input(silk::camera_input::Input::STREAM_QUALITY, silk::camera_input::Stream_Quality::LOW);

    m_ui.input->process(m_comms);

    m_input_mgr->update(dt);

//	{
//		std::string  msg;
//		q::util::format(msg, "CPU: {}%", m_protocol.data_board_cpu_usage.value);
//		m_ui.statusBar->showMessage(msg.c_str());
//	}

	q::System::inst().get_renderer()->end_frame();
}


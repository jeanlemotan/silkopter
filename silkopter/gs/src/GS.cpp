#include "stdafx.h"
#include "GS.h"

GS::GS(QWidget *parent)
	: QMainWindow(parent)
    //, m_gyro_calibrarion_samples(k_calibration_sample_count)
    , m_comms(m_io_service, m_hal)
{
//	m_ui.setupUi(this);

    m_last_time = q::Clock::now();

	auto* timer = new QTimer(this);
	timer->setSingleShot(false);
    timer->start(10);
	connect(timer, &QTimer::timeout, this, &GS::process);

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

    set_remote_address("10.10.10.10");
    //set_uav_address("192.168.1.110");

//    connect(m_ui.action_connect_uav, &QAction::triggered, [this](bool) { set_uav_address("192.168.1.110"); });
//    connect(m_ui.action_connect_uav_ah, &QAction::triggered, [this](bool) { set_uav_address("10.10.10.10"); });
//    connect(m_ui.action_connect_simulator, &QAction::triggered, [this](bool) { set_uav_address("127.0.0.1"); });

//    connect(m_ui.action_setup_motors, &QAction::triggered, [this](bool) { setup_motors(); });

    read_settings();
}

GS::~GS()
{
    m_comms.disconnect();

	m_stop_io_service_thread = true;
    m_io_service.stop();
    if (m_io_service_thread.joinable())
    {
        m_io_service_thread.join();
    }
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
        m_comms.start(boost::asio::ip::address::from_string(m_remote_address), 52521, 52520);
    }

    m_comms.process();

    auto now = q::Clock::now();
    auto dt = now - m_last_time;
	m_last_time = now;

	q::System::inst().get_renderer()->begin_frame();

//    m_input_mgr->update(dt);

	q::System::inst().get_renderer()->end_frame();
}


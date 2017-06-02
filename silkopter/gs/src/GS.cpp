#include "GS.h"

#include "value_editors/Value_Editor_Factory.h"
#include "utils/Clock.h"

#include <QInputDialog>
#include <QMessageBox>

static GL_Widget* s_gl_widget = nullptr;

void bind_context()
{
    s_gl_widget->makeCurrent();
}


GS::GS(QWidget *parent)
    : QMainWindow(parent)
//    , m_hal(m_comms)
    , m_ts()
    , m_comms(m_ts)
{
    QGLFormat format(QGL::DepthBuffer | QGL::Rgba | QGL::NoStencilBuffer | QGL::SingleBuffer);
    format.setSwapInterval(0);
    QGLFormat::setDefaultFormat(format);

    m_editor_factory = std::make_shared<Value_Editor_Factory>();
    m_editor_factory->register_standard_editors();

    m_ui.setupUi(this);

    s_gl_widget = m_ui.gl_widget;
    bind_context();

    QObject::connect(m_ui.tabWidget, &QTabWidget::currentChanged, [this](int index)
    {
        for (int i = 0; i < m_ui.tabWidget->count(); i++)
        {
            if (ITab_Widget* t = dynamic_cast<ITab_Widget*>(m_ui.tabWidget->widget(i)))
            {
                t->set_active(false);
            }
        }
        if (ITab_Widget* t = dynamic_cast<ITab_Widget*>(m_ui.tabWidget->widget(index)))
        {
            t->set_active(true);
        }
    });

    m_ui.uav_descriptor_widget->init(m_ui.toolbar, m_comms, *m_ui.properties_browser);
    m_ui.nodes_widget->init(m_ui.toolbar, m_comms, *m_ui.properties_browser);
    m_ui.properties_browser->init(m_editor_factory);

    m_process_last_tp = Clock::now();

    auto* timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->start(1);
    connect(timer, &QTimer::timeout, this, &GS::process, Qt::QueuedConnection);

    show();

    {
        QSettings settings;
        set_remote_address(settings.value("address", "").toString().toLatin1().data());
    }

    QObject::connect(m_ui.action_connect, &QAction::triggered, [this] (bool triggered)
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Enter UAV IP", "IP", QLineEdit::Normal, m_remote_address.c_str(), &ok);
        if (ok)
        {
            set_remote_address(text.toLatin1().data());
        }
    });

    m_comms.sig_type_system_reset.connect([this]()
    {
       m_ui.uav_descriptor_widget->refresh();
       m_ui.nodes_widget->refresh();
    });

    //set_remote_address("10.10.10.10");
    //set_remote_address("127.0.0.1");

    read_settings();
}

GS::~GS()
{
//    m_comms.disconnect();
}

void GS::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
//    settings.setValue("hal_window/geometry", m_hal_window->saveGeometry());
//    settings.setValue("hal_window/windowState", m_hal_window->saveState());
//    settings.setValue("multi_config_window/geometry", m_multi_config_window->saveGeometry());
//    settings.setValue("multi_config_window/windowState", m_multi_config_window->saveState());
//    settings.setValue("sim_window/geometry", m_sim_window->saveGeometry());
//    settings.setValue("sim_window/windowState", m_sim_window->saveState());

    QMainWindow::closeEvent(event);
}

void GS::read_settings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
//    m_hal_window->restoreGeometry(settings.value("hal_window/geometry").toByteArray());
//    m_hal_window->restoreState(settings.value("hal_window/windowState").toByteArray());
//    m_multi_config_window->restoreGeometry(settings.value("multi_config_window/geometry").toByteArray());
//    m_multi_config_window->restoreState(settings.value("multi_config_window/windowState").toByteArray());
//    m_sim_window->restoreGeometry(settings.value("sim_window/geometry").toByteArray());
//    m_sim_window->restoreState(settings.value("sim_window/windowState").toByteArray());
}


void GS::set_remote_address(std::string const& address)
{
    if (address.empty())
    {
        m_remote_address = address;
    }
    else
    {
        try
        {
            boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(address);
            m_comms.start_udp(ip_address, 8006, 8005);

            m_remote_address = address;
            QSettings settings;
            settings.setValue("address", address.c_str());
        }
        catch(std::exception const& e)
        {
            QMessageBox::critical(this, "Error", ("Address '" + m_remote_address + "' is invalid: " + e.what()).c_str());
        }
    }
}

void GS::process()
{
    if (m_comms.is_connected())
    {
        m_ui.statusbar->showMessage(("Connected to " + m_remote_address).c_str());
    }
    else
    {
        m_ui.statusbar->showMessage("");
    }
//    if (!m_comms.is_connected() && !m_remote_address.empty())
//    {
//        m_comms.start_udp(boost::asio::ip::address::from_string(m_remote_address), 8001, 8000);
//        //m_comms.start_rfmon("wlp0s20u1u4", 3);
//    }


    bind_context();

    m_comms.process_rcp();

    auto now = Clock::now();
    auto dt = now - m_process_last_tp;
    if (dt >= std::chrono::milliseconds(33))
    {
        m_process_last_tp = now;

        if (q::System::has_inst())
        {
            q::System::inst().get_renderer()->begin_frame();
        }

        m_comms.process();
        m_ui.nodes_widget->process();

        if (q::System::has_inst())
        {
            q::System::inst().get_renderer()->end_frame();
        }
    }
}


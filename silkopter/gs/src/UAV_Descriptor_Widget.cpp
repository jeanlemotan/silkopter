#include "UAV_Descriptor_Widget.h"
#include "Comms.h"
#include "Properties_Browser.h"

#include "ui_UAV_Descriptor_Widget.h"

#include <QMessageBox>


UAV_Descriptor_Widget::UAV_Descriptor_Widget()
    : m_ui(new Ui_UAV_Descriptor_Widget())
{
    m_ui->setupUi(this);
}

UAV_Descriptor_Widget::~UAV_Descriptor_Widget()
{
    m_ui.reset();
}

void UAV_Descriptor_Widget::init(QToolBar* toolbar, silk::Comms& comms, Properties_Browser& browser)
{
    QASSERT(m_comms == nullptr);
    m_toolbar = toolbar;
    m_comms = &comms;
    m_browser = &browser;
}

void UAV_Descriptor_Widget::show_new_descriptor_menu()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }

    std::shared_ptr<const ts::IStruct_Type> base_type = m_comms->get_type_system().get_root_scope()->find_specialized_symbol_by_path<ts::IStruct_Type>("IUAV_Descriptor");
    if (!base_type)
    {
        QMessageBox::critical(this, "Error", "This UAV doesn't support any descriptors");
        return;
    }

    QMenu menu;

    std::vector<std::shared_ptr<const ts::IStruct_Type>> types = base_type->get_all_inheriting_types();
    for (std::shared_ptr<const ts::IStruct_Type> type: types)
    {
        if (type->is_public() && type->get_all_inheriting_types().empty()) //only show public, final types
        {
            QAction* action = menu.addAction(type->get_ui_name().c_str());
            QObject::connect(action, &QAction::triggered, [this, type](bool) { create_descriptor(type); });
        }
    }

    menu.exec(QCursor::pos());
}

void UAV_Descriptor_Widget::set_active(bool active)
{
    if (!active)
    {
        if (m_refresh_action)
        {
            delete m_refresh_action;
            m_refresh_action = nullptr;

            delete m_upload_action;
            m_upload_action = nullptr;

            delete m_new_action;
            m_new_action = nullptr;
        }
        return;
    }
    m_refresh_action = m_toolbar->addAction(QIcon(":/icons/ui/reconnect.png"), "Refresh");
    QObject::connect(m_refresh_action, &QAction::triggered, [this](bool) { refresh(); });

    m_upload_action = m_toolbar->addAction(QIcon(":/icons/ui/upload.png"), "Upload");
    QObject::connect(m_upload_action, &QAction::triggered, [this](bool) { upload(); });

    m_new_action = m_toolbar->addAction(QIcon(":/icons/ui/new.png"), "New");
    QObject::connect(m_new_action, &QAction::triggered, [this](bool) { show_new_descriptor_menu(); });

    m_browser->set_value(m_descriptor);
}

void UAV_Descriptor_Widget::create_descriptor(std::shared_ptr<const ts::IStruct_Type> type)
{
    QASSERT(type);
    if (!type)
    {
        return;
    }

    std::shared_ptr<ts::IStruct_Value> descriptor = type->create_specialized_value();
    QASSERT(descriptor);
    auto construction_result = descriptor->construct();
    if (construction_result != ts::success)
    {
        QMessageBox::critical(this, "Error", ("Type System Error: " + construction_result.error().what()).c_str());
        return;
    }

    m_descriptor = descriptor;
    m_browser->set_value(m_descriptor);
}

void UAV_Descriptor_Widget::upload()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }
    auto result = m_comms->send_uav_descriptor(m_descriptor);
    if (result != ts::success)
    {
        QMessageBox::critical(this, "Error", result.error().what().c_str());
    }
    else
    {
        m_descriptor = result.payload();
        m_browser->set_value(m_descriptor);
    }
}

void UAV_Descriptor_Widget::refresh()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }
    auto result = m_comms->request_uav_descriptor();
    if (result != ts::success)
    {
        QMessageBox::critical(this, "Error", result.error().what().c_str());
    }
    else
    {
        m_descriptor = result.payload();
        m_browser->set_value(m_descriptor);
    }
}


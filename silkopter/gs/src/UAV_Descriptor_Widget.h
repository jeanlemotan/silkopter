#pragma once

#include <QToolBar>
#include "ITab_Widget.h"

namespace silk
{
class Comms;
}

class Properties_Browser;
class Ui_UAV_Descriptor_Widget;

class UAV_Descriptor_Widget : public ITab_Widget
{
public:
    UAV_Descriptor_Widget();
    ~UAV_Descriptor_Widget();

    void init(QToolBar* toolbar, silk::Comms& comms, Properties_Browser& browser);

    void refresh();

private:
    void set_active(bool active) override;

    void upload();
    void create_descriptor(std::shared_ptr<const ts::IStruct_Type> type);
    void show_new_descriptor_menu();

    QToolBar* m_toolbar = nullptr;
    QAction* m_refresh_action = nullptr;
    QAction* m_upload_action = nullptr;
    QAction* m_new_action = nullptr;

    silk::Comms* m_comms = nullptr;
    Properties_Browser* m_browser = nullptr;

    std::unique_ptr<Ui_UAV_Descriptor_Widget> m_ui;

    std::shared_ptr<ts::IStruct_Value> m_descriptor;
};

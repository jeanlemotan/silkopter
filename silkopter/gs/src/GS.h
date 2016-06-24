#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GS.h"

#include "Comms.h"

class Value_Editor_Factory;


class GS : public QMainWindow
{
public:
    GS(QWidget *parent = 0);
    ~GS();

private:
    void process();

//    void keyPressEvent(QKeyEvent* event);
//    void keyReleaseEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);

    void read_settings();

    q::Clock::time_point m_process_last_tp;

    Ui::GS m_ui;

//    silk::HAL m_hal;
    ts::Type_System m_ts;
    silk::Comms m_comms;

    std::shared_ptr<Value_Editor_Factory> m_editor_factory;

    void set_remote_address(std::string const& address);
    std::string m_remote_address;
};



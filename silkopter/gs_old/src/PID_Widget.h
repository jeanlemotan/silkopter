#pragma once

#include "ui_PID_Widget.h"
#include "Comms.h"

class PID_Widget : public QWidget
{
public:
    PID_Widget(QWidget *parent = 0);

public:
    void init(silk::Comms* comms);

private:
    void request_pids();
    void send_pids();

    silk::Comms* m_comms = nullptr;
    Ui::PID_Widget m_ui;
};



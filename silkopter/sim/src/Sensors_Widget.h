#pragma once

#include "Config.h"
#include "ui_Sensors_Widget.h"

class Sensors_Widget : public QWidget
{
public:
    Sensors_Widget(QWidget* parent = 0);
    void set_config(Config* config);
    void notify_config_changed();

private:

    Config m_dummy_config;
    Config* m_config = nullptr;
    Ui::Sensors_Widget m_ui;
};


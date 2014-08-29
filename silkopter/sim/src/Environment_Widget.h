#pragma once

#include "Config.h"
#include "ui_Environment_Widget.h"

class Environment_Widget : public QWidget
{
public:
    Environment_Widget(QWidget* parent = 0);

    void set_config(Config* config);
    void notify_config_changed();

private:
    Config m_dummy_config;
    Config* m_config = nullptr;

    Ui::Environment_Widget m_ui;
};


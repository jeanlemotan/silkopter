#include "stdafx.h"
#include "Environment_Widget.h"

Environment_Widget::Environment_Widget(QWidget* parent)
    : QWidget(parent)
    , m_config(&m_dummy_config)
{
    m_ui.setupUi(this);

    notify_config_changed();

    //environment
    QObject::connect(m_ui.ground_enabled, &QCheckBox::stateChanged, [this](int v) { m_config->environment.is_ground_enabled = v != 0; });
    QObject::connect(m_ui.gravity_enabled, &QCheckBox::stateChanged, [this](int v) { m_config->environment.is_gravity_enabled = v != 0; });
}

void Environment_Widget::set_config(Config* config)
{
    m_config = config ? config : &m_dummy_config;
}

void Environment_Widget::notify_config_changed()
{
    m_ui.ground_enabled->setChecked(m_config->environment.is_ground_enabled);
    m_ui.gravity_enabled->setChecked(m_config->environment.is_gravity_enabled);
}



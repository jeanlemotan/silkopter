#pragma once

#include "ui_Status_Widget.h"
#include "UAV.h"

class Status_Widget : public QWidget
{
public:
	Status_Widget(QWidget* parent = 0);

    void process(UAV const& uav);

private:
    Ui::Status_Widget m_ui;
};


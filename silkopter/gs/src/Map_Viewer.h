#pragma once

#include "HAL.h"
#include "Comms.h"
//#include "ui_Map_Viewer.h"

class Map_Viewer : public QWidget
{
public:
    Map_Viewer(QWidget *parent);
    ~Map_Viewer();
    void add_samples(q::Clock::time_point tp, math::vec2d const& lat_lon);
    void process();

private:
    void show_context_menu(QPoint const& point);

    qmapcontrol::MapControl* m_map_control = nullptr;

    //Ui::Map_Viewer m_ui;
    bool m_is_started = false;

    struct Sample
    {
        math::vec2d lat_long;
        double tp;
    };

    std::vector<Sample> m_samples;
};

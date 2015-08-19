#pragma once

#include "HAL.h"
#include "Comms.h"
//#include "ui_Map_Viewer.h"


namespace qmapcontrol
{
    class MapControl;
    class MapAdapter;
    class MapLayer;
}


class Map_Viewer : public QWidget
{
public:
    Map_Viewer(QWidget *parent);
    ~Map_Viewer();
    void add_sample(math::vec3d const& position);
    void process();

private:
    void show_context_menu(QPoint const& point);

    qmapcontrol::MapControl* m_map_control = nullptr;
    qmapcontrol::MapAdapter* m_map_adapter = nullptr;
    qmapcontrol::MapLayer* m_map_layer = nullptr;

    enum class Map_Type
    {
        GOOGLE_MAPS,
        GOOGLE_SATELLITE,
        GOOGLE_TERRAIN,
        OSM
    };
    Map_Type m_map_type;

    void set_map_type(Map_Type type);


    //Ui::Map_Viewer m_ui;
    bool m_is_started = false;

    struct Sample
    {
        math::vec3d position;
        double accuracy;
//        double tp;
    };

    std::vector<Sample> m_samples;
//    double m_tp = 0;

    bool m_is_following_sample = true;
};

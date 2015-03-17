#include "stdafx.h"
#include "Map_Viewer.h"
#include "HAL.h"
#include "Comms.h"

Map_Viewer::Map_Viewer(QWidget *parent)
    : QWidget(parent)
{
    QFont font;
    font.setPointSize(8);

    m_map_control = new qmapcontrol::MapControl(this);
    m_map_control->showScale(true);
    m_map_control->enablePersistentCache(QDir("./cache"), 1000);

    m_map_adapter = new qmapcontrol::GoogleMapAdapter(qmapcontrol::GoogleMapAdapter::maps);

    // create mapadapter, for mainlayer and overlay
    //auto mapadapter = new qmapcontrol::OSMMapAdapter();

    // create a layer with the mapadapter and type MapLayer
    m_map_layer = new qmapcontrol::MapLayer("Map-Layer", m_map_adapter);
//    mainlayer->setMapAdapter(mapadapter);
//    sights->setMapAdapter(mapadapter);
//    museum->setMapAdapter(mapadapter);
//    pubs->setMapAdapter(mapadapter);
//    notes->setMapAdapter(mapadapter);
//    m_map_control->updateRequestNew();

    // create a layer with the mapadapter and type MapLayer
//    qmapcontrol::Layer* l = new qmapcontrol::Layer("Custom Layer", m_map_adapter, qmapcontrol::Layer::MapLayer);

//    //current GPS Location
//    auto gpsDot = new qmapcontrol::CirclePoint(0, 0, 10, "GPS", qmapcontrol::CirclePoint::Middle, new QPen( Qt::darkBlue ));
//    l->addGeometry(gpsDot);

//    // add Layer to the MapControl
//    m_map_control->addLayer(l);

    // add Layer to the MapControl
    m_map_control->addLayer(m_map_layer);

//    addZoomButtons();

    m_map_control->setView(QPointF(0,0));
    m_map_control->setZoom(2);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_map_control);

    // myWidget is any QWidget-derived class
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(mapToGlobal(pos));
    });
}

Map_Viewer::~Map_Viewer()
{
}

void Map_Viewer::set_map_type(Map_Type type)
{
    if (m_map_type == type && m_map_adapter != nullptr)
    {
        return;
    }

    int zoom = m_map_adapter->currentZoom();
    delete m_map_adapter;

    switch (type)
    {
        case Map_Type::GOOGLE_MAPS:
        {
            m_map_adapter = new qmapcontrol::GoogleMapAdapter(qmapcontrol::GoogleMapAdapter::maps);
        }
        break;
        case Map_Type::GOOGLE_SATELLITE:
        {
            m_map_adapter = new qmapcontrol::GoogleMapAdapter(qmapcontrol::GoogleMapAdapter::satellite);
        }
        break;
        case Map_Type::GOOGLE_TERRAIN:
        {
            m_map_adapter = new qmapcontrol::GoogleMapAdapter(qmapcontrol::GoogleMapAdapter::terrain);
        }
        break;
        case Map_Type::OSM:
        {
            m_map_adapter = new qmapcontrol::OSMMapAdapter();
        }
        break;
    }

    m_map_layer->setMapAdapter(m_map_adapter);
    m_map_control->setZoom(zoom);
    m_map_type = type;
}

void Map_Viewer::show_context_menu(QPoint const& pos)
{
    QMenu menu;

    {
        auto* action = menu.addAction(QIcon(":/icons/focus.png"), "Follow Sample");
        action->setCheckable(true);
        action->setChecked(m_is_following_sample);
        connect(action, &QAction::toggled, [this](bool yes)
        {
            m_is_following_sample = yes;
            if (yes && !m_samples.empty())
            {
                auto const& lat_lon = m_samples.back().lat_lon;
                m_map_control->setView(QPointF(lat_lon.y, lat_lon.x));
            }
        });

        menu.addSeparator();
    }

    auto* sm = menu.addMenu(QIcon(":/icons/map.png"), "Show");
    auto* action = sm->addAction("Google Maps");
    action->setCheckable(true);
    action->setChecked(m_map_type == Map_Type::GOOGLE_MAPS);
    connect(action, &QAction::toggled, [this](bool yes) { set_map_type(Map_Type::GOOGLE_MAPS); });
    action = sm->addAction("Google Satellite");
    action->setCheckable(true);
    action->setChecked(m_map_type == Map_Type::GOOGLE_SATELLITE);
    connect(action, &QAction::toggled, [this](bool yes) { set_map_type(Map_Type::GOOGLE_SATELLITE); });
    action = sm->addAction("Google Terrain");
    action->setCheckable(true);
    action->setChecked(m_map_type == Map_Type::GOOGLE_TERRAIN);
    connect(action, &QAction::toggled, [this](bool yes) { set_map_type(Map_Type::GOOGLE_TERRAIN); });
    action = sm->addAction("Open Street Maps");
    action->setCheckable(true);
    action->setChecked(m_map_type == Map_Type::OSM);
    connect(action, &QAction::toggled, [this](bool yes) { set_map_type(Map_Type::OSM); });

    menu.exec(pos);
}

void Map_Viewer::add_sample(q::Clock::time_point tp, math::vec2d const& lat_lon)
{
    double tpd = std::chrono::duration<double>(tp.time_since_epoch()).count();
    if (tpd < m_tp)
    {
        QLOGE("Sample from the past!!");
        return;
    }

    if (!m_map_control->isPanning() && m_is_following_sample)
    {
        m_map_control->setView(QPointF(lat_lon.y, lat_lon.x));
    }
    else
    {
        m_is_following_sample = false;
    }
    Sample s;
    s.lat_lon = lat_lon;
    s.tp = tpd;
    m_samples.push_back(s);
}

void Map_Viewer::process()
{
    if (m_map_control->isPanning())
    {
        m_is_following_sample = false;
    }

    m_map_control->resize(size());
}


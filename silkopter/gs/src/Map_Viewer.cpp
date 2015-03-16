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

    auto mapadapter = new qmapcontrol::GoogleMapAdapter(qmapcontrol::GoogleMapAdapter::maps);

    // create mapadapter, for mainlayer and overlay
    //auto mapadapter = new qmapcontrol::OSMMapAdapter();

    // create a layer with the mapadapter and type MapLayer
    auto mainlayer = new qmapcontrol::MapLayer("OpenStreetMap-Layer", mapadapter);
//    mainlayer->setMapAdapter(mapadapter);
//    sights->setMapAdapter(mapadapter);
//    museum->setMapAdapter(mapadapter);
//    pubs->setMapAdapter(mapadapter);
//    notes->setMapAdapter(mapadapter);
//    m_map_control->updateRequestNew();

    // create a layer with the mapadapter and type MapLayer
    qmapcontrol::Layer* l = new qmapcontrol::Layer("Custom Layer", mapadapter, qmapcontrol::Layer::MapLayer);

    //current GPS Location
    auto gpsDot = new qmapcontrol::CirclePoint(0, 0, 10, "GPS", qmapcontrol::CirclePoint::Middle, new QPen( Qt::darkBlue ));
    l->addGeometry(gpsDot);

    // add Layer to the MapControl
    m_map_control->addLayer(l);

    // add Layer to the MapControl
    m_map_control->addLayer(mainlayer);

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

void Map_Viewer::show_context_menu(QPoint const& pos)
{
    QMenu menu;

    menu.addSeparator();

    menu.exec(pos);
}

void Map_Viewer::add_samples(q::Clock::time_point tp, math::vec2d const& lat_lon)
{
//    gpsposition->setText(QString::number(time) + " / " + QString::number(coordinate.x()) + " / " + QString::number(coordinate.y()));
    m_map_control->setView(QPointF(lat_lon.y, lat_lon.x));

    //update the gps dot location on map
    //gpsDot->setCoordinate(coordinate);
}

void Map_Viewer::process()
{
    m_map_control->resize(size());
}


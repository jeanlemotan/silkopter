#include "stdafx.h"
#include "Map_Viewer.h"
#include "HAL.h"
#include "Comms.h"

Map_Viewer::Map_Viewer(QWidget *parent)
    : QWidget(parent)
{
    QFont font;
    font.setPointSize(8);

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
}

void Map_Viewer::process()
{
}


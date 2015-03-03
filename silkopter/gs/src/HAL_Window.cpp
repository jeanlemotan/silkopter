#include "stdafx.h"
#include "HAL_Window.h"

#include "qneblock.h"
#include "qnodeseditor.h"

#include <QGraphicsScene>
#include <QFileDialog>

#include "qneport.h"

#include "boost/algorithm/string.hpp"

HAL_Window::HAL_Window(silk::HAL& hal, QWidget *parent)
    : QMainWindow(parent)
    , m_hal(hal)
{
    m_scene = new QGraphicsScene();

    setWindowTitle("HAL Editor");


    QDockWidget *dock = new QDockWidget(tr("Nodes"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_view = new QGraphicsView(dock);
    m_view->setScene(m_scene);

    m_view->setRenderHint(QPainter::Antialiasing, true);

    dock->setWidget(m_view);
    addDockWidget(Qt::LeftDockWidgetArea, dock);


    m_nodesEditor = new QNodesEditor(this);
    m_nodesEditor->install(m_scene);

    m_hal.node_defs_refreshed_signal.connect(std::bind(&HAL_Window::on_node_factories_refreshed, this));
    //m_hal.nodes_refreshed_signal.connect(std::bind(&HAL_Window::on_nodes_refreshed, this));
}

HAL_Window::~HAL_Window()
{

}

std::string prettify_name(std::string const& name)
{
    std::string out = name;
    boost::trim(out);
    char old_c = 0;
    for (char& c: out)
    {
        if (c == '_')
        {
            c = ' ';
        }
        else if (std::isalpha(c) && old_c == ' ')
        {
            c = toupper(c);
        }
    }
    if (!out.empty())
    {
        out[0] = toupper(out[0]);
    }

    return out;
}

void HAL_Window::on_node_factories_refreshed()
{
}

void HAL_Window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sources");
        auto nodes = m_hal.get_source_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_source(n); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sinks");
        auto nodes = m_hal.get_sink_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_sink(n); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Processors");
        auto nodes = m_hal.get_processor_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_processor(n); });
        }
    }

    menu.exec(event->globalPos());
}

void HAL_Window::create_source(std::shared_ptr<silk::node::source::GS_ISource> def)
{
    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(prettify_name(def->name).c_str());
    b->setPos(m_view->sceneRect().center().toPoint());
    b->setBrush(QBrush(QColor(QRgb(0xf1c40f))));

    for (auto const& o: def->outputs)
    {
        auto port = b->addOutputPort(prettify_name(o.name).c_str());
        port->setPortType(o.class_id);
        port->setBrush(QBrush(QColor(QRgb(0x9b59b6))));
    }
}

void HAL_Window::create_sink(std::shared_ptr<silk::node::sink::GS_ISink> def)
{
    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(prettify_name(def->name).c_str());
    b->setPos(m_view->sceneRect().center().toPoint());
    b->setBrush(QBrush(QColor(QRgb(0xF9690E))));

    for (auto const& i: def->inputs)
    {
        auto port = b->addInputPort(prettify_name(i.name).c_str());
        port->setPortType(i.class_id);
        port->setBrush(QBrush(QColor(QRgb(0xe67e22))));
    }
}

void HAL_Window::create_processor(std::shared_ptr<silk::node::processor::GS_IProcessor> def)
{
    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(prettify_name(def->name).c_str());
    b->setPos(m_view->sceneRect().center().toPoint());
    b->setBrush(QBrush(QColor(QRgb(0x26C281))));

    for (auto const& i: def->inputs)
    {
        auto port = b->addInputPort(prettify_name(i.name).c_str());
        port->setPortType(i.class_id);
        port->setBrush(QBrush(QColor(QRgb(0xe67e22))));
    }
    for (auto const& o: def->outputs)
    {
        auto port = b->addOutputPort(prettify_name(o.name).c_str());
        port->setPortType(o.class_id);
        port->setBrush(QBrush(QColor(QRgb(0x9b59b6))));
    }
}

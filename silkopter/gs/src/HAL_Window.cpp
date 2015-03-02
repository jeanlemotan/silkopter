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

    m_hal.node_factories_refreshed_signal.connect(std::bind(&HAL_Window::on_node_factories_refreshed, this));
    //m_hal.nodes_refreshed_signal.connect(std::bind(&HAL_Window::on_nodes_refreshed, this));

//    QNEBlock *b = new QNEBlock(0);
//    m_scene->addItem(b);
//    b->addPort("test", 0, QNEPort::NamePort);
//    b->addPort("TestBlock", 0, QNEPort::TypePort);
//    b->addInputPort("in1");
//    b->addInputPort("in2");
//    b->addInputPort("in3");
//    b->addOutputPort("out1");
//    b->addOutputPort("out2");
//    b->addOutputPort("out3");

//    b = b->clone();
//    b->setPos(150, 0);

//    b = b->clone();
//    b->setPos(150, 150);
}

HAL_Window::~HAL_Window()
{

}

std::string prettify_name(std::string const& name)
{
    std::string out = name;
    boost::trim(out);
    for (size_t i = 0; i < out.size(); i++)
    {
        if (out[i] == '_')
        {
            out[i] = ' ';
        }
        else if (std::isalpha(out[i]) && i > 0 && out[i - 1] == ' ')
        {
            out[i] = toupper(out[i]);
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
//    auto buses = m_hal.get_bus_factory().get_all();
//    for (auto const& n: buses)
//    {
//        QNEBlock *b = new QNEBlock(0);
//        m_scene->addItem(b);
//        b->addPort(n->name.c_str(), 0, QNEPort::NamePort);
//        //b->addPort(, 0, QNEPort::TypePort);
//        b->setPos(150, 150);
//    }
    {
        auto nodes = m_hal.get_source_factory().get_all();
        for (auto const& n: nodes)
        {
        }
    }
    {
        auto nodes = m_hal.get_sink_factory().get_all();
        for (auto const& n: nodes)
        {
        }
    }
    {
    }
}

void HAL_Window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sources");
        auto nodes = m_hal.get_source_factory().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_source(n); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sinks");
        auto nodes = m_hal.get_sink_factory().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_sink(n); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Processors");
        auto nodes = m_hal.get_processor_factory().get_all();
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
    QNEBlock *b = new QNEBlock(0);
    m_scene->addItem(b);
    b->addPort(prettify_name(def->name).c_str(), 0, QNEPort::NamePort);
    //b->addPort(, 0, QNEPort::TypePort);
    b->setPos(150, 150);

    for (auto const& o: def->outputs)
    {
        b->addPort(prettify_name(o.name).c_str(), true, 0, 0);
        b->setPos(m_view->sceneRect().center().toPoint());
    }
}

void HAL_Window::create_sink(std::shared_ptr<silk::node::sink::GS_ISink> def)
{
    QNEBlock *b = new QNEBlock(0);
    m_scene->addItem(b);
    b->addPort(prettify_name(def->name).c_str(), 0, QNEPort::NamePort);
    //b->addPort(, 0, QNEPort::TypePort);
    b->setPos(150, 150);

    for (auto const& i: def->inputs)
    {
        b->addPort(prettify_name(i.name).c_str(), false, 0, 0);
        b->setPos(m_view->sceneRect().center().toPoint());
    }
}

void HAL_Window::create_processor(std::shared_ptr<silk::node::processor::GS_IProcessor> def)
{
    QNEBlock *b = new QNEBlock(0);
    m_scene->addItem(b);
    b->addPort(prettify_name(def->name).c_str(), 0, QNEPort::NamePort);
    //b->addPort(, 0, QNEPort::TypePort);
    b->setPos(150, 150);

    for (auto const& i: def->inputs)
    {
        b->addPort(prettify_name(i.name).c_str(), false, 0, 0);
        b->setPos(m_view->sceneRect().center().toPoint());
    }
    for (auto const& o: def->outputs)
    {
        b->addPort(prettify_name(o.name).c_str(), true, 0, 0);
        b->setPos(m_view->sceneRect().center().toPoint());
    }
}

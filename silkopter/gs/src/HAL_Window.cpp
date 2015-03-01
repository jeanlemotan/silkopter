#include "stdafx.h"
#include "HAL_Window.h"

#include "qneblock.h"
#include "qnodeseditor.h"

#include <QGraphicsScene>
#include <QFileDialog>

#include "qneport.h"

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
            QNEBlock *b = new QNEBlock(0);
            m_scene->addItem(b);
            b->addPort(n->name.c_str(), 0, QNEPort::NamePort);
            //b->addPort(, 0, QNEPort::TypePort);
            b->setPos(150, 150);

            for (auto const& o: n->outputs)
            {
                b->addPort(o.name.c_str(), true, 0, 0);
                b->setPos(m_view->sceneRect().center().toPoint());
            }
        }
    }
    {
        auto nodes = m_hal.get_sink_factory().get_all();
        for (auto const& n: nodes)
        {
            QNEBlock *b = new QNEBlock(0);
            m_scene->addItem(b);
            b->addPort(n->name.c_str(), 0, QNEPort::NamePort);
            //b->addPort(, 0, QNEPort::TypePort);
            b->setPos(150, 150);

            for (auto const& i: n->inputs)
            {
                b->addPort(i.name.c_str(), false, 0, 0);
                b->setPos(m_view->sceneRect().center().toPoint());
            }
        }
    }
    {
        auto nodes = m_hal.get_processor_factory().get_all();
        for (auto const& n: nodes)
        {
            QNEBlock *b = new QNEBlock(0);
            m_scene->addItem(b);
            b->addPort(n->name.c_str(), 0, QNEPort::NamePort);
            //b->addPort(, 0, QNEPort::TypePort);
            b->setPos(150, 150);

            for (auto const& i: n->inputs)
            {
                b->addPort(i.name.c_str(), false, 0, 0);
                b->setPos(m_view->sceneRect().center().toPoint());
            }
            for (auto const& o: n->outputs)
            {
                b->addPort(o.name.c_str(), true, 0, 0);
                b->setPos(m_view->sceneRect().center().toPoint());
            }
        }
    }
}

//void HAL_Window::addBlock()
//{
//    QNEBlock *b = new QNEBlock(0);

//    m_scene->addItem(b);
//	static const char* names[] = {"Vin", "Voutsadfasdf", "Imin", "Imax", "mul", "add", "sub", "div", "Conv", "FFT"};
//	for (int i = 0; i < 4 + rand() % 3; i++)
//	{
//		b->addPort(names[rand() % 10], rand() % 2, 0, 0);
//        b->setPos(m_view->sceneRect().center().toPoint());
//	}
//}

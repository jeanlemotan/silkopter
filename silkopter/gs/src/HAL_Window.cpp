#include "stdafx.h"
#include "HAL_Window.h"

#include "ModelTest.h"
#include "json_editor/JSON_Model.h"
#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneconnection.h"
#include "qneport.h"

#include <QGraphicsScene>
#include <QFileDialog>

#include "boost/algorithm/string.hpp"

#include "ui_New_Node.h"

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
//    m_view->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

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

void HAL_Window::on_node_factories_refreshed()
{
}

void HAL_Window::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    auto pos = QPointF(m_view->mapToScene(event->pos()));


    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sources");
        auto nodes = m_hal.get_source_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), n->name.c_str());
            connect(action, &QAction::triggered, [=](bool) { create_source(n, pos); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sinks");
        auto nodes = m_hal.get_sink_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), n->name.c_str());
            connect(action, &QAction::triggered, [=](bool) { create_sink(n, pos); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Processors");
        auto nodes = m_hal.get_processor_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), n->name.c_str());
            connect(action, &QAction::triggered, [=](bool) { create_processor(n, pos); });
        }
    }

    menu.exec(event->globalPos());
}

void HAL_Window::refresh_source(silk::node::source::Source& node)
{
    auto& data = m_nodes[node.name];
    for (auto const& o: node.outputs)
    {
        auto& od = data.outputs[o.name];
        QASSERT(od.port);
        od.port->setName(q::util::format2<std::string>("{}", o.name).c_str());
        od.port->setPortType(q::util::format2<std::string>("{}", o.class_id).c_str());
    }
    data.block->refreshGeometry();
}

void HAL_Window::add_source(silk::node::source::Source_ptr node, QPointF pos)
{
    QASSERT(node);

    auto& data = m_nodes[node->name];
    data = Node_Data();

    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);

    b->setName(node->name.c_str());
    b->setPos(pos);
    b->setBrush(QBrush(QColor(0xf1c40f)));
    for (auto const& o: node->outputs)
    {
        auto port = b->addOutputPort(QString());
        port->setBrush(QBrush(QColor(0x9b59b6)));
        data.outputs[o.name].port.reset(port);
    }
    data.block.reset(b);

    refresh_source(*node);
    node->changed_signal.connect([this](silk::node::source::Source& node)
    {
        refresh_source(node);
    });
}

void HAL_Window::create_source(silk::node::source::Source_Def_ptr def, QPointF pos)
{
    auto init_params = jsonutil::clone_value(def->default_init_params);

    QDialog dialog;
    dialog.setLayout(new QVBoxLayout(&dialog));

    Ui::New_Node ui;
    QWidget* widget = new QWidget(&dialog);
    ui.setupUi(widget);
    widget->setMinimumSize(600, 400);
    dialog.layout()->addWidget(widget);

    JSON_Model* model = new JSON_Model(ui.init_params);
    model->set_document("Init Params", &init_params);
    ui.init_params->setModel(model);
    ui.init_params->expandAll();
    ui.init_params->header()->resizeSections(QHeaderView::ResizeToContents);

    connect(ui.ok, &QPushButton::released, &dialog, &QDialog::accept);
    connect(ui.cancel, &QPushButton::released, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_hal.add_source(def->name, def->name, std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::source::Source_ptr node)
        {
            if (result == silk::HAL::Result::OK)
            {
                add_source(node, pos);
            }
        });
    }
}

void HAL_Window::add_sink(silk::node::sink::Sink_ptr node, QPointF pos)
{
    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(node->name.c_str());
    b->setPos(pos);
    b->setBrush(QBrush(QColor(0x26C281)));
    for (auto const& i: node->inputs)
    {
        auto port = b->addInputPort(i.name.c_str());
        port->setPortType(q::util::format2<std::string>("{}", i.class_id).c_str());
        port->setBrush(QBrush(QColor(0xe67e22)));
    }
}

void HAL_Window::create_sink(silk::node::sink::Sink_Def_ptr def, QPointF pos)
{
    auto init_params = jsonutil::clone_value(def->default_init_params);

    QDialog dialog;
    dialog.setLayout(new QVBoxLayout(&dialog));

    Ui::New_Node ui;
    QWidget* widget = new QWidget(&dialog);
    ui.setupUi(widget);
    widget->setMinimumSize(600, 400);
    dialog.layout()->addWidget(widget);

    JSON_Model* model = new JSON_Model(ui.init_params);
    model->set_document("Init Params", &init_params);
    ui.init_params->setModel(model);
    ui.init_params->expandAll();
    ui.init_params->header()->resizeSections(QHeaderView::ResizeToContents);

    connect(ui.ok, &QPushButton::released, &dialog, &QDialog::accept);
    connect(ui.cancel, &QPushButton::released, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_hal.add_sink(def->name, def->name, std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::sink::Sink_ptr node)
        {
            if (result == silk::HAL::Result::OK)
            {
                add_sink(node, pos);
            }
        });
    }
}

void HAL_Window::refresh_processor(silk::node::processor::Processor& node)
{
    auto& data = m_nodes[node.name];

    for (auto const& i: node.inputs)
    {
        auto& id = data.inputs[i.name];
        QASSERT(id.port);
        id.port->setName(q::util::format2<std::string>("{}", i.name).c_str());
        id.port->setPortType(q::util::format2<std::string>("{}", i.class_id).c_str());
        id.port->disconnectAll();

        //find the connection in the config
        q::Path path("inputs/" + i.name);
        auto* input_streamj = jsonutil::find_value(node.config, path);
        QASSERT(input_streamj && input_streamj->IsString());
        if (input_streamj && input_streamj->IsString())
        {
            std::string input_stream(input_streamj->GetString());
            auto tokens = q::util::tokenize(input_stream, std::string("/"));
            if (tokens.size() == 2)
            {
                auto const& node_name = tokens[0];
                auto const& output_name = tokens[1];
                auto it_node = m_nodes.find(node_name);
                QASSERT(it_node != m_nodes.end());
                if (it_node != m_nodes.end())
                {
                    auto const& outputs = it_node->second.outputs;
                    auto it_output = outputs.find(output_name);
                    QASSERT(it_output != outputs.end());
                    if (it_output != outputs.end())
                    {
                        auto connection = new QNEConnection(0);
                        m_scene->addItem(connection);
                        connection->setPort1((QNEPort*)id.port.get());
                        connection->setPort2(it_output->second.port.get());
                        connection->updatePath();
                    }
                }
            }
        }
    }

    for (auto const& o: node.outputs)
    {
        auto& od = data.outputs[o.name];
        QASSERT(od.port);
        od.port->setName(q::util::format2<std::string>("{} {}Hz", o.name, o.rate).c_str());
        od.port->setPortType(q::util::format2<std::string>("{}", o.class_id).c_str());
    }
    data.block->refreshGeometry();
}

void HAL_Window::add_processor(silk::node::processor::Processor_ptr node, QPointF pos)
{
    auto& data = m_nodes[node->name];
    data = Node_Data();

    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(node->name.c_str());
    b->setPos(pos);
    b->setBrush(QBrush(QColor(0x26C281)));
    for (auto const& i: node->inputs)
    {
        auto port = b->addInputPort(QString());
        port->setBrush(QBrush(QColor(0xe67e22)));
        port->connectedSignal.connect([port, node, this](QNEPort* output_port)
        {
            std::string input_name = port->name().toLatin1().data();
            auto* block = output_port->block();
            std::string node_name = block->name().toLatin1().data();
            std::string stream_name = node_name + "/" + output_port->name().toLatin1().data();
            m_hal.connect_input(node, input_name, stream_name, [](silk::HAL::Result) {});
        });
        data.inputs[i.name].port.reset(port);
    }
    for (auto const& o: node->outputs)
    {
        auto port = b->addOutputPort(QString());
        port->setBrush(QBrush(QColor(0x9b59b6)));
        data.outputs[o.name].port.reset(port);
    }

    data.block.reset(b);

    refresh_processor(*node);
    node->changed_signal.connect([this](silk::node::processor::Processor& node)
    {
        refresh_processor(node);
    });
}

void HAL_Window::create_processor(silk::node::processor::Processor_Def_ptr def, QPointF pos)
{
    auto init_params = jsonutil::clone_value(def->default_init_params);

    QDialog dialog;
    dialog.setLayout(new QVBoxLayout(&dialog));

    Ui::New_Node ui;
    QWidget* widget = new QWidget(&dialog);
    ui.setupUi(widget);
    widget->setMinimumSize(600, 400);
    dialog.layout()->addWidget(widget);

    JSON_Model* model = new JSON_Model(ui.init_params);
    model->set_document("Init Params", &init_params);
    ui.init_params->setModel(model);
    ui.init_params->expandAll();
    ui.init_params->header()->resizeSections(QHeaderView::ResizeToContents);

    connect(ui.ok, &QPushButton::released, &dialog, &QDialog::accept);
    connect(ui.cancel, &QPushButton::released, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_hal.add_processor(def->name, def->name, std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::processor::Processor_ptr node)
        {
            if (result == silk::HAL::Result::OK)
            {
                add_processor(node, pos);
            }
        });
    }
}

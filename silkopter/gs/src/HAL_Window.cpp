#include "stdafx.h"
#include "HAL_Window.h"

#include "ModelTest.h"
#include "json_editor/JSON_Model.h"
#include "qneblock.h"
#include "qnodeseditor.h"

#include <QGraphicsScene>
#include <QFileDialog>

#include "qneport.h"

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
        old_c = c;
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

void HAL_Window::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    auto pos = QPointF(m_view->mapToScene(event->pos()));


    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sources");
        auto nodes = m_hal.get_source_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_source(n, pos); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Sinks");
        auto nodes = m_hal.get_sink_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_sink(n, pos); });
        }
    }

    {
        QMenu* submenu = menu.addMenu(QIcon(), "Processors");
        auto nodes = m_hal.get_processor_defs().get_all();
        for (auto const& n: nodes)
        {
            auto* action = submenu->addAction(QIcon(), prettify_name(n->name).c_str());
            connect(action, &QAction::triggered, [=](bool) { create_processor(n, pos); });
        }
    }

    menu.exec(event->globalPos());
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

    if (dialog.exec() == QDialog::Rejected)
    {
        return;
    }

//    Item_Key item;
//    item.class_id = node->class_id;
//    item.name = node->name;
//    m_ui_items[item] = Item_Data({pos});

    m_hal.add_source(def->name, def->name, std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::source::Source_ptr node)
    {
        if (result == silk::HAL::Result::OK)
        {
            QNEBlock *b = new QNEBlock();
            m_scene->addItem(b);
            b->setName(prettify_name(node->name).c_str());
            b->setPos(pos);
            b->setBrush(QBrush(QColor(0xf1c40f)));
            for (auto const& o: node->outputs)
            {
                auto port = b->addOutputPort(prettify_name(o.name).c_str());
                port->setPortType(o.class_id);
                port->setBrush(QBrush(QColor(QRgb(0x9b59b6))));
            }
        }
    });
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

    if (dialog.exec() == QDialog::Rejected)
    {
        return;
    }

//    Item_Key item;
//    item.class_id = node->class_id;
//    item.name = node->name;
//    m_ui_items[item] = Item_Data({pos});

    m_hal.add_sink(def->name, def->name, std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::sink::Sink_ptr node)
    {
        if (result == silk::HAL::Result::OK)
        {
            QNEBlock *b = new QNEBlock();
            m_scene->addItem(b);
            b->setName(prettify_name(node->name).c_str());
            b->setPos(pos);
            b->setBrush(QBrush(QColor(0x26C281)));
            for (auto const& i: node->inputs)
            {
                auto port = b->addInputPort(prettify_name(i.name).c_str());
                port->setPortType(i.class_id);
                port->setBrush(QBrush(QColor(QRgb(0xe67e22))));
            }
        }
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

    if (dialog.exec() == QDialog::Rejected)
    {
        return;
    }

//    Item_Key item;
//    item.class_id = node->class_id;
//    item.name = node->name;
//    m_ui_items[item] = Item_Data({pos});

    m_hal.add_processor(def->name, def->name, std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::processor::Processor_ptr node)
    {
        if (result == silk::HAL::Result::OK)
        {
            QNEBlock *b = new QNEBlock();
            m_scene->addItem(b);
            b->setName(prettify_name(node->name).c_str());
            b->setPos(pos);
            b->setBrush(QBrush(QColor(0x26C281)));
            for (auto const& i: node->inputs)
            {
                auto port = b->addInputPort(prettify_name(i.name).c_str());
                port->setPortType(i.class_id);
                port->setBrush(QBrush(QColor(QRgb(0xe67e22))));
            }
            for (auto const& o: node->outputs)
            {
                auto port = b->addOutputPort(prettify_name(o.name).c_str());
                port->setPortType(o.class_id);
                port->setBrush(QBrush(QColor(QRgb(0x9b59b6))));
            }
        }
    });
}

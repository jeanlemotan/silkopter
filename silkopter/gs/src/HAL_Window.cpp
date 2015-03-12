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

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/processor/ILPF.h"
#include "common/node/processor/IResampler.h"
#include "common/node/processor/IMultirotor_Pilot.h"

#include "ui_New_Node.h"

static std::map<q::rtti::class_id, QColor> s_node_colors =
{{
    { q::rtti::get_class_id<silk::node::INode>(), QColor(0xF4D03F) },
    { q::rtti::get_class_id<silk::node::ISource>(), QColor(0x86E2D5) },
    { q::rtti::get_class_id<silk::node::ISink>(), QColor(0xF1A9A0) },
    { q::rtti::get_class_id<silk::node::IProcessor>(), QColor(0xFDE3A7) },
    { q::rtti::get_class_id<silk::node::ILPF>(), QColor(0xF7CA18) },
    { q::rtti::get_class_id<silk::node::IResampler>(), QColor(0xEB974E) },
    { q::rtti::get_class_id<silk::node::IMultirotor_Pilot>(), QColor(0xBE90D4) },
}};

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

    m_nodes_editor = new QNodesEditor(this);
    m_nodes_editor->install(m_scene);

    m_selection.config_dock = new QDockWidget(this);
    m_selection.config_view = new QTreeView(m_selection.config_dock);
    m_selection.config_model = new JSON_Model(m_selection.config_view);
    connect(m_selection.config_model, &QAbstractItemModel::dataChanged, this, &HAL_Window::on_config_changed);

    m_selection.config_dock->setWidget(m_selection.config_view);
    m_selection.config_view->setModel(m_selection.config_model);

    addDockWidget(Qt::RightDockWidgetArea, m_selection.config_dock);


    connect(m_scene, &QGraphicsScene::selectionChanged, [this]() { selection_changed(); });

    m_hal.node_defs_refreshed_signal.connect(std::bind(&HAL_Window::on_node_factories_refreshed, this));
    m_hal.nodes_refreshed_signal.connect(std::bind(&HAL_Window::refresh_nodes, this));

    refresh_nodes();
}

HAL_Window::~HAL_Window()
{

}

void HAL_Window::on_node_factories_refreshed()
{
}

void HAL_Window::refresh_nodes()
{
    auto nodes = m_hal.get_nodes().get_all();

    for (auto const& n: nodes)
    {
        add_node(n, QPointF());
    }
}

void HAL_Window::on_config_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    m_hal.set_node_config(m_selection.node, m_selection.config_json);
}

static auto prettify_name(std::string const& name) -> std::string
{
    std::string new_name = name;
    boost::trim(new_name);
    char old_c = 0;
    for (auto& c: new_name)
    {
        c == '_' ? ' ' : c;
        if (::isalnum(c) && old_c == ' ')
        {
            c = ::toupper(c);
        }
        old_c = c;
    }
    return new_name;
}

static auto get_icon(std::string const& node_icon_name, silk::node::Node_Def& node_def) -> QIcon
{
    auto name = prettify_name(node_def.name);
    QString icon_name(q::util::format2<std::string>(":/icons/{}.png", name).c_str());
    if (QFile::exists(icon_name))
    {
        return QIcon(icon_name);
    }

    //load based on the name and subparts of it
    auto tokens = q::util::tokenize(name, std::string(" "));
    name.clear();
    for (auto const& t: tokens)
    {
        name += t;
        icon_name = q::util::format2<std::string>(":/icons/{}.png", name).c_str();
        if (QFile::exists(icon_name))
        {
            return QIcon(icon_name);
        }
        name += " ";
    }

    return QIcon(node_icon_name.c_str());
}

void HAL_Window::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);

    auto pos = QPointF(m_view->mapToScene(event->pos()));

    auto nodes = m_hal.get_node_defs().get_all();

    QMenu* sources = menu.addMenu(QIcon(":/icons/Source.png"), "Sources");
    QMenu* sinks = menu.addMenu(QIcon(":/icons/Sink.png"), "Sinks");
    QMenu* processors = menu.addMenu(QIcon(":/icons/Processor.png"), "Processors");
    QMenu* lpfs = menu.addMenu(QIcon(":/icons/LPF.png"), "Low Pass Filters");
    QMenu* resamplers = menu.addMenu(QIcon(":/icons/Resampler.png"), "Resamplers");
    QMenu* pilots = menu.addMenu(QIcon(":/icons/Pilot.png"), "Pilots");
    QMenu* misc = menu.addMenu(QIcon(":/icons/Node.png"), "Misc");

    for (auto const& n: nodes)
    {
        QAction* action = nullptr;
        if (n->class_id == q::rtti::get_class_id<silk::node::ISource>())
        {
            action = sources->addAction(get_icon(":/icons/Source.png", *n), prettify_name(n->name).c_str());
        }
        else if (n->class_id == q::rtti::get_class_id<silk::node::ISink>())
        {
            action = sinks->addAction(get_icon(":/icons/Sink.png", *n), prettify_name(n->name).c_str());
        }
        else if (n->class_id == q::rtti::get_class_id<silk::node::IProcessor>())
        {
            action = processors->addAction(get_icon(":/icons/Processor.png", *n), prettify_name(n->name).c_str());
        }
        else if (n->class_id == q::rtti::get_class_id<silk::node::ILPF>())
        {
            action = lpfs->addAction(get_icon(":/icons/LPF.png", *n), prettify_name(n->name).c_str());
        }
        else if (n->class_id == q::rtti::get_class_id<silk::node::IResampler>())
        {
            action = resamplers->addAction(get_icon(":/icons/Resampler.png", *n), prettify_name(n->name).c_str());
        }
        else if (n->class_id == q::rtti::get_class_id<silk::node::IMultirotor_Pilot>())
        {
            action = pilots->addAction(get_icon(":/icons/Pilot.png", *n), prettify_name(n->name).c_str());
        }
        else
        {
            action = misc->addAction(get_icon(":/icons/Node.png", *n), prettify_name(n->name).c_str());
        }

        if (action)
        {
            connect(action, &QAction::triggered, [=](bool) { create_node(n, pos); });
        }
    }

    if (sources->actions().empty())
    {
        sources->addAction(QIcon(), "none")->setEnabled(false);
    }
    if (sinks->actions().empty())
    {
        sinks->addAction(QIcon(), "none")->setEnabled(false);
    }
    if (processors->actions().empty())
    {
        processors->addAction(QIcon(), "none")->setEnabled(false);
    }
    if (lpfs->actions().empty())
    {
        lpfs->addAction(QIcon(), "none")->setEnabled(false);
    }
    if (resamplers->actions().empty())
    {
        resamplers->addAction(QIcon(), "none")->setEnabled(false);
    }
    if (pilots->actions().empty())
    {
        pilots->addAction(QIcon(), "none")->setEnabled(false);
    }
    if (misc->actions().empty())
    {
        misc->addAction(QIcon(), "none")->setEnabled(false);
    }

    menu.exec(event->globalPos());
}

void HAL_Window::selection_changed()
{
    set_config_editor_node(silk::node::Node_ptr());

    auto items = m_scene->selectedItems();
    if (items.empty())
    {
        return;
    }
    auto* item = items.at(0);
    if (!item || item->type() != QNEBlock::Type)
    {
        return;
    }
    QNEBlock* block = reinterpret_cast<QNEBlock*>(item);
    auto node = m_hal.get_nodes().find_by_name(block->id().toLatin1().data());
    if (!node)
    {
        return;
    }
    set_config_editor_node(node);
}

void HAL_Window::set_config_editor_node(silk::node::Node_ptr node)
{
    if (node)
    {
        m_selection.config_json = jsonutil::clone_value(node->config);
        m_selection.config_model->set_document(std::string("Config"), &m_selection.config_json);
        m_selection.config_view->expandAll();
    }
    else
    {
        m_selection.config_model->set_document(std::string(), nullptr);
    }
    m_selection.node = node;
}

std::string HAL_Window::compute_unique_name(std::string const& name) const
{
    std::string new_name = name.empty() ? "Node" : name;
    if (m_nodes.find(new_name) == m_nodes.end())
    {
        return new_name;
    }

    //find the ending number
    int start = -1;
    for (int i = new_name.length() - 1; i >= 0; i--)
    {
        if (!isdigit(new_name[i]))
        {
            break;
        }
        start = i;
    }

    uint32_t number = 0;
    if (start >= 0)
    {
        number = atoi(&new_name.c_str()[start]);
        new_name = new_name.substr(0, start);
    }
    boost::trim(new_name);

    std::string result = new_name;
    char nstr[32];
    int count = 0;
    while (true)
    {
        sprintf(nstr, " %d", number);
        result = new_name + nstr;
        if (m_nodes.find(result) == m_nodes.end())
        {
            return result;
        }
        number++;
        count++;
    }
    return name;
}


void HAL_Window::refresh_node(silk::node::Node& node)
{
    auto& data = m_nodes[node.name];

    for (auto const& i: node.inputs)
    {
        auto& id = data.inputs[i.name];
        QASSERT(id.port);
        if (i.rate > 0)
        {
            id.port->setName(q::util::format2<std::string>("{} {}Hz", i.name, i.rate).c_str());
        }
        else
        {
            id.port->setName(q::util::format2<std::string>("{}", i.name).c_str());
        }
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

    //refresh the config
    if (m_selection.node.get() == &node)
    {
        set_config_editor_node(m_selection.node);
    }
}

void HAL_Window::add_node(silk::node::Node_ptr node, QPointF pos)
{
    auto& data = m_nodes[node->name];
    data = Node_Data();

    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(node->name.c_str());
    b->setId(node->name.c_str());
    b->setPos(pos);
    b->setBrush(QBrush(s_node_colors[node->class_id]));

    for (auto const& i: node->inputs)
    {
        auto port = b->addInputPort(QString());
        port->setBrush(QBrush(QColor(0xe67e22)));
        port->setId(i.name.c_str());

        auto input_name = i.name;
        port->connectedSignal.connect([node, input_name, this](QNEPort* output_port)
        {
            auto* block = output_port->block();
            std::string node_name = block->id().toLatin1().data();
            std::string stream_name = node_name + "/" + output_port->id().toLatin1().data();
            m_hal.connect_input(node, input_name, stream_name);
        });

        auto& port_data = data.inputs[i.name];
        port_data.port.reset(port);
    }
    for (auto const& o: node->outputs)
    {
        auto port = b->addOutputPort(QString());
        port->setBrush(QBrush(QColor(0x9b59b6)));
        port->setId(o.name.c_str());

        auto& port_data = data.outputs[o.name];
        port_data.port.reset(port);
    }

    data.block.reset(b);

    refresh_node(*node);
    node->changed_signal.connect([this](silk::node::Node& node)
    {
        refresh_node(node);
    });
}

void HAL_Window::create_node(silk::node::Node_Def_ptr def, QPointF pos)
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
    ui.name->setText(compute_unique_name(prettify_name(def->name)).c_str());

    connect(ui.ok, &QPushButton::released, &dialog, &QDialog::accept);
    connect(ui.cancel, &QPushButton::released, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_hal.add_node(def->name, ui.name->text().toLatin1().data(), std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::Node_ptr node)
        {
            if (result == silk::HAL::Result::OK)
            {
                add_node(node, pos);
            }
        });
    }
}

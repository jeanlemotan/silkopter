#include "HAL_Window.h"

#include "Sim_Window.h"


#include "ModelTest.h"
#include "json_editor/JSON_Model.h"
#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneconnection.h"
#include "qneport.h"

#include "sz_math.hpp"

#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include "Stream_Viewer_Widget.h"

#include "boost/algorithm/string.hpp"

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/ILPF.h"
#include "common/node/IResampler.h"
#include "common/node/IPilot.h"
#include "common/node/IController.h"
#include "common/node/ITransformer.h"
#include "common/node/IGenerator.h"
#include "common/node/IMulti_Simulator.h"

#include "ui_New_Node.h"

static std::map<silk::node::Type, QColor> s_node_colors =
{{
    { silk::node::ISource::TYPE, QColor(0x86E2D5) },
    { silk::node::ISink::TYPE, QColor(0xF1A9A0) },
    { silk::node::IProcessor::TYPE, QColor(0xFDE3A7) },
    { silk::node::ILPF::TYPE, QColor(0xF7CA18) },
    { silk::node::IResampler::TYPE, QColor(0xEB974E) },
    { silk::node::IPilot::TYPE, QColor(0xBE90D4) },
    { silk::node::IController::TYPE, QColor(0x9B59B6) },
    { silk::node::ITransformer::TYPE, QColor(0xF1C40F) },
    { silk::node::IGenerator::TYPE, QColor(0xF1C40F) },
    { silk::node::IMulti_Simulator::TYPE, QColor(0xF1C40F) },
}};


HAL_Window::HAL_Window(silk::HAL& hal, silk::Comms& comms, Render_Context& context, QWidget *parent)
    : QMainWindow(parent)
    , m_hal(hal)
    , m_comms(comms)
    , m_context(context)
{
    m_scene = new QGraphicsScene();

    setWindowTitle("HAL Editor");
    setMouseTracking(true);


    m_view = new QGraphicsView(this);
    m_view->setScene(m_scene);
    m_view->setCacheMode(QGraphicsView::CacheNone);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_view->setRenderHints(QPainter::RenderHints({QPainter::Antialiasing, QPainter::TextAntialiasing, QPainter::SmoothPixmapTransform, QPainter::HighQualityAntialiasing}));
    m_view->setMouseTracking(true);
//    m_view->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

    setCentralWidget(m_view);

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

    connect(m_nodes_editor, &QNodesEditor::contextMenu, this, &HAL_Window::contextMenu);
    connect(m_nodes_editor, &QNodesEditor::portContextMenu, this, &HAL_Window::portContextMenu);
    connect(m_nodes_editor, &QNodesEditor::blockContextMenu, this, &HAL_Window::blockContextMenu);
    connect(m_nodes_editor, &QNodesEditor::connectionContextMenu, this, &HAL_Window::connectionContextMenu);

    m_hal.node_defs_refreshed_signal.connect(std::bind(&HAL_Window::on_node_factories_refreshed, this));
    m_hal.nodes_refreshed_signal.connect(std::bind(&HAL_Window::refresh_nodes, this));

    refresh_nodes();
}

HAL_Window::~HAL_Window()
{
    m_streams.clear();
    m_nodes.clear();
}

void HAL_Window::on_node_factories_refreshed()
{
}

void HAL_Window::refresh_nodes()
{
    auto nodes = m_hal.get_nodes().get_all();

    for (auto const& n: nodes)
    {
        QPointF pos;
        auto* positionj = jsonutil::find_value(n->init_params, q::Path("__gs/position"));
        if (positionj)
        {
            math::vec2f position;
            autojsoncxx::error::ErrorStack result;
            if (!autojsoncxx::from_value(position, *positionj, result))
            {
                std::ostringstream ss;
                ss << result;
                QLOGE("Cannot deserialize position data: {}", ss.str());
            }
            pos.setX(position.x);
            pos.setY(position.y);
        }

        add_node(n, pos);
    }
    for (auto const& n: nodes)
    {
        refresh_node(*n);
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
        c = c == '_' ? ' ' : c;
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

void HAL_Window::portContextMenu(QGraphicsSceneMouseEvent* event, QNEPort* port)
{
    QASSERT(port);

    QMenu menu(this);

    QAction* view_stream_action = menu.addAction(QIcon(":/icons/view.png"), "View Stream");
    QNEPort* output_port = port->isOutput() ? port : nullptr;

    menu.addSeparator();

    auto& conn = port->connections();
    for (auto const& c: conn)
    {
        QNEPort* other = c->port1() == port ? c->port2() : c->port1();
        QASSERT(other);
        if (other)
        {
            menu.addAction(QIcon(":/icons/remove.png"),
                           q::util::format2<std::string>("Disconnect from {}/{}",
                                                         other->block()->id().toLatin1().data(),
                                                         other->id().toLatin1().data()).c_str());
            if (!output_port && other->isOutput())
            {
                output_port = other;
            }
        }
    }

    if (output_port)
    {
        std::string stream_name = (output_port->block()->id() + "/" + output_port->id()).toLatin1().data();
        connect(view_stream_action, &QAction::triggered, [=](bool) { open_stream_viewer(stream_name); });
    }
    else
    {
        view_stream_action->setEnabled(false);
    }

    menu.exec(event->screenPos());
}

void HAL_Window::connectionContextMenu(QGraphicsSceneMouseEvent* event, QNEConnection* connection)
{
    QASSERT(connection);

    QMenu menu(this);

    QAction* action = menu.addAction(QIcon(":/icons/view.png"), "Open Viewer");
    menu.addSeparator();
    action = menu.addAction(QIcon(":/icons/remove.png"), "Disconnect");

    menu.exec(event->screenPos());
}

void HAL_Window::blockContextMenu(QGraphicsSceneMouseEvent* event, QNEBlock* block)
{
    QASSERT(block);

    QMenu menu(this);

    auto it = m_nodes.find(block->id().toLatin1().data());
    if (it != m_nodes.end())
    {
        auto node = it->second.node.lock();
        if (node && node->type == silk::node::IMulti_Simulator::TYPE)
        {
            auto action = menu.addAction(QIcon(":/icons/simulator.png"), "View Simulatior");
            connect(action, &QAction::triggered, [=](bool)
            {
                delete m_sim_window;
                m_sim_window = new Sim_Window(m_hal, node, m_comms, m_context, this);
                m_sim_window->show();
            });
        }
    }

    menu.addAction(QIcon(":/icons/remove.png"), q::util::format2<std::string>("Remove {}", block->id().toLatin1().data()).c_str());

    menu.exec(event->screenPos());
}

void HAL_Window::contextMenu(QGraphicsSceneMouseEvent* event)
{
    QMenu menu(this);

    auto pos = event->scenePos();

    auto nodes = m_hal.get_node_defs().get_all();

    QMenu* sources = menu.addMenu(QIcon(":/icons/Source.png"), "Sources");
    QMenu* sinks = menu.addMenu(QIcon(":/icons/Sink.png"), "Sinks");
    QMenu* processors = menu.addMenu(QIcon(":/icons/Processor.png"), "Processors");
    QMenu* lpfs = menu.addMenu(QIcon(":/icons/LPF.png"), "Low Pass Filters");
    QMenu* resamplers = menu.addMenu(QIcon(":/icons/Resampler.png"), "Resamplers");
    QMenu* controllers = menu.addMenu(QIcon(":/icons/Controller.png"), "Controllers");
    QMenu* transformers = menu.addMenu(QIcon(":/icons/Transformer.png"), "Transformers");
    QMenu* generators = menu.addMenu(QIcon(":/icons/Generator.png"), "Generators");
    QMenu* pilots = menu.addMenu(QIcon(":/icons/Pilot.png"), "Pilots");
    QMenu* misc = menu.addMenu(QIcon(":/icons/Node.png"), "Misc");
    sources->setEnabled(false);
    sinks->setEnabled(false);
    processors->setEnabled(false);
    lpfs->setEnabled(false);
    resamplers->setEnabled(false);
    controllers->setEnabled(false);
    transformers->setEnabled(false);
    generators->setEnabled(false);
    pilots->setEnabled(false);
    misc->setEnabled(false);

    for (auto const& n: nodes)
    {
        QAction* action = nullptr;
        if (n->type == silk::node::ISource::TYPE)
        {
            action = sources->addAction(get_icon(":/icons/Source.png", *n), prettify_name(n->name).c_str());
            sources->setEnabled(true);
        }
        else if (n->type == silk::node::ISink::TYPE)
        {
            action = sinks->addAction(get_icon(":/icons/Sink.png", *n), prettify_name(n->name).c_str());
            sinks->setEnabled(true);
        }
        else if (n->type == silk::node::IProcessor::TYPE)
        {
            action = processors->addAction(get_icon(":/icons/Processor.png", *n), prettify_name(n->name).c_str());
            processors->setEnabled(true);
        }
        else if (n->type == silk::node::ILPF::TYPE)
        {
            action = lpfs->addAction(get_icon(":/icons/LPF.png", *n), prettify_name(n->name).c_str());
            lpfs->setEnabled(true);
        }
        else if (n->type == silk::node::IResampler::TYPE)
        {
            action = resamplers->addAction(get_icon(":/icons/Resampler.png", *n), prettify_name(n->name).c_str());
            resamplers->setEnabled(true);
        }
        else if (n->type == silk::node::IPilot::TYPE)
        {
            action = pilots->addAction(get_icon(":/icons/Pilot.png", *n), prettify_name(n->name).c_str());
            pilots->setEnabled(true);
        }
        else if (n->type == silk::node::IController::TYPE)
        {
            action = controllers->addAction(get_icon(":/icons/Controller.png", *n), prettify_name(n->name).c_str());
            controllers->setEnabled(true);
        }
        else if (n->type == silk::node::ITransformer::TYPE)
        {
            action = transformers->addAction(get_icon(":/icons/Transformer.png", *n), prettify_name(n->name).c_str());
            transformers->setEnabled(true);
        }
        else if (n->type == silk::node::IGenerator::TYPE)
        {
            action = generators->addAction(get_icon(":/icons/Generator.png", *n), prettify_name(n->name).c_str());
            generators->setEnabled(true);
        }
        else
        {
            action = misc->addAction(get_icon(":/icons/Node.png", *n), prettify_name(n->name).c_str());
            misc->setEnabled(true);
        }

        if (action)
        {
            connect(action, &QAction::triggered, [=](bool) { create_node(n, pos); });
        }
    }

    menu.exec(event->screenPos());
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

void HAL_Window::open_stream_viewer(std::string const& stream_name)
{
    QDockWidget* dock = new QDockWidget(q::util::format2<std::string>("Stream: {}", stream_name).c_str(), this);
    QFont font;
    font.setPointSize(8);
    dock->setFont(font);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    Stream_Viewer_Widget* viewer = new Stream_Viewer_Widget(m_hal, m_comms, dock);
    viewer->set_stream_name(stream_name);

    dock->layout()->setMargin(0);
    dock->layout()->setContentsMargins(0, 0, 0, 0);
    dock->setWidget(viewer);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    //dock->show();
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
        id.port->setPortType(static_cast<int>(i.type));
        id.port->setPortRate(i.rate);
        id.port->disconnectAll();

        auto stream = i.stream.lock();
        if (!stream)
        {
            continue;
        }
        auto it_stream = m_streams.find(stream->name);
        QASSERT(it_stream != m_streams.end());
        if (it_stream != m_streams.end())
        {
            auto connection = new QNEConnection(0);
            m_scene->addItem(connection);
            connection->setPort1((QNEPort*)id.port.get());
            connection->setPort2(it_stream->second.port.get());
            connection->updatePath();
        }
    }

    for (auto const& o: node.outputs)
    {
        auto& od = data.outputs[o.name];
        QASSERT(od.port);
        od.port->setName(q::util::format2<std::string>("{} {}Hz", o.name, o.rate).c_str());
        od.port->setPortType(static_cast<int>(o.type));
        od.port->setPortRate(o.rate);
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
    b->setBrush(QBrush(s_node_colors[node->type]));

    data.node = node;
    data.block.reset(b);

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
            m_hal.connect_node_input(node, input_name, stream_name);
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

        auto& stream_data = m_streams[o.stream->name];
        stream_data.stream = o.stream;
        stream_data.port = port_data.port;
        stream_data.block = data.block;
    }

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
        auto* positionj = jsonutil::get_or_add_value(init_params, q::Path("__gs/position"), rapidjson::kObjectType, init_params.GetAllocator());
        if (positionj)
        {
            rapidjson::Document doc;
            autojsoncxx::to_document(math::vec2f(pos.x(), pos.y()), doc);
            jsonutil::clone_value(*positionj, doc, init_params.GetAllocator());
        }

        m_hal.add_node(def->name, ui.name->text().toLatin1().data(), std::move(init_params), [this, pos](silk::HAL::Result result, silk::node::Node_ptr node)
        {
            if (result == silk::HAL::Result::OK)
            {
                add_node(node, pos);
                refresh_node(*node);
            }
        });
    }
}

void HAL_Window::process()
{
    if (m_sim_window)
    {
        m_sim_window->process();
        if (m_sim_window->isHidden())
        {
            delete m_sim_window;
            m_sim_window = 0;
        }
    }
}

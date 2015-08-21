#include "HAL_Window.h"

#include "Sim_Window.h"


#include "ModelTest.h"
#include "json_editor/JSON_Model.h"
#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneconnection.h"
#include "qneport.h"

#include "sz_math.hpp"
#include "sz_Calibration_Data.hpp"


#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include "Stream_Viewer_Widget.h"
#include "Custom_Item_Delegate.h"

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
#include "common/node/IBrain.h"

#include "ui_New_Node.h"

#include "calibration/Acceleration_Calibration_Wizard.h"
#include "calibration/Angular_Velocity_Calibration_Wizard.h"
#include "calibration/Magnetic_Field_Calibration_Wizard.h"

static const q::Path k_settings_path("settings.json");


static std::map<silk::node::Type, QColor> s_node_colors =
{{
    { silk::node::ISource::TYPE, QColor(0x86E2D5) },
    { silk::node::ISink::TYPE, QColor(0xF1A9A0) },
    { silk::node::IProcessor::TYPE, QColor(0xFDE3A7) },
    { silk::node::ILPF::TYPE, QColor(0xF7CA18) },
    { silk::node::IResampler::TYPE, QColor(0xEB974E) },
    { silk::node::IPilot::TYPE, QColor(0xBE90D4) },
    { silk::node::IController::TYPE, QColor(0x9B59B6) },
    { silk::node::ITransformer::TYPE, QColor(0xF10FC4) },
    { silk::node::IGenerator::TYPE, QColor(0xC40FF1) },
    { silk::node::IMulti_Simulator::TYPE, QColor(0xF1C40F) },
    { silk::node::IBrain::TYPE, QColor(0xC4F10F) },
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

    load_editor_data();


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
    m_selection.config_view->setItemDelegate(new Custom_Item_Delegate(m_selection.config_view));

    addDockWidget(Qt::RightDockWidgetArea, m_selection.config_dock);

    connect(m_scene, &QGraphicsScene::selectionChanged, [this]() { selection_changed(); });

    connect(m_nodes_editor, &QNodesEditor::contextMenu, this, &HAL_Window::context_menu);
    connect(m_nodes_editor, &QNodesEditor::portContextMenu, this, &HAL_Window::port_context_menu);
    connect(m_nodes_editor, &QNodesEditor::blockContextMenu, this, &HAL_Window::block_context_menu);
    connect(m_nodes_editor, &QNodesEditor::connectionContextMenu, this, &HAL_Window::connection_context_menu);

    m_hal.get_nodes().item_added_signal.connect([this](silk::node::Node_ptr item)
    {
        add_node(item);
    });
    m_hal.get_nodes().item_will_be_removed_signal.connect([this](silk::node::Node_ptr item)
    {
        remove_node(item);
    });
    m_hal.get_streams().item_added_signal.connect([this](silk::node::stream::gs::Stream_ptr item)
    {
        add_stream(item);
    });
    m_hal.get_streams().item_will_be_removed_signal.connect([this](silk::node::stream::gs::Stream_ptr item)
    {
        remove_stream(item);
    });



//    refresh_nodes();
}

HAL_Window::~HAL_Window()
{
    m_ui_streams.clear();
    m_ui_nodes.clear();
}

//void HAL_Window::refresh_nodes()
//{
//    auto nodes = m_hal.get_nodes().get_all();

//    for (auto const& n: nodes)
//    {
//        QPointF pos;
//        auto* positionj = jsonutil::find_value(n->init_params, q::Path("__gs/position"));
//        if (positionj)
//        {
//            math::vec2f position;
//            autojsoncxx::error::ErrorStack result;
//            if (!autojsoncxx::from_value(position, *positionj, result))
//            {
//                std::ostringstream ss;
//                ss << result;
//                QLOGE("Cannot deserialize position data: {}", ss.str());
//            }
//            pos.setX(position.x);
//            pos.setY(position.y);
//        }

//        add_node(n, pos);
//    }
//    for (auto const& n: nodes)
//    {
//        refresh_node(*n);
//    }
//}

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

void HAL_Window::port_context_menu(QGraphicsSceneMouseEvent* event, QNEPort* port)
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
        if (other && !output_port && other->isOutput())
        {
            output_port = other;
            break;
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

void HAL_Window::connection_context_menu(QGraphicsSceneMouseEvent* event, QNEConnection* connection)
{
    QASSERT(connection);

    QNEPort* input_port = !connection->port1()->isOutput() ? connection->port1() : connection->port2();
    QASSERT(!input_port->isOutput());

    auto* block = input_port->block();
    QASSERT(block);

    auto it = m_ui_nodes.find(block->id().toLatin1().data());
    if (it == m_ui_nodes.end())
    {
        return;
    }
    auto node = it->second.node.lock();
    if (!node)
    {
        return;
    }

    QMenu menu(this);

//    QAction* action = menu.addAction(QIcon(":/icons/view.png"), "Open Viewer");
//    menu.addSeparator();
    QAction* action = menu.addAction(QIcon(":/icons/remove.png"), "Disconnect");

    connect(action, &QAction::triggered, [=](bool)
    {
        std::string input_name = input_port->id().toLatin1().data();
        m_hal.set_node_input_stream_path(node, input_name, q::Path());
    });

    menu.exec(event->screenPos());
}

auto HAL_Window::supports_acceleration_calibration(silk::node::Node const& node, silk::node::Node::Output const& output) const -> bool
{
    if (output.type != silk::node::stream::Type::ACCELERATION)
    {
        return false;
    }
    q::Path path("Calibration");
    path += output.name;
    auto const* calibrationj = jsonutil::find_value(node.config, path);
    if (!calibrationj)
    {
        return false;
    }
    sz::calibration::Acceleration_Points calibration;
    autojsoncxx::error::ErrorStack result;
    return autojsoncxx::from_value(calibration, *calibrationj, result);
}

auto HAL_Window::supports_angular_velocity_calibration(silk::node::Node const& node, silk::node::Node::Output const& output) const -> bool
{
    if (output.type != silk::node::stream::Type::ANGULAR_VELOCITY)
    {
        return false;
    }
    q::Path path("Calibration");
    path += output.name;
    auto const* calibrationj = jsonutil::find_value(node.config, path);
    if (!calibrationj)
    {
        return false;
    }
    sz::calibration::Angular_Velocity_Points calibration;
    autojsoncxx::error::ErrorStack result;
    return autojsoncxx::from_value(calibration, *calibrationj, result);
}

auto HAL_Window::supports_magnetic_field_calibration(silk::node::Node const& node, silk::node::Node::Output const& output) const -> bool
{
    if (output.type != silk::node::stream::Type::MAGNETIC_FIELD)
    {
        return false;
    }
    q::Path path("Calibration");
    path += output.name;
    auto const* calibrationj = jsonutil::find_value(node.config, path);
    if (!calibrationj)
    {
        return false;
    }
    sz::calibration::Magnetic_Field_Points calibration;
    autojsoncxx::error::ErrorStack result;
    return autojsoncxx::from_value(calibration, *calibrationj, result);
}

void HAL_Window::block_context_menu(QGraphicsSceneMouseEvent* event, QNEBlock* block)
{
    QASSERT(block);

    QMenu menu(this);

    auto it = m_ui_nodes.find(block->id().toLatin1().data());
    if (it != m_ui_nodes.end())
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

        for (auto const& os: node->outputs)
        {
            if (supports_acceleration_calibration(*node, os))
            {
                auto action = menu.addAction(QIcon(":/icons/calibrate.png"), "Start Acceleration Calibration");
                connect(action, &QAction::triggered, [=](bool)
                {
                    do_acceleration_calibration(node, &os - &node->outputs.front());
                });
            }
            if (supports_angular_velocity_calibration(*node, os))
            {
                auto action = menu.addAction(QIcon(":/icons/calibrate.png"), "Start Angular Velocity Calibration");
                connect(action, &QAction::triggered, [=](bool)
                {
                    do_angular_velocity_calibration(node, &os - &node->outputs.front());
                });

                continue;
            }
            if (supports_magnetic_field_calibration(*node, os))
            {
                auto action = menu.addAction(QIcon(":/icons/calibrate.png"), "Start Magnetic Field Calibration");
                connect(action, &QAction::triggered, [=](bool)
                {
                    do_magnetic_field_calibration(node, &os - &node->outputs.front());
                });

                continue;
            }
        }

        QAction* action = menu.addAction(QIcon(":/icons/remove.png"), q::util::format2<std::string>("Remove {}", block->id().toLatin1().data()).c_str());
        connect(action, &QAction::triggered, [this, node](bool)
        {
            try_remove_node(node);
        });

        menu.exec(event->screenPos());
    }
}

void HAL_Window::context_menu(QGraphicsSceneMouseEvent* event)
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
            connect(action, &QAction::triggered, [=](bool) { try_add_node(n, pos); });
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
    if (m_ui_nodes.find(new_name) == m_ui_nodes.end())
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
        if (m_ui_nodes.find(result) == m_ui_nodes.end())
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

void HAL_Window::do_acceleration_calibration(silk::node::Node_ptr node, size_t output_idx)
{
    Acceleration_Calibration_Wizard wizard(m_hal, m_comms, node, output_idx, this);
    wizard.exec();
}

void HAL_Window::do_magnetic_field_calibration(silk::node::Node_ptr node, size_t output_idx)
{
    Magnetic_Field_Calibration_Wizard wizard(m_hal, m_comms, node, output_idx, this);
    wizard.exec();
}

void HAL_Window::do_angular_velocity_calibration(silk::node::Node_ptr node, size_t output_idx)
{
    Angular_Velocity_Calibration_Wizard wizard(m_hal, m_comms, node, output_idx, this);
    wizard.exec();
}

void HAL_Window::add_stream(silk::node::stream::gs::Stream_ptr stream)
{
}

void HAL_Window::remove_stream(silk::node::stream::gs::Stream_ptr stream)
{

}


void HAL_Window::refresh_node(silk::node::Node& node)
{
    auto& data = m_ui_nodes[node.name];

//    QPointF pos;
//    auto* positionj = jsonutil::find_value(node->config, q::Path("__gs/position"));
//    if (positionj)
//    {
//        math::vec2f position;
//        autojsoncxx::error::ErrorStack result;
//        if (!autojsoncxx::from_value(position, *positionj, result))
//        {
//            std::ostringstream ss;
//            ss << result;
//            QLOGE("Cannot deserialize position data: {}", ss.str());
//        }
//        pos.setX(position.x);
//        pos.setY(position.y);
//    }

    for (auto const& i: node.inputs)
    {
        auto& ui_input = data.ui_inputs[i.name];
        QASSERT(ui_input.port);
        if (i.rate > 0)
        {
            ui_input.port->setName(q::util::format2<std::string>("{} {}Hz", i.name, i.rate).c_str());
        }
        else
        {
            ui_input.port->setName(q::util::format2<std::string>("{}", i.name).c_str());
        }
        ui_input.port->setPortType(static_cast<int>(i.type));
        ui_input.port->setPortRate(i.rate);
        ui_input.port->disconnectAll();

        //get the hal stream this input is connected to
        auto stream = i.stream.lock();
        if (!stream)
        {
            continue;
        }

        //find the hal node this stream originates from
        auto stream_node = stream->node.lock();
        QASSERT(stream_node);
        if (!stream_node)
        {
            continue;
        }

        auto it_stream = m_ui_streams.find(stream->name);
        QASSERT(it_stream != m_ui_streams.end());
        if (it_stream != m_ui_streams.end())
        {
            auto connection = new QNEConnection(0);
            m_scene->addItem(connection);
            connection->setPort1((QNEPort*)ui_input.port);
            connection->setPort2(it_stream->second.port);
            connection->updatePath();
        }

    }

    for (auto const& o: node.outputs)
    {
        auto& ui_output = data.ui_outputs[o.name];
        QASSERT(ui_output.port);
        ui_output.port->setName(q::util::format2<std::string>("{} {}Hz", o.name, o.rate).c_str());
        ui_output.port->setPortType(static_cast<int>(o.type));
        ui_output.port->setPortRate(o.rate);
    }
    data.block->refreshGeometry();

    //refresh the config
    if (m_selection.node.get() == &node)
    {
        set_config_editor_node(m_selection.node);
    }
}

void HAL_Window::add_node(silk::node::Node_ptr node)
{
    QPointF pos = get_node_position(node->name);

    auto* positionj = jsonutil::find_value(node->config, q::Path("__gs/position"));
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

    auto& data = m_ui_nodes[node->name];
    data = UI_Node();

    QNEBlock *b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(node->name.c_str());
    b->setId(node->name.c_str());
    b->setPos(pos);

    b->setBrush(QBrush(Qt::white));
    auto it = s_node_colors.find(node->type);
    if (it != s_node_colors.end())
    {
        b->setBrush(it->second);
    }

    b->positionChangedSignal.connect([this, node](const QPointF& pos)
    {
        set_node_position(node->name, pos);
    });

    data.node = node;
    data.block = b;

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
            q::Path stream_path(node_name);
            stream_path += output_port->id().toLatin1().data();
            m_hal.set_node_input_stream_path(node, input_name, stream_path);
        });

        auto& port_data = data.ui_inputs[i.name];
        port_data.port = port;
    }
    for (auto const& o: node->outputs)
    {
        auto port = b->addOutputPort(QString());
        port->setBrush(QBrush(QColor(0x9b59b6)));
        port->setId(o.name.c_str());

        auto& port_data = data.ui_outputs[o.name];
        port_data.port = port;

        auto& stream_data = m_ui_streams[o.stream->name];
        stream_data.stream = o.stream;
        stream_data.port = port_data.port;
        stream_data.block = data.block;
    }

    auto* node_ptr = node.get();
    m_connections.push_back( node->changed_signal.connect([this, node_ptr]()
    {
        refresh_node(*node_ptr);
    }) );
}

void HAL_Window::try_add_node(silk::node::Node_Def_ptr def, QPointF pos)
{
    auto init_paramsj = jsonutil::clone_value(def->default_init_params);

    QDialog dialog;
    dialog.setLayout(new QVBoxLayout(&dialog));

    Ui::New_Node ui;
    QWidget* widget = new QWidget(&dialog);
    ui.setupUi(widget);
    widget->setMinimumSize(600, 400);
    dialog.layout()->addWidget(widget);

    JSON_Model* model = new JSON_Model(ui.init_params);
    model->set_document("Init Params", &init_paramsj);
    ui.init_params->setModel(model);
    ui.init_params->expandAll();
    ui.init_params->header()->resizeSections(QHeaderView::ResizeToContents);
    ui.name->setText(compute_unique_name(prettify_name(def->name)).c_str());

    connect(ui.ok, &QPushButton::released, &dialog, &QDialog::accept);
    connect(ui.cancel, &QPushButton::released, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        std::string node_name = ui.name->text().toLatin1().data();
        set_node_position(node_name, pos);
        m_hal.add_node(def->name, node_name, std::move(init_paramsj));
    }
}

void HAL_Window::remove_node(silk::node::Node_ptr node)
{
    auto it = m_ui_nodes.find(node->name);
    if (it != m_ui_nodes.end())
    {
        UI_Node& nd = it->second;
        m_scene->removeItem(nd.block);
        delete nd.block;
        m_ui_nodes.erase(it);
    }
}

void HAL_Window::try_remove_node(silk::node::Node_ptr node)
{
    std::string nodeName = node->name;

    auto answer = QMessageBox::question(this, "Question", q::util::format2<std::string>("Are you sure you want to remove node {}", node->name).c_str());
    if (answer == QMessageBox::Yes)
    {
        m_hal.remove_node(node);
    }
}

void HAL_Window::set_node_position(std::string const& node_name, QPointF const& pos)
{
    q::Path path("nodes/" + node_name + "/hal_editor/position");
    auto* positionj = jsonutil::get_or_add_value(m_editor_data, path, rapidjson::kObjectType, m_editor_data.GetAllocator());
    if (positionj)
    {
        rapidjson::Document doc;
        autojsoncxx::to_document(math::vec2f(pos.x(), pos.y()), doc);
        jsonutil::clone_value(*positionj, doc, m_editor_data.GetAllocator());
    }
    save_editor_data();
}
auto HAL_Window::get_node_position(std::string const& node_name) -> QPointF
{
    math::vec2f sz;
    q::Path path("nodes/" + node_name + "/hal_editor/position");
    auto* positionj = jsonutil::get_or_add_value(m_editor_data, path, rapidjson::kObjectType, m_editor_data.GetAllocator());
    if (positionj)
    {
        autojsoncxx::error::ErrorStack result;
        if (!autojsoncxx::from_value(sz, *positionj, result))
        {
            std::ostringstream ss;
            ss << result;
            QLOGW("Cannot deserialize node {} position: {}", node_name, ss.str());
        }
    }
    return QPointF(sz.x, sz.y);
}

void HAL_Window::save_editor_data()
{
    rapidjson::StringBuffer s;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
    m_editor_data.Accept(writer);
    q::data::File_Sink fs(k_settings_path);
    if (fs.is_open())
    {
        fs.write(reinterpret_cast<uint8_t const*>(s.GetString()), s.GetSize());
    }
    else
    {
        QLOGE("Cannot open '{}' to save settings.", k_settings_path);
    }
}
void HAL_Window::load_editor_data()
{
    m_editor_data.SetObject();

    q::data::File_Source fs(k_settings_path);
    if (!fs.is_open())
    {
        return;
    }

    auto data = q::data::read_whole_source_as_string<std::string>(fs);
    if (m_editor_data.Parse(data.c_str()).HasParseError())
    {
        QLOGE("Failed to load '{}': {}:{}", k_settings_path, m_editor_data.GetParseError(), m_editor_data.GetErrorOffset());
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


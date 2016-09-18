#include "Nodes_Widget.h"
#include "Comms.h"
#include "Properties_Browser.h"

#include "common/node/INode.h"
#include "common/node/ISink.h"
#include "common/node/ISource.h"
#include "common/node/IProcessor.h"
#include "common/node/ILPF.h"
#include "common/node/ICombiner.h"
#include "common/node/IResampler.h"
#include "common/node/IPilot.h"
#include "common/node/IController.h"
#include "common/node/ITransformer.h"
#include "common/node/IGenerator.h"
#include "common/node/IBrain.h"
#include "common/node/IMultirotor_Simulator.h"

#include "boost/algorithm/string.hpp"
#include "def_lang/Value_Selector.h"

#include "ui_New_Node_Dialog.h"
#include "qneconnection.h"

#include "Angular_Velocity_Calibration_Wizard.h"
#include "Acceleration_Calibration_Wizard.h"
#include "Magnetic_Field_Calibration_Wizard.h"

#include "Stream_Viewer_Window.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QSettings>

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
    { silk::node::IMultirotor_Simulator::TYPE, QColor(0xF1C40F) },
    { silk::node::IBrain::TYPE, QColor(0xC4F10F) },
    { silk::node::ICombiner::TYPE, QColor(0xF10FC4) },
}};


Nodes_Widget::Nodes_Widget()
{
    m_scene = new QGraphicsScene();

    setMouseTracking(true);

    load_editor_data();

    m_view = new QGraphicsView(this);
    m_view->setScene(m_scene);
    m_view->setCacheMode(QGraphicsView::CacheNone);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_view->setRenderHints(QPainter::RenderHints({QPainter::Antialiasing, QPainter::TextAntialiasing, QPainter::SmoothPixmapTransform, QPainter::HighQualityAntialiasing}));
    m_view->setMouseTracking(true);
    //    m_view->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    layout->addWidget(m_view);

    m_nodes_editor = new QNodesEditor(this);
    m_nodes_editor->install(m_scene);

    connect(m_scene, &QGraphicsScene::selectionChanged, [this]() { on_selection_changed(); });
    connect(m_nodes_editor, &QNodesEditor::contextMenu, this, &Nodes_Widget::show_context_menu);
    connect(m_nodes_editor, &QNodesEditor::portContextMenu, this, &Nodes_Widget::show_port_context_menu);
    connect(m_nodes_editor, &QNodesEditor::blockContextMenu, this, &Nodes_Widget::show_block_context_menu);
    connect(m_nodes_editor, &QNodesEditor::connectionContextMenu, this, &Nodes_Widget::show_connection_context_menu);
}

Nodes_Widget::~Nodes_Widget()
{
}

void Nodes_Widget::init(QToolBar* toolbar, silk::Comms& comms, Properties_Browser& browser)
{
    QASSERT(m_comms == nullptr);
    m_toolbar = toolbar;
    m_comms = &comms;
    m_browser = &browser;
}

void Nodes_Widget::set_active(bool active)
{
    m_browser->set_value(nullptr);
    m_selection.node = nullptr;
    on_selection_changed();

    if (!active)
    {
        if (m_refresh_action)
        {
            delete m_refresh_action;
            m_refresh_action = nullptr;

            delete m_upload_action;
            m_upload_action = nullptr;

//            delete m_new_action;
//            m_new_action = nullptr;
        }

        m_connections.value_changed_connection.disconnect();

        return;
    }
    m_refresh_action = m_toolbar->addAction(QIcon(":/icons/ui/reconnect.png"), "Refresh");
    QObject::connect(m_refresh_action, &QAction::triggered, [this](bool) { refresh(); });

    m_upload_action = m_toolbar->addAction(QIcon(":/icons/ui/upload.png"), "Upload");
    m_upload_action->setEnabled(false);
    m_upload_action->setShortcut(QKeySequence("Ctrl+S"));
    QObject::connect(m_upload_action, &QAction::triggered, [this](bool)
    {
        if (m_selection.node)
        {
            upload_config(m_selection.node);
        }
    });

    m_connections.value_changed_connection = m_browser->sig_value_changed.connect([this]()
    {
        on_browser_value_changed();
    });

//    m_new_action = m_toolbar->addAction(QIcon(":/icons/ui/new.png"), "New");
//    QObject::connect(m_new_action, &QAction::triggered, [this](bool) { show_new_descriptor_menu(); });
}

void Nodes_Widget::on_browser_value_changed()
{
    if (!m_selection.node)
    {
        return;
    }

    upload_config(m_selection.node);
}

void Nodes_Widget::upload_config(std::shared_ptr<Node> node)
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }

    auto result = m_comms->set_node_config(node->name, node->config);
    if (result != ts::success)
    {
        QMessageBox::critical(this, "Error", result.error().what().c_str());
    }

    m_uav_name.clear();
    refresh_node(result.payload());
}

void Nodes_Widget::refresh()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }

    auto result = m_comms->request_uav_descriptor();
    if (result != ts::success)
    {
        QMessageBox::critical(this, "Error", result.error().what().c_str());
        return;
    }

    m_uav_name.clear();
    if (result.payload())
    {
        std::shared_ptr<const ts::IMember> member = result.payload()->find_member_by_name("name");
        if (member)
        {
            std::shared_ptr<const ts::IString_Value> name_value = std::dynamic_pointer_cast<const ts::IString_Value>(member->get_value());
            if (name_value)
            {
                m_uav_name = name_value->get_value();
            }
        }
    }

    refresh_node_defs();
    refresh_nodes();
}

void Nodes_Widget::refresh_node_defs()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }

    auto node_defs_result = m_comms->request_node_defs();
    if (node_defs_result != ts::success)
    {
        QMessageBox::critical(this, "Error", node_defs_result.error().what().c_str());
        return;
    }

    m_node_defs = node_defs_result.payload();
}

void Nodes_Widget::refresh_nodes()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
    }

    auto result = m_comms->request_nodes();
    if (result != ts::success)
    {
        QMessageBox::critical(this, "Error", result.error().what().c_str());
        return;
    }

    m_streams.clear();
    m_nodes.clear();
    m_scene->clear();

    std::vector<silk::Comms::Node> nodes = result.extract_payload();
    for (silk::Comms::Node const& node: nodes)
    {
        add_node(node);
    }

    //refresh one more time to link the streams
    for (silk::Comms::Node const& node: nodes)
    {
        refresh_node(node);
    }
}

bool Nodes_Widget::supports_acceleration_calibration(Node const& node, Node::Output const& output) const
{
    if (output.type.get_semantic() != silk::stream::Semantic::ACCELERATION)
    {
        return false;
    }

    std::shared_ptr<const ts::IVector_Value> value = node.config->select_specialized<ts::IVector_Value>(ts::Value_Selector("calibration." + output.name));
    if (!value)
    {
        return false;
    }

    return value->get_specialized_type()->get_inner_type() == m_comms->get_type_system().get_root_scope()->find_symbol_by_name("Acceleration_Calibration_Point");
}

bool Nodes_Widget::supports_angular_velocity_calibration(Node const& node, Node::Output const& output) const
{
    if (output.type.get_semantic() != silk::stream::Semantic::ANGULAR_VELOCITY)
    {
        return false;
    }
    std::shared_ptr<const ts::IVector_Value> value = node.config->select_specialized<ts::IVector_Value>(ts::Value_Selector("calibration." + output.name));
    if (!value)
    {
        return false;
    }

    return value->get_specialized_type()->get_inner_type() == m_comms->get_type_system().get_root_scope()->find_symbol_by_name("Angular_Velocity_Calibration_Point");
}

bool Nodes_Widget::supports_magnetic_field_calibration(Node const& node, Node::Output const& output) const
{
    if (output.type.get_semantic() != silk::stream::Semantic::MAGNETIC_FIELD)
    {
        return false;
    }
    std::shared_ptr<const ts::IVector_Value> value = node.config->select_specialized<ts::IVector_Value>(ts::Value_Selector("calibration." + output.name));
    if (!value)
    {
        return false;
    }

    return value->get_specialized_type()->get_inner_type() == m_comms->get_type_system().get_root_scope()->find_symbol_by_name("Magnetic_Field_Calibration_Point");
}

void Nodes_Widget::do_acceleration_calibration(Node& node, size_t output_idx)
{
    Node::Output const& output = node.outputs[output_idx];
    std::shared_ptr<ts::IVector_Value> value = node.config->select_specialized<ts::IVector_Value>(ts::Value_Selector("calibration." + output.name));
    if (!value)
    {
        QASSERT(false);
        return;
    }

    Acceleration_Calibration_Wizard wizard(*m_comms, node.name, node.name + "/" + output.name, output.rate, node.config, value, this);
    if (wizard.exec() == QDialog::Accepted)
    {
        auto result = m_comms->set_node_config(node.name, node.config);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot save calibration data for node '" + node.name + "':\n" + result.error().what()).c_str());
        }
        else
        {
            refresh_node(result.payload());
        }
    }
}

void Nodes_Widget::do_magnetic_field_calibration(Node& node, size_t output_idx)
{
    Node::Output const& output = node.outputs[output_idx];
    std::shared_ptr<ts::IVector_Value> value = node.config->select_specialized<ts::IVector_Value>(ts::Value_Selector("calibration." + output.name));
    if (!value)
    {
        QASSERT(false);
        return;
    }

    Magnetic_Field_Calibration_Wizard wizard(*m_comms, node.name, node.name + "/" + output.name, output.rate, node.config, value, this);
    if (wizard.exec() == QDialog::Accepted)
    {
        auto result = m_comms->set_node_config(node.name, node.config);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot save calibration data for node '" + node.name + "':\n" + result.error().what()).c_str());
        }
        else
        {
            refresh_node(result.payload());
        }
    }
}

void Nodes_Widget::do_angular_velocity_calibration(Node& node, size_t output_idx)
{
    Node::Output const& output = node.outputs[output_idx];
    std::shared_ptr<ts::IVector_Value> value = node.config->select_specialized<ts::IVector_Value>(ts::Value_Selector("calibration." + output.name));
    if (!value)
    {
        QASSERT(false);
        return;
    }

    Angular_Velocity_Calibration_Wizard wizard(*m_comms, node.name, node.name + "/" + output.name, output.rate, node.config, value, this);
    if (wizard.exec() == QDialog::Accepted)
    {
        auto result = m_comms->set_node_config(node.name, node.config);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot save calibration data for node '" + node.name + "':\n" + result.error().what()).c_str());
        }
        else
        {
            refresh_node(result.payload());
        }
    }
}

static std::string prettify_name(std::string const& name)
{
    std::string new_name = name;
    boost::trim(new_name);
    char old_c = ' ';
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

static QIcon get_icon(std::string const& node_icon_name, silk::Comms::Node_Def const& node_def)
{
    auto name = prettify_name(node_def.name);
    QString icon_name(q::util::format<std::string>(":/icons/nodes/{}.png", name).c_str());
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
        icon_name = q::util::format<std::string>(":/icons/nodes/{}.png", name).c_str();
        if (QFile::exists(icon_name))
        {
            return QIcon(icon_name);
        }
        name += " ";
    }

    return QIcon(node_icon_name.c_str());
}


std::string Nodes_Widget::compute_unique_name(std::string const& name) const
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

void Nodes_Widget::show_block_context_menu(QGraphicsSceneMouseEvent* event, QNEBlock* block)
{
    QASSERT(block);

    QMenu menu(this);

    auto it = m_nodes.find(block->id().toLatin1().data());
    if (it != m_nodes.end())
    {
        std::shared_ptr<Node> node = it->second;
        if (node && node->type == silk::node::IMultirotor_Simulator::TYPE)
        {
            auto action = menu.addAction(QIcon(":/icons/ui/simulator.png"), "View Simulatior");
            connect(action, &QAction::triggered, [=](bool)
            {
//                delete m_sim_window;
//                m_sim_window = new Sim_Window(m_hal, node, m_comms, m_context, this);
//                m_sim_window->show();
            });
        }

        QAction* action = menu.addAction(QIcon(":/icons/ui/minus.png"), q::util::format<std::string>("Remove {}", block->id().toLatin1().data()).c_str());
        connect(action, &QAction::triggered, [this, node](bool)
        {
            remove_node_dialog(node);
        });

        menu.addSeparator();

        bool supports_any_calibration = false;
        for (size_t i = 0; i < node->outputs.size(); i++)
        {
            Node::Output const& os = node->outputs[i];

            if (supports_acceleration_calibration(*node, os))
            {
                auto action = menu.addAction(QIcon(":/icons/ui/calibrate.png"), "Start Acceleration Calibration");
                connect(action, &QAction::triggered, [=](bool)
                {
                    do_acceleration_calibration(*node, i);
                });

                supports_any_calibration = true;
                continue;
            }
            if (supports_angular_velocity_calibration(*node, os))
            {
                auto action = menu.addAction(QIcon(":/icons/ui/calibrate.png"), "Start Angular Velocity Calibration");
                connect(action, &QAction::triggered, [=](bool)
                {
                    do_angular_velocity_calibration(*node, i);
                });

                supports_any_calibration = true;
                continue;
            }
            if (supports_magnetic_field_calibration(*node, os))
            {
                auto action = menu.addAction(QIcon(":/icons/ui/calibrate.png"), "Start Magnetic Field Calibration");
                connect(action, &QAction::triggered, [=](bool)
                {
                    do_magnetic_field_calibration(*node, i);
                });

                supports_any_calibration = true;
                continue;
            }
        }

        menu.exec(event->screenPos());
    }
}

void Nodes_Widget::open_stream_viewer(std::string const& stream_path)
{
    auto it = m_streams.find(stream_path);
    if (it == m_streams.end())
    {
        QASSERT(0);
        return;
    }

    Stream const& stream = it->second;
    Node::Output const& output = stream.node->outputs[stream.output_idx];

    Stream_Viewer_Window* viewer = new Stream_Viewer_Window(nullptr);
    viewer->init(*m_comms, stream_path, output.rate, output.type);
    viewer->show();
}

void Nodes_Widget::show_port_context_menu(QGraphicsSceneMouseEvent* event, QNEPort* port)
{
    QASSERT(port);

    QMenu menu(this);

    QAction* view_stream_action = menu.addAction(QIcon(":/icons/ui/view.png"), "View Stream");
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
        std::string stream_path = (output_port->block()->id() + "/" + output_port->id()).toLatin1().data();
        connect(view_stream_action, &QAction::triggered, [=](bool) { open_stream_viewer(stream_path); });
    }
    else
    {
        view_stream_action->setEnabled(false);
    }

    menu.exec(event->screenPos());
}

void Nodes_Widget::show_connection_context_menu(QGraphicsSceneMouseEvent* event, QNEConnection* connection)
{
    QASSERT(connection);

    QNEPort* input_port = !connection->port1()->isOutput() ? connection->port1() : connection->port2();
    QASSERT(!input_port->isOutput());

    auto* block = input_port->block();
    QASSERT(block);

    auto it = m_nodes.find(block->id().toLatin1().data());
    if (it == m_nodes.end())
    {
        return;
    }
    std::shared_ptr<Node> node = it->second;

    QMenu menu(this);

//    QAction* action = menu.addAction(QIcon(":/icons/ui/view.png"), "Open Viewer");
//    menu.addSeparator();
    QAction* action = menu.addAction(QIcon(":/icons/ui/minus.png"), "Disconnect");

    connect(action, &QAction::triggered, [=](bool)
    {
        std::string input_name = input_port->id().toLatin1().data();
        set_node_input_stream_path(*node, input_name, std::string());
    });

    menu.exec(event->screenPos());
}

void Nodes_Widget::show_context_menu(QGraphicsSceneMouseEvent* event)
{
    QMenu menu(this);

    QPointF pos = event->scenePos();

    QMenu* sources = menu.addMenu(QIcon(":/icons/nodes/Source.png"), "Sources");
    QMenu* sinks = menu.addMenu(QIcon(":/icons/nodes/Sink.png"), "Sinks");
    QMenu* processors = menu.addMenu(QIcon(":/icons/nodes/Processor.png"), "Processors");
    QMenu* lpfs = menu.addMenu(QIcon(":/icons/nodes/LPF.png"), "Low Pass Filters");
    QMenu* combiners = menu.addMenu(QIcon(":/icons/nodes/Combiner.png"), "Combiners");
    QMenu* resamplers = menu.addMenu(QIcon(":/icons/nodes/Resampler.png"), "Resamplers");
    QMenu* controllers = menu.addMenu(QIcon(":/icons/nodes/Controller.png"), "Controllers");
    QMenu* transformers = menu.addMenu(QIcon(":/icons/nodes/Transformer.png"), "Transformers");
    QMenu* generators = menu.addMenu(QIcon(":/icons/nodes/Generator.png"), "Generators");
    QMenu* pilots = menu.addMenu(QIcon(":/icons/nodes/Pilot.png"), "Pilots");
    QMenu* misc = menu.addMenu(QIcon(":/icons/nodes/Node.png"), "Misc");
    sources->setEnabled(false);
    sinks->setEnabled(false);
    processors->setEnabled(false);
    lpfs->setEnabled(false);
    combiners->setEnabled(false);
    resamplers->setEnabled(false);
    controllers->setEnabled(false);
    transformers->setEnabled(false);
    generators->setEnabled(false);
    pilots->setEnabled(false);
    misc->setEnabled(false);

    for (silk::Comms::Node_Def const& def: m_node_defs)
    {
        QAction* action = nullptr;
        if (def.type == silk::node::ISource::TYPE)
        {
            action = sources->addAction(get_icon(":/icons/nodes/Source.png", def), prettify_name(def.name).c_str());
            sources->setEnabled(true);
        }
        else if (def.type == silk::node::ISink::TYPE)
        {
            action = sinks->addAction(get_icon(":/icons/nodes/Sink.png", def), prettify_name(def.name).c_str());
            sinks->setEnabled(true);
        }
        else if (def.type == silk::node::IProcessor::TYPE)
        {
            action = processors->addAction(get_icon(":/icons/nodes/Processor.png", def), prettify_name(def.name).c_str());
            processors->setEnabled(true);
        }
        else if (def.type == silk::node::ILPF::TYPE)
        {
            action = lpfs->addAction(get_icon(":/icons/nodes/LPF.png", def), prettify_name(def.name).c_str());
            lpfs->setEnabled(true);
        }
        else if (def.type == silk::node::ICombiner::TYPE)
        {
            action = combiners->addAction(get_icon(":/icons/nodes/Combiner.png", def), prettify_name(def.name).c_str());
            combiners->setEnabled(true);
        }
        else if (def.type == silk::node::IResampler::TYPE)
        {
            action = resamplers->addAction(get_icon(":/icons/nodes/Resampler.png", def), prettify_name(def.name).c_str());
            resamplers->setEnabled(true);
        }
        else if (def.type == silk::node::IPilot::TYPE)
        {
            action = pilots->addAction(get_icon(":/icons/nodes/Pilot.png", def), prettify_name(def.name).c_str());
            pilots->setEnabled(true);
        }
        else if (def.type == silk::node::IController::TYPE)
        {
            action = controllers->addAction(get_icon(":/icons/nodes/Controller.png", def), prettify_name(def.name).c_str());
            controllers->setEnabled(true);
        }
        else if (def.type == silk::node::ITransformer::TYPE)
        {
            action = transformers->addAction(get_icon(":/icons/nodes/Transformer.png", def), prettify_name(def.name).c_str());
            transformers->setEnabled(true);
        }
        else if (def.type == silk::node::IGenerator::TYPE)
        {
            action = generators->addAction(get_icon(":/icons/nodes/Generator.png", def), prettify_name(def.name).c_str());
            generators->setEnabled(true);
        }
        else
        {
            action = misc->addAction(get_icon(":/icons/nodes/Node.png", def), prettify_name(def.name).c_str());
            misc->setEnabled(true);
        }

        if (action)
        {
            connect(action, &QAction::triggered, [def, pos, this](bool)
            {
                add_node_dialog(def, pos);
            });
        }
    }

    menu.exec(event->screenPos());
}

bool Nodes_Widget::set_node_input_stream_path(Node const& node, std::string const& input_name, std::string const& stream_path)
{
    auto it = std::find_if(node.inputs.begin(), node.inputs.end(), [&input_name](Node::Input const& input) { return input.name == input_name; });
    if (it == node.inputs.end())
    {
        QMessageBox::critical(this, "Error", ("Cannot find input '" + input_name + "' in node '" + node.name + "'").c_str());
        return false;
    }

    auto result = m_comms->set_node_input_stream_path(node.name, input_name, stream_path);
    if (result != ts::success)
    {
        QMessageBox::critical(this, "Error", result.error().what().c_str());
        return false;
    }

    refresh_node(result.payload());

    return true;
}

void Nodes_Widget::remove_node_dialog(std::shared_ptr<Node> node)
{
    auto answer = QMessageBox::question(this, "Question", q::util::format<std::string>("Are you sure you want to remove node {}", node->name).c_str());
    if (answer == QMessageBox::Yes)
    {
        auto result = m_comms->remove_node(node->name);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", result.error().what().c_str());
            return;
        }

        //refresh all nodes
        refresh_nodes();
    }
}

void Nodes_Widget::add_node_dialog(silk::Comms::Node_Def const& def, QPointF pos)
{
    std::shared_ptr<ts::IStruct_Value> descriptor = def.default_descriptor->get_specialized_type()->create_specialized_value();
    if (!descriptor)
    {
        QMessageBox::critical(this, "Error", "Cannot create descriptor");
        return;
    }

    auto construct_result = descriptor->copy_construct(*def.default_descriptor);
    if (construct_result != ts::success)
    {
        QMessageBox::critical(this, "Error", ("Cannot copy constructor: " + construct_result.error().what()).c_str());
        return;
    }

    Ui::New_Node_Dialog ui;
    QDialog dialog;
    ui.setupUi(&dialog);
    ui.properties->init(m_browser->get_editor_factory());
    ui.properties->set_value(descriptor);

    ui.properties->expandAll();
    ui.properties->header()->resizeSections(QHeaderView::ResizeToContents);
    ui.name->setText(compute_unique_name(prettify_name(def.name)).c_str());

    if (dialog.exec() == QDialog::Accepted)
    {
        std::string node_name = ui.name->text().toLatin1().data();
        set_node_position(node_name, pos);
        auto result = m_comms->add_node(node_name, def.name, descriptor);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", result.error().what().c_str());
            return;
        }

        add_node(result.payload());
    }
}

void Nodes_Widget::add_node(silk::Comms::Node const& src_node)
{
    QPointF pos = get_node_position(src_node.name);

    std::shared_ptr<Node>& dst_node = m_nodes[src_node.name];
    dst_node = std::make_shared<Node>();

    QNEBlock* b = new QNEBlock();
    m_scene->addItem(b);
    b->setName(src_node.name.c_str());
    b->setId(src_node.name.c_str());
    b->setPos(pos);

    b->setBrush(QBrush(Qt::white));
    auto it = s_node_colors.find(src_node.type);
    if (it != s_node_colors.end())
    {
        b->setBrush(it->second);
    }

    std::string node_name = src_node.name;
    b->sig_position_changed.connect([this, node_name](const QPointF& pos)
    {
        set_node_position(node_name, pos);
    });

    dst_node->name = src_node.name;
    dst_node->type = src_node.type;
    dst_node->descriptor = src_node.descriptor;
    dst_node->config = src_node.config;

    dst_node->block = b;

    for (silk::Comms::Node::Input const& src_input: src_node.inputs)
    {
        QNEPort* port = b->addInputPort(QString());
        port->setBrush(QBrush(QColor(0xe67e22)));
        port->setId(src_input.name.c_str());
        port->setToolTip(silk::stream::get_as_string(src_input.type, true).c_str());
        port->setName(q::util::format<std::string>("{} {}Hz", prettify_name(src_input.name), src_input.rate).c_str());
        port->setPortType(src_input.type.get_id());
        port->setPortRate(src_input.rate);

        std::string input_name = src_input.name;
        port->sig_connected.connect([dst_node, input_name, port, this](QNEPort* output_port)
        {
            QNEBlock* block = output_port->block();
            std::string node_name = block->id().toLatin1().data();
            std::string stream_path(node_name);
            stream_path += "/";
            stream_path += output_port->id().toLatin1().data();
            if (!set_node_input_stream_path(*dst_node, input_name, stream_path))
            {
                port->disconnectAll();
            }
        });

        Node::Input dst_input;
        dst_input.name = src_input.name;
        dst_input.rate = src_input.rate;
        dst_input.stream_path = src_input.stream_path;
        dst_input.type = src_input.type;
        dst_input.port = port;
        dst_node->inputs.push_back(dst_input);
    }
    for (size_t i = 0; i < src_node.outputs.size(); i++)
    {
        silk::Comms::Node::Output const& src_output = src_node.outputs[i];

        QNEPort* port = b->addOutputPort(QString());
        port->setBrush(QBrush(QColor(0x9b59b6)));
        port->setId(src_output.name.c_str());
        port->setToolTip(silk::stream::get_as_string(src_output.type, true).c_str());
        port->setName(q::util::format<std::string>("{} {}Hz", prettify_name(src_output.name), src_output.rate).c_str());
        port->setPortType(src_output.type.get_id());
        port->setPortRate(src_output.rate);

        Node::Output dst_output;
        dst_output.name = src_output.name;
        dst_output.rate = src_output.rate;
        dst_output.type = src_output.type;
        dst_output.port = port;
        dst_node->outputs.push_back(dst_output);

        std::string stream_name = dst_node->name + "/" + dst_output.name;
        Stream& stream = m_streams[stream_name];
        stream.node = dst_node;
        stream.block = b;
        stream.port = port;
        stream.output_idx = i;
    }

    refresh_node(src_node);
}

void Nodes_Widget::refresh_node(silk::Comms::Node const& src_node)
{
    auto it = m_nodes.find(src_node.name);
    QASSERT(it != m_nodes.end());
    if (it == m_nodes.end())
    {
        return;
    }

    std::shared_ptr<Node> const& dst_node = it->second;

    auto result = dst_node->descriptor->copy_assign(*src_node.descriptor);
    QASSERT(result == ts::success);

    result = dst_node->config->copy_assign(*src_node.config);
    QASSERT(result == ts::success);

    QASSERT(src_node.inputs.size() == dst_node->inputs.size());

    for (size_t i = 0; i < src_node.inputs.size(); i++)
    {
        silk::Comms::Node::Input const& src_input = src_node.inputs[i];
        Node::Input& dst_input = dst_node->inputs[i];

        dst_input.stream_path = src_input.stream_path;

        dst_input.port->disconnectAll();
        auto it = m_streams.find(dst_input.stream_path);
        if (it == m_streams.end())
        {
            continue;
        }

        Stream const& stream = it->second;

        QNEConnection* connection = new QNEConnection(0);
        m_scene->addItem(connection);
        connection->setPort1((QNEPort*)dst_input.port);
        connection->setPort2(stream.port);
        connection->updatePath();
    }
}

void Nodes_Widget::on_selection_changed()
{
    m_browser->set_value(nullptr);

    m_selection.node = nullptr;
    if (m_upload_action)
    {
        m_upload_action->setEnabled(false);
    }

    QList<QGraphicsItem*> items = m_scene->selectedItems();
    if (items.empty())
    {
        return;
    }
    QGraphicsItem* item = items.at(0);
    if (!item || item->type() != QNEBlock::Type)
    {
        return;
    }
    QNEBlock* block = reinterpret_cast<QNEBlock*>(item);

    auto it = m_nodes.find(block->id().toLatin1().data());
    if (it == m_nodes.end())
    {
        return;
    }
    m_browser->set_value(it->second->config);
    m_browser->expandAll();

    m_selection.node = it->second;
    if (m_upload_action)
    {
        m_upload_action->setEnabled(true);
    }
}

void Nodes_Widget::set_node_position(std::string const& node_name, QPointF const& pos)
{
    std::string settings_file = "settings_";
    if (m_uav_name.empty())
    {
        settings_file += "noname";
    }
    else
    {
        settings_file += m_uav_name;
    }
    QSettings settings(settings_file.c_str(), QSettings::IniFormat);

    settings.beginGroup(("nodes/" + node_name + "").c_str());
    settings.setValue("position", pos);
    settings.endGroup();
}

QPointF Nodes_Widget::get_node_position(std::string const& node_name)
{
    std::string settings_file = "settings_";
    if (m_uav_name.empty())
    {
        settings_file += "noname";
    }
    else
    {
        settings_file += m_uav_name;
    }
    QSettings settings(settings_file.c_str(), QSettings::IniFormat);

    settings.beginGroup(("nodes/" + node_name + "").c_str());
    QPointF pos = settings.value("position", QPointF()).toPointF();
    settings.endGroup();

    return pos;
}

void Nodes_Widget::load_editor_data()
{

}

void Nodes_Widget::save_editor_data()
{

}

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

#include "boost/algorithm/string.hpp"

#include "ui_New_Node_Dialog.h"

#include <QVBoxLayout>
#include <QMessageBox>

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
//    { silk::node::IMulti_Simulator::TYPE, QColor(0xF1C40F) },
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

    //    m_selection.config_dock = new QDockWidget(this);
    //    m_selection.config_view = new QTreeView(m_selection.config_dock);
    //    m_selection.config_model = new JSON_Model(m_selection.config_view);
    //    connect(m_selection.config_model, &QAbstractItemModel::dataChanged, this, &HAL_Window::on_config_changed);

    //    m_selection.config_dock->setWidget(m_selection.config_view);
    //    m_selection.config_view->setModel(m_selection.config_model);
    //    m_selection.config_view->setItemDelegate(new Custom_Item_Delegate(m_selection.config_view));

    //    addDockWidget(Qt::RightDockWidgetArea, m_selection.config_dock);

    //    connect(m_scene, &QGraphicsScene::selectionChanged, [this]() { selection_changed(); });

    connect(m_nodes_editor, &QNodesEditor::contextMenu, this, &Nodes_Widget::show_context_menu);
    //    connect(m_nodes_editor, &QNodesEditor::portContextMenu, this, &HAL_Window::port_context_menu);
    //    connect(m_nodes_editor, &QNodesEditor::blockContextMenu, this, &HAL_Window::block_context_menu);
    //    connect(m_nodes_editor, &QNodesEditor::connectionContextMenu, this, &HAL_Window::connection_context_menu);
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
    if (!active)
    {
        if (m_refresh_action)
        {
            delete m_refresh_action;
            m_refresh_action = nullptr;

            //            delete m_upload_action;
            //            m_upload_action = nullptr;

            //            delete m_new_action;
            //            m_new_action = nullptr;
        }
        return;
    }
    m_refresh_action = m_toolbar->addAction(QIcon(":/icons/ui/reconnect.png"), "Refresh");
    QObject::connect(m_refresh_action, &QAction::triggered, [this](bool) { refresh(); });

    //    m_upload_action = m_toolbar->addAction(QIcon(":/icons/ui/upload.png"), "Upload");
    //    QObject::connect(m_upload_action, &QAction::triggered, [this](bool) { upload(); });

    //    m_new_action = m_toolbar->addAction(QIcon(":/icons/ui/new.png"), "New");
    //    QObject::connect(m_new_action, &QAction::triggered, [this](bool) { show_new_descriptor_menu(); });

    m_browser->set_value(nullptr);
}

void Nodes_Widget::refresh()
{
    if (!m_comms->is_connected())
    {
        QMessageBox::critical(this, "Error", "Not connected to any UAV");
        return;
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

    m_scene->clear();
    m_nodes.clear();

    std::vector<silk::Comms::Node> nodes = result.extract_payload();
    for (silk::Comms::Node const& node: nodes)
    {
        add_node(node);
    }
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

static auto get_icon(std::string const& node_icon_name, silk::Comms::Node_Def const& node_def) -> QIcon
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
            connect(action, &QAction::triggered, [=](bool) { try_add_node(def, pos); });
        }
    }

    menu.exec(event->screenPos());
}

void Nodes_Widget::try_add_node(silk::Comms::Node_Def const& def, QPointF pos)
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
        //        set_node_position(node_name, pos);
        auto result = m_comms->add_node(node_name, def.name, descriptor);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", result.error().what().c_str());
            return;
        }

        //refresh_nodes();
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
    b->positionChangedSignal.connect([this, node_name](const QPointF& pos)
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
        auto port = b->addInputPort(QString());
        port->setBrush(QBrush(QColor(0xe67e22)));
        port->setId(src_input.name.c_str());
        port->setToolTip(silk::stream::get_as_string(src_input.type, true).c_str());
        if (src_input.rate > 0)
        {
            port->setName(q::util::format<std::string>("{} {}Hz", src_input.name, src_input.rate).c_str());
        }
        else
        {
            port->setName(q::util::format<std::string>("{}", src_input.name).c_str());
        }
        port->setPortType(src_input.type.get_id());
        port->setPortRate(src_input.rate);

        auto input_name = src_input.name;
        port->connectedSignal.connect([src_node, input_name, this](QNEPort* output_port)
        {
            auto* block = output_port->block();
            std::string node_name = block->id().toLatin1().data();
            q::Path stream_path(node_name);
            stream_path += output_port->id().toLatin1().data();
//            m_hal.set_node_input_stream_path(node, input_name, stream_path);
        });

        Node::Input& dst_input = dst_node->inputs[src_input.name];
        dst_input.name = src_input.name;
        dst_input.rate = src_input.rate;
        dst_input.stream_path = src_input.stream_path;
        dst_input.type = src_input.type;
        dst_input.port = port;
    }
    for (silk::Comms::Node::Output const& src_output: src_node.outputs)
    {
        auto port = b->addOutputPort(QString());
        port->setBrush(QBrush(QColor(0x9b59b6)));
        port->setId(src_output.name.c_str());
        port->setToolTip(silk::stream::get_as_string(src_output.type, true).c_str());
        port->setName(q::util::format<std::string>("{} {}Hz", src_output.name, src_output.rate).c_str());
        port->setPortType(src_output.type.get_id());
        port->setPortRate(src_output.rate);

        Node::Output& dst_output = dst_node->outputs[src_output.name];
        dst_output.name = src_output.name;
        dst_output.rate = src_output.rate;
        dst_output.type = src_output.type;
        dst_output.port = port;

//        auto& stream_data = m_ui_streams[src_output.stream->name];
//        stream_data.stream = src_output.stream;
//        stream_data.port = dst_output.port;
//        stream_data.block = dst_node.block;
    }

//    auto* node_ptr = src_node.get();
//    m_connections.push_back( src_node.changed_signal.connect([this, node_ptr]()
//    {
//        refresh_node(*node_ptr);
//    }) );
}

void Nodes_Widget::set_node_position(std::string const& node_name, QPointF const& pos)
{

}

QPointF Nodes_Widget::get_node_position(std::string const& node_name)
{
    return QPointF();
}

void Nodes_Widget::load_editor_data()
{

}

void Nodes_Widget::save_editor_data()
{

}

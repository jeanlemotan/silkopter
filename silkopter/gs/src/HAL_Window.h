#pragma once

#include <QMainWindow>
#include <QtWidgets>

#include "Render_Widget.h"

#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneport.h"

#include "HAL.h"
#include "Comms.h"

class QNodesEditor;
class JSON_Model;
class Sim_Window;

class HAL_Window : public QMainWindow
{
public:
    explicit HAL_Window(silk::HAL& hal, silk::Comms& comms, Render_Context& context, QWidget* parent = 0);
    ~HAL_Window();

    void process();

private:
    void on_nodes_refreshed();
    void on_config_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

private:
    void refresh_nodes();

    void context_menu(QGraphicsSceneMouseEvent* event);
    void port_context_menu(QGraphicsSceneMouseEvent* event, QNEPort* port);
    void block_context_menu(QGraphicsSceneMouseEvent* event, QNEBlock* block);
    void connection_context_menu(QGraphicsSceneMouseEvent* event, QNEConnection* connection);

    void selection_changed();
    void set_config_editor_node(silk::node::gs::Node_ptr node);

    void open_stream_viewer(std::string const& stream_name);

    auto supports_acceleration_calibration(silk::node::gs::Node const& node, silk::node::gs::Node::Output const& output) const -> bool;
    auto supports_angular_velocity_calibration(silk::node::gs::Node const& node, silk::node::gs::Node::Output const& output) const -> bool;
    auto supports_magnetic_field_calibration(silk::node::gs::Node const& node, silk::node::gs::Node::Output const& output) const -> bool;

    void do_acceleration_calibration(silk::node::gs::Node_ptr node, size_t output_idx);
    void do_magnetic_field_calibration(silk::node::gs::Node_ptr node, size_t output_idx);
    void do_angular_velocity_calibration(silk::node::gs::Node_ptr node, size_t output_idx);

    void add_stream(silk::stream::gs::Stream_ptr stream);
    void remove_stream(silk::stream::gs::Stream_ptr stream);

    void refresh_node(silk::node::gs::Node& node);
    void add_node(silk::node::gs::Node_ptr node);
    void remove_node(silk::node::gs::Node_ptr node);
    void try_add_node(silk::node::gs::Node_Def_ptr def, QPointF pos);
    void try_remove_node(silk::node::gs::Node_ptr node);

    std::vector<q::util::Scoped_Connection> m_connections;

    silk::HAL& m_hal;
    silk::Comms& m_comms;
    Render_Context& m_context;

    QNodesEditor* m_nodes_editor;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

    rapidjson::Document m_editor_data;

    struct Selection
    {
        silk::node::gs::Node_ptr node;
        QDockWidget* config_dock = nullptr;
        QTreeView* config_view = nullptr;
        rapidjson::Document config_json;
        JSON_Model* config_model = nullptr;
    } m_selection;

    struct UI_Stream
    {
        silk::stream::gs::Stream_wptr stream;
        QNEPort* port = nullptr;
        QNEBlock* block = nullptr;
    };

    struct UI_Node
    {
        silk::node::gs::Node_wptr node;

        QNEBlock* block = nullptr;
        struct UI_Input
        {
            QNEPort* port = nullptr;
        };
        struct UI_Output
        {
            QNEPort* port = nullptr;
        };
        std::map<std::string, UI_Input> ui_inputs;
        std::map<std::string, UI_Output> ui_outputs;
    };

    std::map<std::string, UI_Stream> m_ui_streams;
    std::map<std::string, UI_Node> m_ui_nodes;

    Sim_Window* m_sim_window = nullptr;

    auto compute_unique_name(std::string const& name) const -> std::string;

    void set_node_position(std::string const& node_name, QPointF const& pos);
    auto get_node_position(std::string const& node_name) -> QPointF;

    void save_editor_data();
    void load_editor_data();
};


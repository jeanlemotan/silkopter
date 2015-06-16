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
    void on_node_factories_refreshed();
    void on_nodes_refreshed();
    void on_config_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

private:
    void refresh_nodes();

    void context_menu(QGraphicsSceneMouseEvent* event);
    void port_context_menu(QGraphicsSceneMouseEvent* event, QNEPort* port);
    void block_context_menu(QGraphicsSceneMouseEvent* event, QNEBlock* block);
    void connection_context_menu(QGraphicsSceneMouseEvent* event, QNEConnection* connection);

    void selection_changed();
    void set_config_editor_node(silk::node::Node_ptr node);

    void open_stream_viewer(std::string const& stream_name);

    void do_acceleration_calibration(silk::node::stream::Stream_ptr stream);
    void do_magnetic_field_calibration(silk::node::stream::Stream_ptr stream);
    void do_angular_velocity_calibration(silk::node::stream::Stream_ptr stream);


    void refresh_node(silk::node::Node& node);
    void add_node(silk::node::Node_ptr node);
    void remove_node(silk::node::Node_ptr node);
    void try_add_node(silk::node::Node_Def_ptr def, QPointF pos);
    void try_remove_node(silk::node::Node_ptr node);

    std::vector<q::util::Connection> m_connections;

    silk::HAL& m_hal;
    silk::Comms& m_comms;
    QNodesEditor* m_nodes_editor;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

    rapidjson::Document m_editor_data;

    struct Selection
    {
        silk::node::Node_ptr node;
        QDockWidget* config_dock = nullptr;
        QTreeView* config_view = nullptr;
        rapidjson::Document config_json;
        JSON_Model* config_model = nullptr;
    } m_selection;

    struct Stream_Data
    {
        silk::node::stream::Stream_wptr stream;
        QNEPort* port = nullptr;
        QNEBlock* block = nullptr;
    };

    struct Node_Data
    {
        silk::node::Node_wptr node;

        QNEBlock* block = nullptr;
        struct Input_Data
        {
            QNEPort* port = nullptr;
        };
        struct Output_Data
        {
            QNEPort* port = nullptr;
        };
        std::map<std::string, Input_Data> inputs;
        std::map<std::string, Output_Data> outputs;
    };

    std::map<std::string, Stream_Data> m_streams;
    std::map<std::string, Node_Data> m_nodes;

    Render_Context& m_context;
    Sim_Window* m_sim_window = nullptr;

    auto compute_unique_name(std::string const& name) const -> std::string;

    void set_node_position(std::string const& node_name, QPointF const& pos);
    auto get_node_position(std::string const& node_name) -> QPointF;

    void save_editor_data();
    void load_editor_data();
};


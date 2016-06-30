#pragma once

#include <QToolBar>
#include <QGraphicsView>
#include "ITab_Widget.h"
#include "Comms.h"

#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneport.h"

#include <QGraphicsSceneMouseEvent>

class Properties_Browser;

class Nodes_Widget : public ITab_Widget
{
public:
    Nodes_Widget();
    ~Nodes_Widget();

    void init(QToolBar* toolbar, silk::Comms& comms, Properties_Browser& browser);

    void refresh();
private:
    void refresh_node_defs();
    void refresh_nodes();

    void set_active(bool active) override;

    void on_selection_changed();

    //std::vector<q::util::Scoped_Connection> m_connections;

    QToolBar* m_toolbar = nullptr;
    QAction* m_refresh_action = nullptr;
    silk::Comms* m_comms = nullptr;
    Properties_Browser* m_browser = nullptr;

    QNodesEditor* m_nodes_editor = nullptr;
    QGraphicsView* m_view = nullptr;
    QGraphicsScene* m_scene = nullptr;

    struct Selection
    {
        //silk::Comms::Node node;
        //QDockWidget* config_dock = nullptr;
        //QTreeView* config_view = nullptr;
        //rapidjson::Document config_json;
        //JSON_Model* config_model = nullptr;
    } m_selection;

    struct Node
    {
        std::string name;
        silk::node::Type type;
        std::shared_ptr<ts::IStruct_Value> descriptor;
        std::shared_ptr<ts::IStruct_Value> config;

        QNEBlock* block = nullptr;

        struct Input
        {
            std::string stream_path;
            silk::stream::Type type;
            std::string name;
            uint32_t rate = 0;

            QNEPort* port = nullptr;
        };
        struct Output
        {
            silk::stream::Type type;
            std::string name;
            uint32_t rate = 0;

            QNEPort* port = nullptr;
        };
        std::vector<Input> inputs;
        std::vector<Output> outputs;
    };

    struct Stream
    {
        std::shared_ptr<Node> node;
        size_t output_idx = 0;
        QNEPort* port = nullptr;
        QNEBlock* block = nullptr;
    };

    std::map<std::string, Stream> m_streams;
    std::map<std::string, std::shared_ptr<Node>> m_nodes;

    std::vector<silk::Comms::Node_Def> m_node_defs;

    std::string m_uav_name;

//    Sim_Window* m_sim_window = nullptr;

    std::string compute_unique_name(std::string const& name) const;

    void set_node_position(std::string const& node_name, QPointF const& pos);
    QPointF get_node_position(std::string const& node_name);

    void save_editor_data();
    void load_editor_data();

    bool supports_acceleration_calibration(Node const& node, Node::Output const& output) const;
    bool supports_angular_velocity_calibration(Node const& node, Node::Output const& output) const;
    bool supports_magnetic_field_calibration(Node const& node, Node::Output const& output) const;

    void do_acceleration_calibration(Node const& node, size_t output_idx);
    void do_magnetic_field_calibration(Node const& node, size_t output_idx);
    void do_angular_velocity_calibration(Node const& node, size_t output_idx);


    void show_context_menu(QGraphicsSceneMouseEvent*);
    void show_port_context_menu(QGraphicsSceneMouseEvent* event, QNEPort* port);
    void show_connection_context_menu(QGraphicsSceneMouseEvent* event, QNEConnection* connection);
    void show_block_context_menu(QGraphicsSceneMouseEvent* event, QNEBlock* block);

    void remove_node_dialog(std::shared_ptr<Node> node);

    bool set_node_input_stream_path(Node const& node, std::string const& input_name, std::string const& stream_path);

    void add_node_dialog(silk::Comms::Node_Def const& def, QPointF pos);
    void add_node(silk::Comms::Node const& node);
    void refresh_node(silk::Comms::Node const& src_node);
};

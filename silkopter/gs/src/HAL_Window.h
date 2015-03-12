#pragma once

#include <QMainWindow>
#include <QtWidgets>

#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneport.h"

#include "HAL.h"

class QNodesEditor;
class JSON_Model;

class HAL_Window : public QMainWindow
{
public:
    explicit HAL_Window(silk::HAL& hal, QWidget* parent = 0);
    ~HAL_Window();

private:
    void on_node_factories_refreshed();
    void on_nodes_refreshed();
    void on_config_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);

private:
    void refresh_nodes();

    void contextMenuEvent(QContextMenuEvent* event);

    void selection_changed();
    void set_config_editor_node(silk::node::Node_ptr node);

    void refresh_node(silk::node::Node& node);
    void add_node(silk::node::Node_ptr node, QPointF pos);
    void create_node(silk::node::Node_Def_ptr def, QPointF pos);

    silk::HAL& m_hal;
    QNodesEditor* m_nodes_editor;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

    struct Selection
    {
        silk::node::Node_ptr node;
        QDockWidget* config_dock = nullptr;
        QTreeView* config_view = nullptr;
        rapidjson::Document config_json;
        JSON_Model* config_model = nullptr;
    } m_selection;

    struct Node_Data
    {
        std::shared_ptr<QNEBlock> block;
        struct Input_Data
        {
            uint32_t rate = 0;
            std::shared_ptr<QNEPort> port;
        };
        struct Output_Data
        {
            uint32_t rate = 0;
            std::shared_ptr<QNEPort> port;
        };
        std::map<std::string, Input_Data> inputs;
        std::map<std::string, Output_Data> outputs;
    };

    std::map<std::string, Node_Data> m_nodes;

    auto compute_unique_name(std::string const& name) const -> std::string;
};


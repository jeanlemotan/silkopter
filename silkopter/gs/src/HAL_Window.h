#pragma once

#include <QMainWindow>
#include <QtWidgets>

#include "qneblock.h"
#include "qnodeseditor.h"
#include "qneport.h"

#include "HAL.h"

class QNodesEditor;

class HAL_Window : public QMainWindow
{
public:
    explicit HAL_Window(silk::HAL& hal, QWidget* parent = 0);
    ~HAL_Window();

private:
    void on_node_factories_refreshed();
    void on_nodes_refreshed();

private:
    void contextMenuEvent(QContextMenuEvent* event);

    void refresh_source(silk::node::source::Source& node);
    void add_source(silk::node::source::Source_ptr node, QPointF pos);
    void create_source(silk::node::source::Source_Def_ptr def, QPointF pos);

    void add_sink(silk::node::sink::Sink_ptr node, QPointF pos);
    void create_sink(silk::node::sink::Sink_Def_ptr def, QPointF pos);

    void refresh_processor(silk::node::processor::Processor& node);
    void add_processor(silk::node::processor::Processor_ptr node, QPointF pos);
    void create_processor(silk::node::processor::Processor_Def_ptr def, QPointF pos);

    silk::HAL& m_hal;
    QNodesEditor* m_nodesEditor;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

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
};


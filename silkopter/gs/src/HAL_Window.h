#pragma once

#include <QMainWindow>
#include <QtWidgets>

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

    void create_source(silk::node::source::Source_ptr def, QPointF pos);
    void create_sink(silk::node::sink::Sink_ptr def, QPointF pos);
    void create_processor(silk::node::processor::Processor_ptr def, QPointF pos);

    silk::HAL& m_hal;
    QNodesEditor* m_nodesEditor;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

    struct Item_Key
    {
        q::rtti::class_id class_id;
        std::string name;

        bool operator==(Item_Key const& other) const
        {
            return class_id == other.class_id && name == other.name;
        }
    };

    struct Item_Hasher
    {
        auto operator()(Item_Key const& key) const -> size_t
        {
            size_t hash = std::hash<q::rtti::class_id>()(key.class_id);
            q::util::hash_combine(hash, std::hash<std::string>()(key.name));
            return hash;
        }
    };

    struct Item_Data
    {
        QPointF position;
    };

    std::unordered_map<Item_Key, Item_Data, Item_Hasher> m_ui_items;

};


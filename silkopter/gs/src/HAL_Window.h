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

    void create_source(std::shared_ptr<silk::node::source::GS_ISource> def);
    void create_sink(std::shared_ptr<silk::node::sink::GS_ISink> def);
    void create_processor(std::shared_ptr<silk::node::processor::GS_IProcessor> def);

    silk::HAL& m_hal;
    QNodesEditor* m_nodesEditor;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
};


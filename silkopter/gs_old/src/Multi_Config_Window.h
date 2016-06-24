#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include "ui_Multi_Config_Window.h"

#include "HAL.h"
#include "Comms.h"

class JSON_Model;

class Multi_Config_Window : public QMainWindow
{
public:
    explicit Multi_Config_Window(silk::HAL& hal, silk::Comms& comms, QWidget* parent = 0);
    ~Multi_Config_Window();

    void process();

private:
    void on_config_refreshed();
    void on_config_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    void show_config();

private:
    void contextMenu(QGraphicsSceneMouseEvent* event);

    void selection_changed();

    Ui::Multi_Config_Window m_ui;

    silk::HAL& m_hal;
    silk::Comms& m_comms;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;

    silk::config::Multi m_config;
    QDockWidget* m_config_dock = nullptr;
    QTreeView* m_config_view = nullptr;
    rapidjson::Document m_config_json;
    JSON_Model* m_config_model = nullptr;
};


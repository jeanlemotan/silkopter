#pragma once

#include <QMainWindow>
#include "ui_Simulator.h"

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>

class Simulator : public QMainWindow
{
    Q_OBJECT
public:
    explicit Simulator(QWidget* parent = 0);

signals:

public slots:

private:
    Ui::Simulator m_ui;
    Qt3DExtras::Qt3DWindow* m_view = nullptr;
    Qt3DCore::QEntity* m_rootEntity = nullptr;
    Qt3DRender::QCamera* m_cameraEntity = nullptr;

    Qt3DCore::QEntity* m_uavEntity = nullptr;
    Qt3DCore::QEntity* m_groundEntity = nullptr;
};

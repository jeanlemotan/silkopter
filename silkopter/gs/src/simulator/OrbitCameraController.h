#pragma once

#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DCore/QEntity>
#include <QVector3D>

namespace Qt3DRender {
class QCamera;
}

namespace Qt3DLogic {
class QFrameAction;
}

namespace Qt3DInput {

class QKeyboardDevice;
class QMouseDevice;
class QLogicalDevice;
class QAction;
class QActionInput;
class QAxis;
class QAnalogAxisInput;
class QButtonAxisInput;
class QAxisActionHandler;

}

class OrbitCameraController : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QCamera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(float linearSpeed READ linearSpeed WRITE setLinearSpeed NOTIFY linearSpeedChanged)
    Q_PROPERTY(float lookSpeed READ lookSpeed WRITE setLookSpeed NOTIFY lookSpeedChanged)
    Q_PROPERTY(float zoomInLimit READ zoomInLimit WRITE setZoomInLimit NOTIFY zoomInLimitChanged)

public:
    explicit OrbitCameraController(Qt3DCore::QNode *parent = nullptr);
    ~OrbitCameraController();

    Qt3DRender::QCamera *camera() const;
    float linearSpeed() const;
    float lookSpeed() const;
    float zoomInLimit() const;

    void setCamera(Qt3DRender::QCamera *camera);
    void setLinearSpeed(float linearSpeed);
    void setLookSpeed(float lookSpeed);
    void setZoomInLimit(float zoomInLimit);

Q_SIGNALS:
    void cameraChanged();
    void linearSpeedChanged();
    void lookSpeedChanged();
    void zoomInLimitChanged();

private:
    void init();

    Qt3DRender::QCamera *m_camera;

    Qt3DInput::QAction *m_leftMouseButtonAction;
    Qt3DInput::QAction *m_rightMouseButtonAction;
    Qt3DInput::QAction *m_altButtonAction;
    Qt3DInput::QAction *m_shiftButtonAction;

    Qt3DInput::QAxis *m_rxAxis;
    Qt3DInput::QAxis *m_ryAxis;
    Qt3DInput::QAxis *m_txAxis;
    Qt3DInput::QAxis *m_tyAxis;
    Qt3DInput::QAxis *m_tzAxis;

    Qt3DInput::QActionInput *m_leftMouseButtonInput;
    Qt3DInput::QActionInput *m_rightMouseButtonInput;
    Qt3DInput::QActionInput *m_altButtonInput;
    Qt3DInput::QActionInput *m_shiftButtonInput;

    Qt3DInput::QAnalogAxisInput *m_mouseRxInput;
    Qt3DInput::QAnalogAxisInput *m_mouseRyInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTxPosInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTyPosInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTzPosInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTxNegInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTyNegInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTzNegInput;

    Qt3DInput::QKeyboardDevice *m_keyboardDevice;
    Qt3DInput::QMouseDevice *m_mouseDevice;

    Qt3DInput::QLogicalDevice *m_logicalDevice;

    Qt3DLogic::QFrameAction *m_frameAction;

    float m_linearSpeed;
    float m_lookSpeed;
    float m_zoomInLimit;
    QVector3D m_cameraUp;

private Q_SLOTS:
    void _q_onTriggered(float);
};

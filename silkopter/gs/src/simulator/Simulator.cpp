#include "Simulator.h"

#include <chrono>
#include <Qt3DLogic/QFrameAction>
#include <QtGui/QScreen>
#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DInput/QInputAspect>
#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include "OrbitCameraController.h"
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseMapMaterial>
#include <Qt3DRender/QTexture>


Simulator::Simulator(QWidget* parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);

    m_view = new Qt3DExtras::Qt3DWindow();
    m_view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget* container = QWidget::createWindowContainer(m_view);
    QSize screen_size = m_view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screen_size);

    QWidget* widget = new QWidget(this);
    QHBoxLayout* hl = new QHBoxLayout(widget);
    QVBoxLayout* vl = new QVBoxLayout();
    vl->setAlignment(Qt::AlignTop);
    vl->setMargin(0);
    hl->addWidget(container, 1);
    hl->addLayout(vl);
    hl->setMargin(0);

    setCentralWidget(widget);

    Qt3DInput::QInputAspect* input = new Qt3DInput::QInputAspect;
    m_view->registerAspect(input);
}

void Simulator::init(silk::Comms& comms, std::string const& node_name)
{
    m_comms = &comms;
    m_node_name = node_name;

    m_get_state_message = std::make_shared<silk::messages::Multirotor_Simulator_Get_State_Message>();

    m_comms->sig_node_message_received.connect(std::bind(&Simulator::message_received, this, std::placeholders::_1, std::placeholders::_2));

    init_world();

    Qt3DLogic::QFrameAction* frame_action = new Qt3DLogic::QFrameAction();
    m_root_entity->addComponent(frame_action);

    QObject::connect(frame_action, SIGNAL(triggered(float)), this, SLOT(process_logic(float)));

    QObject::connect(m_ui.actionSimulation, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Simulation_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionReset, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Reset_Message>();
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionStopMotion, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Stop_Motion_Message>();
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionGravity, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Gravity_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionTerrain, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Ground_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
    QObject::connect(m_ui.actionDrag, &QAction::toggled, [this](bool yes)
    {
        auto message = std::make_shared<silk::messages::Multirotor_Simulator_Set_Drag_Enabled_Message>();
        message->set_enabled(yes);
        auto result = m_comms->send_node_message(m_node_name, message);
        QASSERT(result == ts::success);
    });
}

void Simulator::init_world()
{
    m_root_entity = new Qt3DCore::QEntity();
    m_view->setRootEntity(m_root_entity);

    m_camera_entity = m_view->camera();

    m_camera_entity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    m_camera_entity->setPosition(QVector3D(0, -20.f, 0.0f));
    m_camera_entity->setUpVector(QVector3D(0, 0, 1));
    m_camera_entity->setViewCenter(QVector3D(0, 0, 0));
    m_camera_entity->tilt(math::anglef::pi);

    OrbitCameraController* cam_controller = new OrbitCameraController(m_root_entity);
    cam_controller->setCamera(m_camera_entity);

    Qt3DCore::QEntity* light_entity = new Qt3DCore::QEntity(m_root_entity);
    Qt3DRender::QPointLight* light = new Qt3DRender::QPointLight(light_entity);
    light->setColor("white");
    light->setIntensity(1);
    light_entity->addComponent(light);
    Qt3DCore::QTransform* light_transform = new Qt3DCore::QTransform(light_entity);
    light_transform->setTranslation(QVector3D(0, 0, 20.f));
    light_entity->addComponent(light_transform);
    //m_cameraEntity->addComponent(lightEntity);

    // uav shape data
    Qt3DRender::QMesh* uav_mesh = new Qt3DRender::QMesh();
    //uavMesh->setSource(QUrl("meshes/xxx.obj"));
    uav_mesh->setSource(QUrl::fromLocalFile("meshes/quad.obj"));


    // Sphere mesh transform
    m_uav_transform = new Qt3DCore::QTransform();

    //uavTransform->setScale(0.001f);
    m_uav_transform->setTranslation(QVector3D(-5.0f, 0.0f, 0.0f));

    Qt3DExtras::QPhongMaterial* uav_material = new Qt3DExtras::QPhongMaterial();
    uav_material->setDiffuse(QColor(QRgb(0xa69929)));

    // uav
    m_uav_entity = new Qt3DCore::QEntity(m_root_entity);
    m_uav_entity->addComponent(uav_mesh);
    m_uav_entity->addComponent(uav_material);
    m_uav_entity->addComponent(m_uav_transform);

    {
        // plane shape data
        Qt3DRender::QMesh* plane_mesh = new Qt3DRender::QMesh();
        plane_mesh->setSource(QUrl::fromLocalFile("meshes/plane.obj"));

        // Sphere mesh transform
        Qt3DCore::QTransform* plane_transform = new Qt3DCore::QTransform();

        //planeTransform->setScale(0.001f);
        plane_transform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

        Qt3DExtras::QDiffuseMapMaterial* plane_material = new Qt3DExtras::QDiffuseMapMaterial();
        plane_material->setAmbient(QColor(QRgb(0xFFFFFF)));

        Qt3DRender::QTextureImage* texture_image = new Qt3DRender::QTextureImage();
        texture_image->setSource(QUrl::fromLocalFile("textures/checkerboard.png"));

        Qt3DRender::QTexture2D* texture = new Qt3DRender::QTexture2D();
        texture->setGenerateMipMaps(true);
        texture->addTextureImage(texture_image);
        texture->setWrapMode(Qt3DRender::QTextureWrapMode(Qt3DRender::QTextureWrapMode::Repeat));
        texture->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
        texture->setMagnificationFilter(Qt3DRender::QTexture2D::Nearest);
        texture->setMaximumAnisotropy(16.f);

        plane_material->setDiffuse(texture);

        // plane
        m_ground_entity = new Qt3DCore::QEntity(m_root_entity);
        m_ground_entity->addComponent(plane_mesh);
        m_ground_entity->addComponent(plane_material);
        m_ground_entity->addComponent(plane_transform);
    }
}

void Simulator::message_received(std::string const& node_name, silk::messages::INode_Message const& _message)
{
    if (node_name != m_node_name)
    {
        return;
    }

    if (silk::messages::Multirotor_Simulator_State_Message const* message = dynamic_cast<silk::messages::Multirotor_Simulator_State_Message const*>(&_message))
    {
        m_state = message->get_state();
        m_send_message = true;
    }
}

void Simulator::process_logic(float dt)
{
    auto now = Clock::now();

    if ((m_send_message == false && now - m_last_message_send_tp >= std::chrono::milliseconds(200)) ||
         m_send_message == true && now - m_last_message_send_tp >= std::chrono::milliseconds(30))
    {
        auto result = m_comms->send_node_message(m_node_name, m_get_state_message);
        QASSERT(result == ts::success);
        m_send_message = false;
        m_last_message_send_tp = now;
    }

    math::vec3f pos = m_state.get_enu_position();
    m_uav_transform->setTranslation(QVector3D(pos.x, pos.y, pos.z));
    m_camera_entity->setViewCenter(QVector3D(pos.x, pos.y, pos.z));

    math::vec4f rot = m_state.get_local_to_enu_rotation();
    m_uav_transform->setRotation(QQuaternion(rot.w, rot.x, rot.y, rot.z));
}

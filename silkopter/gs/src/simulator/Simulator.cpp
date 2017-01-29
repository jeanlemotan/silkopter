#include "Simulator.h"

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
    QSize screenSize = m_view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QWidget* widget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(widget);
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setMargin(0);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    hLayout->setMargin(0);

    setCentralWidget(widget);

    Qt3DInput::QInputAspect* input = new Qt3DInput::QInputAspect;
    m_view->registerAspect(input);

    m_rootEntity = new Qt3DCore::QEntity();
    m_view->setRootEntity(m_rootEntity);

    m_cameraEntity = m_view->camera();

    m_cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    m_cameraEntity->setPosition(QVector3D(0, -20.f, 0.0f));
    m_cameraEntity->setUpVector(QVector3D(0, 0, 1));
    m_cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    m_cameraEntity->tilt(math::anglef::pi);

    OrbitCameraController* camController = new OrbitCameraController(m_rootEntity);
    camController->setCamera(m_cameraEntity);

    Qt3DCore::QEntity* lightEntity = new Qt3DCore::QEntity(m_rootEntity);
    Qt3DRender::QPointLight* light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform* lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(0, 0, 20.f));
    lightEntity->addComponent(lightTransform);
    //m_cameraEntity->addComponent(lightEntity);

    // uav shape data
    Qt3DRender::QMesh* uavMesh = new Qt3DRender::QMesh();
    //uavMesh->setSource(QUrl("meshes/xxx.obj"));
    uavMesh->setSource(QUrl::fromLocalFile("meshes/quad.obj"));


    // Sphere mesh transform
    Qt3DCore::QTransform* uavTransform = new Qt3DCore::QTransform();

    //uavTransform->setScale(0.001f);
    uavTransform->setTranslation(QVector3D(-5.0f, 0.0f, 0.0f));

    Qt3DExtras::QPhongMaterial* uavMaterial = new Qt3DExtras::QPhongMaterial();
    uavMaterial->setDiffuse(QColor(QRgb(0xa69929)));

    // uav
    m_uavEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_uavEntity->addComponent(uavMesh);
    m_uavEntity->addComponent(uavMaterial);
    m_uavEntity->addComponent(uavTransform);

    {
        // plane shape data
        Qt3DRender::QMesh* planeMesh = new Qt3DRender::QMesh();
        planeMesh->setSource(QUrl::fromLocalFile("meshes/plane.obj"));

        // Sphere mesh transform
        Qt3DCore::QTransform* planeTransform = new Qt3DCore::QTransform();

        //planeTransform->setScale(0.001f);
        planeTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

        Qt3DExtras::QDiffuseMapMaterial* planeMaterial = new Qt3DExtras::QDiffuseMapMaterial();
        planeMaterial->setAmbient(QColor(QRgb(0xFFFFFF)));

        Qt3DRender::QTextureImage* textureImage = new Qt3DRender::QTextureImage();
        textureImage->setSource(QUrl::fromLocalFile("textures/checkerboard.png"));

        Qt3DRender::QTexture2D* texture = new Qt3DRender::QTexture2D();
        texture->setGenerateMipMaps(true);
        texture->addTextureImage(textureImage);
        texture->setWrapMode(Qt3DRender::QTextureWrapMode(Qt3DRender::QTextureWrapMode::Repeat));
        texture->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
        texture->setMagnificationFilter(Qt3DRender::QTexture2D::Nearest);
        texture->setMaximumAnisotropy(16.f);

        planeMaterial->setDiffuse(texture);

        // plane
        m_groundEntity = new Qt3DCore::QEntity(m_rootEntity);
        m_groundEntity->addComponent(planeMesh);
        m_groundEntity->addComponent(planeMaterial);
        m_groundEntity->addComponent(planeTransform);
    }

    //Qt3DLogic::QFrameAction* frameAction = new Qt3DLogic::QFrameAction();
    //QObject::connect(m_frameAction, SIGNAL(triggered(float)), this, SLOT(_q_onTriggered(float)));
}

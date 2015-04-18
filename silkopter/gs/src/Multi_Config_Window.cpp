#include "Multi_Config_Window.h"

#include "ModelTest.h"
#include "json_editor/JSON_Model.h"

#include "sz_math.hpp"
#include "sz_Multi_Config.hpp"

#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>

Multi_Config_Window::Multi_Config_Window(silk::HAL& hal, silk::Comms& comms, QWidget *parent)
    : QMainWindow(parent)
    , m_hal(hal)
    , m_comms(comms)
{
    m_ui.setupUi(this);

    m_scene = new QGraphicsScene();

    setWindowTitle("Multi Editor");
    setMouseTracking(true);

    m_view = new QGraphicsView(this);
    m_view->setScene(m_scene);
    m_view->setCacheMode(QGraphicsView::CacheNone);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_view->setRenderHints(QPainter::RenderHints({QPainter::Antialiasing, QPainter::TextAntialiasing, QPainter::SmoothPixmapTransform, QPainter::HighQualityAntialiasing}));
    m_view->setMouseTracking(true);
//    m_view->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

    setCentralWidget(m_view);

    m_config_dock = new QDockWidget(this);

    on_config_refreshed();
    m_hal.multi_config_refreshed_signal.connect([this]() { on_config_refreshed(); });

    QObject::connect(m_ui.action_new_config, &QAction::triggered, [this](bool)
    {
        if (m_config_view)
        {
            auto res = QMessageBox::question(this, "Confirm", "Creating a new config will delete the current one.\nAre you sure?");
            if (res == QMessageBox::No)
            {
                return;
            }
        }
        m_config = silk::config::Multi();
        show_config();
    });
    QObject::connect(m_ui.action_add_motor, &QAction::triggered, [this](bool)
    {
        if (m_config.motors.empty())
        {
            m_config.motors.push_back(silk::config::Multi::Motor());
        }
        else
        {
            m_config.motors.push_back(m_config.motors.back());
        }
        show_config();
    });
    QObject::connect(m_ui.action_apply_config, &QAction::triggered, [this](bool)
    {
        m_hal.set_multi_config(m_config);
    });

    addDockWidget(Qt::RightDockWidgetArea, m_config_dock);
}

Multi_Config_Window::~Multi_Config_Window()
{
}

void Multi_Config_Window::on_config_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    m_config = silk::config::Multi();
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(m_config, m_config_json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Req Id: {} - Cannot deserialize multi config: {}", ss.str());
    }
}

void Multi_Config_Window::on_config_refreshed()
{
    auto config = m_hal.get_multi_config();
    if (config)
    {
        m_config = *config;
        show_config();
    }
    else
    {
        m_ui.action_add_motor->setEnabled(false);
        m_ui.action_apply_config->setEnabled(false);
        delete m_config_view;
        m_config_view = nullptr;
        delete m_config_model;
        m_config_model = nullptr;

        auto* label = new QLabel(m_config_dock);
        label->setText("No Config");
        QFont font;
        font.setPointSize(32);
        label->setFont(font);
        label->setEnabled(false);
        label->setAlignment(Qt::AlignCenter);

        m_config_dock->setWidget(label);
    }
}

void Multi_Config_Window::show_config()
{
    if (!m_config_view)
    {
        m_ui.action_add_motor->setEnabled(true);
        m_ui.action_apply_config->setEnabled(true);
        m_config_view = new QTreeView(m_config_dock);
        m_config_model = new JSON_Model(m_config_view);
        connect(m_config_model, &QAbstractItemModel::dataChanged, this, &Multi_Config_Window::on_config_changed);
        m_config_view->setModel(m_config_model);
        m_config_dock->setWidget(m_config_view);
    }

    autojsoncxx::to_document(m_config, m_config_json);
    m_config_model->set_document("Multi Config", &m_config_json);
    m_config_view->expandAll();
}

void Multi_Config_Window::process()
{
}

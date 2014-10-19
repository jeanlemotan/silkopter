//#include <QtWidgets>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <numeric>

#include "boost/asio.hpp"
#include "boost/noncopyable.hpp"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include <QtCore>
#include <QtWidgets/QMessageBox>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtCore/QAbstractItemModel>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtCore/QString>
#include <QtCore/QtGlobal>
#include <QtCore/QSettings>
#include <QtWidgets/QShortcut>

#include "_qmath.h"
#include "QBase.h"
#include "QData.h"
#include "QInput.h"
#include "q.h"

#include "utils/Channel.h"

#define SILK_DBG(fmt, ...)  QLOG_DBG("brain", fmt, ##__VA_ARGS__)
#define SILK_INFO(fmt, ...)  QLOG_INFO("brain", fmt, ##__VA_ARGS__)
#define SILK_WARNING(fmt, ...)  QLOG_WARNING("brain", fmt, ##__VA_ARGS__)
#define SILK_ERR(fmt, ...)  QLOG_ERR("brain", fmt, ##__VA_ARGS__)


inline math::vec3f bt_to_vec3f(btVector3 const& v)
{
    return math::vec3f(v.x(), v.y(), v.z());
}
inline math::quatf bt_to_quatf(btQuaternion const& v)
{
    return math::quatf(v.x(), v.y(), v.z(), v.w());
}
inline btVector3 vec3f_to_bt(math::vec3f const& v)
{
    return btVector3(v.x, v.y, v.z);
}
inline btQuaternion quatf_to_bt(math::quatf const& v)
{
    return btQuaternion(v.x, v.y, v.z, v.w);
}

struct Render_Context
{
    q::scene::Camera camera;
    q::scene::Scene scene;
    q::draw::Painter painter;
    q::text::Font_ptr font;
    struct Materials
    {
        q::video::Material primitive_2d;
        q::video::Material primitive;
        q::video::Material textured;
        q::video::Material font;
    } materials;
};

struct Mouse_Event
{
    enum class Button: uint8_t
    {
        LEFT	= 1 << 0,
        MIDDLE	= 1 << 1,
        RIGHT	= 1 << 2,
    };
    typedef q::util::Flag_Set<Button, uint8_t> Buttons;

    math::vec2f point;
    float wheel = 0;
    Buttons buttons;
};


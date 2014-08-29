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

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


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


//#include <QtWidgets>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <deque>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread.hpp>

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
#include <QFileDialog>

#include <mapcontrol.h>
#include <osmmapadapter.h>
#include <maplayer.h>

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

extern "C"
{
#include <fftw3.h>
}

#include "_qmath.h"
#include "QBase.h"
#include "QData.h"
#include "QInput.h"
#include "q.h"

#include "utils/Channel.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"

namespace std
{
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}

extern boost::asio::io_service s_async_io_service;

namespace silk
{
    template<typename F> auto async(F f) -> boost::unique_future<decltype(f())>
    {
        typedef decltype(f()) result_type;
        typedef boost::packaged_task<result_type> packaged_task;
        auto task = std::make_shared<packaged_task>(std::move(f));
        boost::unique_future<result_type> future = task->get_future();
        s_async_io_service.post(std::bind(&packaged_task::operator(), task));
        return future;
    }
}

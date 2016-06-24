#pragma once

#if defined __cplusplus

#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <deque>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
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

extern "C"
{
#include <fftw3.h>
}

#include "_qmath.h"
#include "QBase.h"
#include "QData.h"

#include "def_lang/Type_System.h"
#include "def_lang/IStruct_Value.h"
#include "def_lang/IStruct_Type.h"
#include "def_lang/IPoly_Value.h"
#include "def_lang/IPoly_Type.h"
#include "def_lang/IEnum_Value.h"
#include "def_lang/IEnum_Type.h"
#include "def_lang/IEnum_Item.h"
#include "def_lang/All_INumeric_Types.h"
#include "def_lang/All_INumeric_Values.h"
#include "def_lang/IString_Value.h"
#include "def_lang/IString_Type.h"
#include "def_lang/IBool_Value.h"
#include "def_lang/IBool_Type.h"
#include "def_lang/IOptional_Value.h"
#include "def_lang/IOptional_Type.h"
#include "def_lang/IVariant_Value.h"
#include "def_lang/IVariant_Type.h"

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


#endif

#include "Stream_Viewer_Window.h"
#include "Comms.h"

#include "stream_viewers/Acceleration_Stream_Viewer_Widget.h"

Stream_Viewer_Window::Stream_Viewer_Window(QWidget* parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

void Stream_Viewer_Window::init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type)
{
    m_comms = &comms;
    m_stream_path = stream_path;
    m_stream_rate = stream_rate;
    m_stream_type = stream_type;

    setWindowTitle(("Stream - " + m_stream_path).c_str());

    IStream_Viewer_Widget* viewer = nullptr;
    switch (m_stream_type.get_semantic())
    {
    case silk::stream::Semantic::ACCELERATION:
        viewer = new Acceleration_Stream_Viewer_Widget(this);
        break;
    }

    if (viewer)
    {
        viewer->init(comms, stream_path, stream_rate, stream_type);
        layout()->addWidget(viewer);
    }
}

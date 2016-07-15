#pragma once

#include "common/stream/IStream.h"
#include "ui_Stream_Viewer_Window.h"


namespace silk
{
class Comms;
}

class IStream_Viewer_Widget;


class Stream_Viewer_Window : public QWidget
{
    Q_OBJECT
public:
    Stream_Viewer_Window(QWidget* parent);

    void init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type);

private:
    IStream_Viewer_Widget* m_widget = nullptr;
    silk::Comms* m_comms = nullptr;
    std::string m_stream_path;
    uint32_t m_stream_rate;
    silk::stream::Type m_stream_type;

    Ui::Stream_Viewer_Window m_ui;
};

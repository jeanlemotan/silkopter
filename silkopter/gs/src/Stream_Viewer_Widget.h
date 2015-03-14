#pragma once

#include "HAL.h"
#include "Comms.h"


class Stream_Viewer_Widget : public QWidget
{
public:
    Stream_Viewer_Widget(silk::HAL& hal, silk::Comms& comms, QWidget *parent);

    void set_stream_name(std::string const& stream_name);

private:
    std::string m_stream_name;
    silk::HAL& m_hal;
    silk::Comms& m_comms;
};

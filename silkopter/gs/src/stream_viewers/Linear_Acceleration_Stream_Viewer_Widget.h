#pragma once

#include "IStream_Viewer_Widget.h"
#include "boost/signals2.hpp"

namespace silk
{
class Comms;
}

class Linear_Acceleration_Stream_Viewer_Widget : public IStream_Viewer_Widget
{
public:
    Linear_Acceleration_Stream_Viewer_Widget(QWidget* parent);
    ~Linear_Acceleration_Stream_Viewer_Widget();

    void init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type) override;

private:
    silk::Comms* m_comms = nullptr;
    std::string m_stream_path;
    uint32_t m_stream_rate;
    silk::stream::Type m_stream_type;

    boost::signals2::scoped_connection m_connection;
};

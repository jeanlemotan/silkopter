#pragma once

#include "common/stream/IStream.h"

namespace silk
{
class Comms;
}

class IStream_Viewer_Widget : public QWidget
{
public:
    virtual void init(silk::Comms& comms, std::string const& stream_path, uint32_t stream_rate, silk::stream::Type stream_type) = 0;
};

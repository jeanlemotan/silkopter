#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IVideo : public IStream
{
    DEFINE_RTTI_CLASS(IVideo, IStream);
public:
    virtual ~IVideo() {}
};


}
}
}

#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ITorque_Stream : public IStream
{
    DEFINE_RTTI_CLASS(ITorque_Stream, IStream);
public:
    typedef float             Value; //Nm
    typedef stream::Sample<Value>     Sample;

    virtual ~ITorque_Stream() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ITorque_Stream);


}
}
}

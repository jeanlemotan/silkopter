#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ICardinal_Points : public IStream
{
    DEFINE_RTTI_CLASS(ICardinal_Points, IStream);
public:
    struct Value
    {
        struct Points
        {
            math::vec3f n;
            math::vec3f s;
            math::vec3f e;
            math::vec3f w;
        } magnetic, geodezic;
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~ICardinal_Points() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ICardinal_Points);


}
}
}


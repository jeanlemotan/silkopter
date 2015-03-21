#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IVideo : public IScalar_Stream<Type::VIDEO>
{
public:
    struct Value
    {
        enum class Type : uint8_t
        {
            H264
        };

        Type type;
        math::vec2u32 resolution;
        std::vector<uint8_t> data;
    };
    typedef stream::Sample<Value>     Sample;
};
DECLARE_CLASS_PTR(IVideo);


}
}
}

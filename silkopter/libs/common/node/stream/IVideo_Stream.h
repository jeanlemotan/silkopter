#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IVideo_Stream : public IScalar_Stream<IStream::Type::VIDEO>
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
    typedef node::Sample<Value>     Sample;
};
DECLARE_CLASS_PTR(IVideo_Stream);


}
}

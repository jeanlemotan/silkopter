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
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IVideo);


}
}
}

#pragma once

#include "Stream_Base.h"

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
        bool is_keyframe = false;
        math::vec2u32 resolution;
        std::vector<uint8_t> data;
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVideo);


}
}
}

#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IReference_Frame : public IStream
{
    DEFINE_RTTI_CLASS(IReference_Frame, IStream);
public:
    struct Reference_Frame
    {
        math::vec3f front_vector_world;
        math::vec3f right_vector_world;
        math::vec3f up_vector_world;

        math::quatf local_to_world;
    };
    typedef stream::Sample<Reference_Frame>     Sample;

    virtual ~IReference_Frame() {}

    virtual auto get_type() const -> q::rtti::class_id { return q::rtti::get_class_id<IReference_Frame>(); }

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
}


#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IProximity : public IScalar_Stream<Type::PROXIMITY>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        std::vector<math::vec3d> distances;
    };

    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IProximity);

}
}


namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IProximity::Value const& value, size_t& off)
{
    serialize(buffer, value.distances, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IProximity::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.distances, off);
}


}
}


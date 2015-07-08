#pragma once

#include "IStream.h"
//#include "autojsoncxx/boost_types.hpp"
#include "sz_math.hpp"
#include "utils/Json_Util.h"

namespace silk
{
namespace node
{
namespace stream
{

//for streams of data with a fixed sample rate
template <Type TYPE_VALUE>
class IScalar_Stream : public IStream
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};

//for vectorial streams of data with a fixed sample rate
template <Type TYPE_VALUE, Space SPACE_VALUE>
class ISpatial_Stream : public IStream
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    static constexpr Space SPACE = SPACE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};


}
}
}

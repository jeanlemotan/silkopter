#pragma once

#include "IStream.h"

namespace silk
{
namespace stream
{

//for streams of data with a fixed sample rate
template <Semantic SEMANTIC_VALUE>
class IScalar_Stream : public IStream
{
public:
    static constexpr Space SPACE = Space::SCALAR;
    static constexpr Semantic SEMANTIC = SEMANTIC_VALUE;
    static constexpr Type TYPE = Type(SEMANTIC, SPACE);
    virtual auto get_type() const -> Type override { return TYPE; }
};

template <Semantic SEMANTIC_VALUE>
constexpr Type IScalar_Stream<SEMANTIC_VALUE>::TYPE;



//for vectorial streams of data with a fixed sample rate
template <Semantic SEMANTIC_VALUE, Space SPACE_VALUE>
class ISpatial_Stream : public IStream
{
public:
    static constexpr Semantic SEMANTIC = SEMANTIC_VALUE;
    static constexpr Space SPACE = SPACE_VALUE;
    static constexpr Type TYPE = Type(SEMANTIC, SPACE);
    virtual auto get_type() const -> Type override { return TYPE; }
};

template <Semantic SEMANTIC_VALUE, Space SPACE_VALUE>
constexpr Type ISpatial_Stream<SEMANTIC_VALUE, SPACE_VALUE>::TYPE;


}
}

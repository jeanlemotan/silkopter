#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IBool : public IScalar_Stream<Semantic::BOOL>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef bool                    Value;
    typedef stream::Sample<Value>   Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}

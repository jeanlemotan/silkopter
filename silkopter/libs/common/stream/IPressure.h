#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IPressure : public IScalar_Stream<Semantic::PRESSURE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef double                   Value; //kilo Pascals. 1 Pascal == 0.01 millibars, 100000 Pa == 1000 mbar == 1 bar ~= sea level
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}

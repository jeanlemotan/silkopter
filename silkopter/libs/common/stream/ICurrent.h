#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class ICurrent : public IScalar_Stream<Semantic::CURRENT>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef float             Value; //amperes
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}

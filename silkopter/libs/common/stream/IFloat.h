#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IFloat : public IScalar_Stream<Type::FLOAT>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef double                       Value;
    typedef stream::Sample<Value>       Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IFloat);


}
}

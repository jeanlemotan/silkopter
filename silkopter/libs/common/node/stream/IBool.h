#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IBool : public IScalar_Stream<Type::BOOL>
{
public:
    typedef bool                    Value;
    typedef stream::Sample<Value>   Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IBool);


}
}
}

#pragma once

#include "IStream.h"
#include "Sample.h"

namespace silk
{
namespace node
{
namespace stream
{

class ICurrent : public IStream
{
public:
    typedef float             Value; //amperes
    typedef Sample<Value>     Sample;

    virtual ~ICurrent() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(ICurrent);

}
}
}

#pragma once

#include "ITransform.h"

namespace silk
{
namespace node
{
namespace processor
{

template<class Stream>
class IFilter : public ITransform<Stream, Stream>
{
public:
};



}
}
}

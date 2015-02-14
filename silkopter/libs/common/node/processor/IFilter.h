#pragma once

#include "IProcessor.h"

namespace silk
{
namespace node
{
namespace processor
{

template<class Stream>
class IFilter : public IProcessor
{
public:
    virtual auto get_input_stream() -> Stream& = 0;
    virtual auto get_output_stream() -> Stream& = 0;
};



}
}
}

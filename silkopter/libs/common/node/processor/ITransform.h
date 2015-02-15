#pragma once

#include "IProcessor.h"

namespace silk
{
namespace node
{
namespace processor
{

template<class In, class Out>
class ITransform : public IProcessor
{
public:
    virtual auto get_input_stream(size_t idx) -> In& = 0;
    virtual auto get_output_stream(size_t idx) -> Out& = 0;
};



}
}
}

#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISource : public Node_Base<Type::SOURCE>
{
public:
    //sources don't have any inputs
    virtual void set_input_stream_path(size_t, q::Path const&) {}
    virtual auto get_inputs() const -> std::vector<Input> final { return std::vector<Input>(); }
};


}
}


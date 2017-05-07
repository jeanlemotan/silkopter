#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISource : public Node_Base<Type::SOURCE>
{
public:
    //sources don't usually have any inputs
    virtual ts::Result<void> set_input_stream_path(size_t, std::string const&) { return ts::success; }
    virtual std::vector<Input> get_inputs() const { return std::vector<Input>(); }
};


}
}


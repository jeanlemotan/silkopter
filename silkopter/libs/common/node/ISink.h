#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISink : public Node_Base<Type::SINK>
{
public:
    //sinks don't have any outputs
    virtual auto get_stream_outputs() const -> std::vector<Stream_Output> final { return std::vector<Stream_Output>(); }
};
DECLARE_CLASS_PTR(ISink);


}
}


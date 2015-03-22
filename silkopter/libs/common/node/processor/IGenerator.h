#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IGenerator : public INode_Base<Type::GENERATOR>
{
public:
    //generators don't have any inputs
    virtual auto get_inputs() const -> std::vector<Input> final { return std::vector<Input>(); }
};
DECLARE_CLASS_PTR(IGenerator);


}
}


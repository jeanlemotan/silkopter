#pragma once

#include "IParam.h"

namespace silk
{
namespace node
{
namespace param
{

class IFloat : public Param_Base<Type::FLOAT>
{
public:
    typedef param::Value<float>     Value;
    virtual auto get_value() const -> Value const& = 0;
};
DECLARE_CLASS_PTR(IFloat);


}
}
}

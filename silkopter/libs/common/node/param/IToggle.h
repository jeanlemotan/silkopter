#pragma once

#include "IParam.h"

namespace silk
{
namespace node
{
namespace param
{

class IToggle : public Param_Base<Type::TOGGLE>
{
public:
    typedef param::Value<bool>     Value;
    virtual auto get_value() const -> Value const& = 0;
};
DECLARE_CLASS_PTR(IToggle);


}
}
}

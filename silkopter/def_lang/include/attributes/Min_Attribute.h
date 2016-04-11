#pragma once

#include "IAttribute.h"
#include "Result.h"
#include <memory>

namespace ts
{

class IValue;
class IInitializer;

class Min_Attribute : public virtual IAttribute
{
public:
    ~Min_Attribute();

    auto get_name() const -> std::string;
    auto init(IInitializer const& initializer) -> Result<void>;
    auto get_min_value() const -> IValue const&;

private:
    std::unique_ptr<IValue> m_value;
};

}

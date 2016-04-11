#pragma once

#include "IAttribute.h"
#include <memory>

namespace ts
{

class IValue;

class Min_Attribute : public virtual IAttribute
{
public:
    Min_Attribute(std::unique_ptr<IValue> value);
    ~Min_Attribute();

    auto get_name() const -> std::string override;
    auto get_min_value() const -> IValue const&;

private:
    std::unique_ptr<IValue> m_value;
};

}

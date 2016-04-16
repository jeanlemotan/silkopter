#pragma once

#include "IAttribute.h"
#include <memory>

namespace ts
{

class IValue;

class Max_Attribute : public virtual IAttribute
{
public:
    Max_Attribute(std::unique_ptr<IValue> value);
    ~Max_Attribute();

    auto get_name() const -> std::string override;
    auto get_max_value() const -> IValue const&;

private:
    std::unique_ptr<IValue> m_value;
};

}

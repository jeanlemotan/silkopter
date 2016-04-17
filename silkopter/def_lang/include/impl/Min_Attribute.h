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

    std::string get_name() const override;
    IValue const& get_min_value() const;

private:
    std::unique_ptr<IValue> m_value;
};

}

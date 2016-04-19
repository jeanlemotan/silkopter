#pragma once

#include "IAttribute.h"
#include <memory>

namespace ts
{

class IValue;

class Max_Attribute : public virtual IAttribute
{
public:
    Max_Attribute(std::shared_ptr<IValue> value);
    ~Max_Attribute();

    std::string get_name() const override;
    IValue const& get_max_value() const;

private:
    std::shared_ptr<IValue> m_value;
};

}

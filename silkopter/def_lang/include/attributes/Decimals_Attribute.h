#pragma once

#include "IAttribute.h"

namespace ts
{

class Decimals_Attribute : public virtual IAttribute
{
public:
    Decimals_Attribute(size_t decimals);
    ~Decimals_Attribute();

    auto get_name() const -> std::string override;
    auto get_decimals() const -> size_t;

private:
    size_t m_decimals = 0;
};

}

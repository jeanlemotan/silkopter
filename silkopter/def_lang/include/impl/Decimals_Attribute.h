#pragma once

#include "IAttribute.h"

namespace ts
{

class Decimals_Attribute : public virtual IAttribute
{
public:
    Decimals_Attribute(size_t decimals);
    ~Decimals_Attribute();

    std::string get_name() const override;
    size_t get_decimals() const;

private:
    size_t m_decimals = 0;
};

}

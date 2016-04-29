#pragma once

#include "def_lang/IAttribute.h"

namespace ts
{

class Default_Attribute : public virtual IAttribute
{
public:
    Default_Attribute();
    ~Default_Attribute();

    std::string get_name() const override;

private:
};

}

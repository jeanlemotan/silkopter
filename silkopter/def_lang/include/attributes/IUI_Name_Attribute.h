#pragma once

#include "IAttribute.h"

namespace ts
{

class IUI_Name_Attribute : public virtual IAttribute
{
public:
    virtual auto get_ui_name() const -> std::string const& = 0;
};

}

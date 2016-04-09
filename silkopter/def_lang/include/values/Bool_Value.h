#pragma once

#include "IBool_Value.h"

namespace ts
{

class Bool_Value final : public IBool_Value
{
public:
    auto get_template_instantiation_string() const -> std::string override;

};

}

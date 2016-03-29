#pragma once

#include "types/IType.h"

namespace ts
{

class IValue
{
public:
    virtual ~IValue() = default;

    virtual auto is_equal(IValue const& other) const -> bool = 0;
    virtual auto assign_from(IValue const& other) -> bool = 0;

    virtual auto clone() const -> std::shared_ptr<IValue> = 0;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;

    virtual auto parse_from_string(std::string const& str) = 0;
    virtual auto get_ui_string() const -> std::string = 0;
};

}

#pragma once

#include <memory>
#include "Result.h"
#include "IInitializer.h"

namespace ts
{

class IType;
class Value_Selector;

class IValue
{
public:
    virtual ~IValue() = default;

    virtual Result<bool> is_equal(IValue const& other) const = 0;

    virtual Result<void> copy_assign(IValue const& other) = 0;
    virtual Result<void> copy_assign(IInitializer const& initializer) = 0;

    virtual std::unique_ptr<IValue> clone() const = 0;

    virtual std::shared_ptr<IType const> get_type() const = 0;

    virtual Result<void> parse_from_ui_string(std::string const& str) = 0;
    virtual Result<std::string> get_ui_string() const = 0;

    virtual IValue const* select(Value_Selector const& selector) const = 0;
    virtual IValue* select(Value_Selector const& selector) = 0;
};

}

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

    virtual auto is_default_constructible() -> bool = 0;
    virtual auto default_construct() -> Result<void> = 0;

    virtual auto is_constructible() -> bool = 0;
    virtual auto construct(IInitializer const& initializer) -> Result<void> = 0;

    virtual auto is_copy_constructible() -> bool = 0;
    virtual auto copy_contruct(IValue const& other) -> Result<void> = 0;

    virtual auto is_equal(IValue const& other) const -> Result<bool> = 0;

    virtual auto copy_assign(IValue const& other) -> Result<void> = 0;

    virtual auto clone() const -> std::unique_ptr<IValue> = 0;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;

    virtual auto parse_from_ui_string(std::string const& str) -> Result<void> = 0;
    virtual auto get_ui_string() const -> std::string = 0;

    virtual auto select(Value_Selector const& selector) const -> std::shared_ptr<const IValue> = 0;
    virtual auto select(Value_Selector const& selector) -> std::shared_ptr<IValue> = 0;
};

}

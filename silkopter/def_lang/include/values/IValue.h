#pragma once

namespace ts
{

class IType;
class Value_Selector;

class IValue
{
public:
    virtual ~IValue() = default;

    virtual auto is_equal(IValue const& other) const -> bool = 0;
    virtual auto assign_from(IValue const& other) -> bool = 0;

    virtual auto clone() const -> std::shared_ptr<IValue> = 0;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;

    virtual auto parse_from_ui_string(std::string const& str) -> bool = 0;
    virtual auto get_ui_string() const -> std::string = 0;

    virtual auto select(Value_Selector const& selector) const -> std::shared_ptr<const IValue> = 0;
    virtual auto select(Value_Selector const& selector) -> std::shared_ptr<IValue> = 0;
};

}

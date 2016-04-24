#pragma once

#include <memory>
#include "Result.h"
#include "IInitializer.h"
#include "Serialization.h"

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

    virtual std::shared_ptr<IValue> clone() const = 0;

    virtual std::shared_ptr<IType const> get_type() const = 0;

    virtual Result<void> parse_from_ui_string(std::string const& str) = 0;
    virtual Result<std::string> get_ui_string() const = 0;

    virtual std::shared_ptr<const IValue> select(Value_Selector&& selector) const = 0;
    virtual std::shared_ptr<IValue> select(Value_Selector&& selector) = 0;

    virtual Result<serialization::Value> serialize() const = 0;
    virtual Result<void> deserialize(serialization::Value const&) = 0;

    template<typename T>
    std::shared_ptr<const T> select_specialized(Value_Selector&& selector) const;

    template<typename T>
    std::shared_ptr<T> select_specialized(Value_Selector&& selector);
};

}

#include "impl/IValue.inl"

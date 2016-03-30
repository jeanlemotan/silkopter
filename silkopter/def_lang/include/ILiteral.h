#pragma once

namespace ts
{

class IType;
class IValue;

class ILiteral
{
public:
    virtual ~ILiteral() = default;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;
    virtual auto get_value() const -> std::shared_ptr<const IValue> = 0;
};

}

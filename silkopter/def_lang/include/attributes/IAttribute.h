#pragma once

namespace ts
{

class IAttribute
{
public:
    virtual ~IAttribute() = default;

    virtual auto get_name() const -> std::string const& = 0;
};

}

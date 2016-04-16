#pragma once

#include <memory>
#include <vector>
#include "IInitializer_List.h"

namespace ts
{

class Initializer_List final : public virtual IInitializer_List
{
public:
    Initializer_List(std::vector<std::unique_ptr<IInitializer>> initializers);

    auto get_initializer_count() const -> size_t override;
    auto get_initializer(size_t idx) const -> IInitializer const& override;
    auto get_initializer_value() const -> IValue const* override;

private:
    std::vector<std::unique_ptr<IInitializer>> m_initializers;
};

}

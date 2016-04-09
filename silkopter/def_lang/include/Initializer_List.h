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
    auto get_initializer(size_t idx) const -> std::shared_ptr<const IInitializer> override;

private:
    std::vector<std::shared_ptr<IInitializer>> m_initializers;
};

}

#pragma once

#include <memory>
#include <vector>
#include "IInitializer_List.h"

namespace ts
{

class Initializer_List final : public virtual IInitializer_List
{
public:
    Initializer_List(std::vector<std::shared_ptr<IInitializer>> initializers);

    size_t get_initializer_count() const override;
    IInitializer const& get_initializer(size_t idx) const override;
    IValue const* get_initializer_value() const override;

private:
    std::vector<std::shared_ptr<IInitializer>> m_initializers;
};

}

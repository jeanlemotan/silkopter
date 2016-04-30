#pragma once

#include <memory>
#include <vector>
#include "def_lang/IInitializer_List.h"

namespace ts
{

class Initializer_List final : public virtual IInitializer_List
{
public:
    Initializer_List(std::vector<std::shared_ptr<const IInitializer>> initializers);

    size_t get_initializer_count() const override;
    std::shared_ptr<const IInitializer> get_initializer(size_t idx) const override;

private:
    std::vector<std::shared_ptr<const IInitializer>> m_initializers;
};

}

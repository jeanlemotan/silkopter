#pragma once

#include <vector>
#include "IValue_Container.h"

namespace ts
{

class Value_Container_EP : virtual public IValue_Container
{
public:

    ~Value_Container_EP();

    auto insert_value(size_t idx, std::unique_ptr<IValue>&& value) -> Result<void> override;
    auto erase_value(size_t idx) -> Result<void> override;

    auto get_value_count() const -> size_t override;

    auto get_value(size_t idx) const -> IValue const& override;
    auto get_value(size_t idx) -> IValue& override;

private:
    std::vector<std::shared_ptr<IValue>> m_values;
};

}

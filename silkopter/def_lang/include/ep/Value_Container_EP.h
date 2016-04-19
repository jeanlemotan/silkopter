#pragma once

#include <vector>
#include "IValue_Container.h"

namespace ts
{

class Value_Container_EP : virtual public IValue_Container
{
public:

    ~Value_Container_EP();

    Result<void> insert_value(size_t idx, std::shared_ptr<IValue> value) override;
    Result<void> erase_value(size_t idx) override;

    size_t get_value_count() const override;

    IValue const& get_value(size_t idx) const override;
    IValue& get_value(size_t idx) override;

private:
    std::vector<std::shared_ptr<IValue>> m_values;
};

}

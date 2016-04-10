#pragma once

#include <memory>
#include "IInitializer.h"

namespace ts
{

class IInitializer_List : public virtual IInitializer
{
public:
    virtual ~IInitializer_List() = default;

    virtual auto get_initializer_count() const -> size_t = 0;
    virtual auto get_initializer(size_t idx) const -> IInitializer const& = 0;
};

}

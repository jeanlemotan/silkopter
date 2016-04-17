#pragma once

#include <memory>
#include "IInitializer.h"

namespace ts
{

class IInitializer_List : public virtual IInitializer
{
public:
    virtual ~IInitializer_List() = default;

    virtual size_t get_initializer_count() const = 0;
    virtual IInitializer const& get_initializer(size_t idx) const = 0;
};

}

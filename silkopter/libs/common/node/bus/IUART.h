#pragma once

#include "IBus.h"

namespace silk
{
namespace node
{
namespace bus
{

class IUART: public IBus
{
    DEFINE_RTTI_CLASS(IUART, IBus);
public:
    virtual ~IUART() {}

    virtual void lock() = 0;
    virtual auto try_lock() -> bool = 0;
    virtual void unlock() = 0;

    virtual auto read(uint8_t* data, size_t max_size) -> size_t = 0;
    virtual auto write(uint8_t const* data, size_t size) -> bool = 0;
};
DECLARE_CLASS_PTR(IUART);


}
}
}

#pragma once

namespace silk
{
namespace bus
{

class IUART: public q::util::Noncopyable
{
public:
    virtual ~IUART() {}

    virtual auto get_name() const -> q::String const& = 0;

    virtual void lock() = 0;
    virtual auto try_lock() -> bool = 0;
    virtual void unlock() = 0;

    virtual auto read(uint8_t* data, size_t max_size) -> size_t = 0;
    virtual auto write(uint8_t const* data, size_t size) -> bool = 0;
};

}
}


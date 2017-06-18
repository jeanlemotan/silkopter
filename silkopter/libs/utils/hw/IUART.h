#pragma once

namespace util
{
namespace hw
{

class IUART
{
public:
    virtual ~IUART() = default;

    virtual size_t read(uint8_t* data, size_t max_size) = 0;
    virtual bool write(uint8_t const* data, size_t size) = 0;

    virtual void send_break() = 0;
};


}
}

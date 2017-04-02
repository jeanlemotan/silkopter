#pragma once

#include "IUART.h"
#include <string>
#include <vector>
#include <mutex>

namespace util
{
namespace hw
{

class UART_Dev : public IUART
{
public:
    UART_Dev();
    ~UART_Dev();

    enum class Baud
    {
        _9600,
        _19200,
        _38400,
        _57600,
        _115200,
        _230400,
    };

    ts::Result<void> init(std::string const& device, Baud baud);

    void lock() override;
    bool try_lock() override;
    void unlock() override;

    size_t read(uint8_t* data, size_t max_size) override;
    bool write(uint8_t const* data, size_t size) override;

    void send_break() override;

private:
    void close();

    std::string m_device;
    int m_fd = -1;

    static std::recursive_mutex s_mutex;
    std::vector<uint8_t> m_buffer;
};

}
}

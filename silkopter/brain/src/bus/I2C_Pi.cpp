#include "BrainStdAfx.h"
#include "bus/I2C_Pi.h"

#include <errno.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


struct i2c_msg
{
  __u16 addr;
  __u16 flags;
#define I2C_M_TEN		0x0010
#define I2C_M_RD		0x0001
#define I2C_M_NOSTART		0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
#define I2C_M_RECV_LEN		0x0400
  __u16 len;
  __u8 * buf;
};


namespace silk
{
namespace bus
{

I2C_Pi::I2C_Pi(q::String const& name)
    : m_name(name)
{
}

I2C_Pi::~I2C_Pi()
{
    close();
}

auto I2C_Pi::get_name() const -> q::String const&
{
    return m_name;
}

auto I2C_Pi::open(q::String const& device) -> bool
{
    close();

    QLOG_TOPIC("bus_i2c_pi");

    std::lock_guard<I2C_Pi> lg(*this);

    m_device = device;
    m_fd = ::open(device.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", device, strerror(errno));
        return false;
    }
    return true;
}
void I2C_Pi::close()
{
    QLOG_TOPIC("bus_i2c_pi");

    if (m_fd)
    {
        std::lock_guard<I2C_Pi> lg(*this);

        ::close(m_fd);
        m_fd = -1;
    }
}

void I2C_Pi::lock()
{
    m_mutex.lock();
}

auto I2C_Pi::try_lock() -> bool
{
    return m_mutex.try_lock();
}
void I2C_Pi::unlock()
{
    m_mutex.unlock();
}

auto I2C_Pi::read(uint8_t address, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_i2c_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Pi> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    struct i2c_msg msg[1];

    msg[0].addr = address;
    msg[0].flags = I2C_M_RD;
    msg[0].len = size;
    msg[0].buf = data;

    io.msgs = msg;
    io.nmsgs = 1;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("read failed: {}", strerror(errno));
        return false;
    }
    return true;
}
auto I2C_Pi::write(uint8_t address, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_i2c_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Pi> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    struct i2c_msg msg;

    msg.addr = address;
    msg.flags = 0;
    msg.len = size;
    msg.buf = const_cast<uint8_t*>(data);
    io.msgs = &msg;
    io.nmsgs = 1;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("write failed: {}", strerror(errno));
        return false;
    }
    return true;
}

auto I2C_Pi::read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_i2c_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Pi> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    struct i2c_msg msg[2];

    msg[0].addr = address;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &reg;

    msg[1].addr = address;
    msg[1].flags = I2C_M_RD;
    msg[1].len = size;
    msg[1].buf = data;

    io.msgs = msg;
    io.nmsgs = 2;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("read register failed: {}", reg, strerror(errno));
        return false;
    }
    return true;
}
auto I2C_Pi::write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_i2c_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Pi> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    struct i2c_msg msg;

    m_buffer.resize(size + 1);

    m_buffer[0] = reg;
    if (data)
    {
        std::copy(data, data + size, m_buffer.begin() + 1);
    }

    msg.addr = address;
    msg.flags = 0;
    msg.len = size + 1;
    msg.buf = m_buffer.data();
    io.msgs = &msg;
    io.nmsgs = 1;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("write register {} failed: {}", reg, strerror(errno));
        return false;
    }
    return true;
}


}
}

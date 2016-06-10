#include "BrainStdAfx.h"
#include "bus/I2C_Linux.h"

#include <errno.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "uav.def.h"


struct i2c_msg
{
  uint16_t addr = 0;
  uint16_t flags = 0;
#define I2C_M_TEN		0x0010
#define I2C_M_RD		0x0001
#define I2C_M_NOSTART		0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
#define I2C_M_RECV_LEN		0x0400
  uint16_t len = 0;
  uint8_t* buf = nullptr;
};


namespace silk
{
namespace bus
{

I2C_Linux::I2C_Linux()
    : m_descriptor(new I2C_Linux_Descriptor())
{
}

I2C_Linux::~I2C_Linux()
{
    close();
}

bool I2C_Linux::init(std::shared_ptr<IBus_Descriptor> descriptor)
{
    auto specialized = std::dynamic_pointer_cast<I2C_Linux_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    if (!init(specialized->get_dev()))
    {
        return false;
    }

    *m_descriptor = *specialized;
    return true;
}

std::shared_ptr<IBus_Descriptor> I2C_Linux::get_descriptor() const
{
    return m_descriptor;
}

bool I2C_Linux::init(std::string const& dev)
{
    close();

    std::lock_guard<I2C_Linux> lg(*this);

    m_fd = ::open(dev.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", dev, strerror(errno));
        return false;
    }

    return true;
}

void I2C_Linux::close()
{
    QLOG_TOPIC("i2c_linux::close");

    if (m_fd >= 0)
    {
        std::lock_guard<I2C_Linux> lg(*this);

        ::close(m_fd);
        m_fd = -1;
    }
}

void I2C_Linux::lock()
{
    m_mutex.lock();
}

auto I2C_Linux::try_lock() -> bool
{
    return m_mutex.try_lock();
}
void I2C_Linux::unlock()
{
    m_mutex.unlock();
}

auto I2C_Linux::read(uint8_t address, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_linux::read");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Linux> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    memset(&io, 0, sizeof(i2c_rdwr_ioctl_data));

    struct i2c_msg msg;
    memset(&msg, 0, sizeof(i2c_msg));

    msg.addr = address;
    msg.flags = I2C_M_RD;
    msg.len = size;
    msg.buf = reinterpret_cast<decltype(i2c_msg::buf)>(data);

    io.msgs = &msg;
    io.nmsgs = 1;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("read failed: {}", strerror(errno));
        return false;
    }
    return true;
}
auto I2C_Linux::write(uint8_t address, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_linux::write");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Linux> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    memset(&io, 0, sizeof(i2c_rdwr_ioctl_data));

    struct i2c_msg msg;
    memset(&msg, 0, sizeof(i2c_msg));

    msg.addr = address;
    msg.flags = 0;
    msg.len = size;
    msg.buf = decltype(i2c_msg::buf)(data);
    io.msgs = &msg;
    io.nmsgs = 1;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("write failed: {}", strerror(errno));
        return false;
    }
    return true;
}

auto I2C_Linux::read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_linux::read_register");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Linux> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    memset(&io, 0, sizeof(i2c_rdwr_ioctl_data));

    struct i2c_msg msg[2];
    memset(msg, 0, sizeof(i2c_msg) * 2);

    msg[0].addr = address;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = decltype(i2c_msg::buf)(&reg);

    msg[1].addr = address;
    msg[1].flags = I2C_M_RD;
    msg[1].len = size;
    msg[1].buf = decltype(i2c_msg::buf)(data);

    io.msgs = msg;
    io.nmsgs = 2;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        QLOGW("read register {} failed: {}", reg, strerror(errno));
        return false;
    }
    return true;
}
auto I2C_Linux::write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_linux::write_register");
    QASSERT(m_fd >= 0);

    std::lock_guard<I2C_Linux> lg(*this);

    struct i2c_rdwr_ioctl_data io;
    memset(&io, 0, sizeof(i2c_rdwr_ioctl_data));

    struct i2c_msg msg;
    memset(&msg, 0, sizeof(i2c_msg));

    m_buffer.resize(size + 1);

    m_buffer[0] = reg;
    if (data)
    {
        std::copy(data, data + size, m_buffer.begin() + 1);
    }

    msg.addr = address;
    msg.flags = 0;
    msg.len = size + 1;
    msg.buf = decltype(i2c_msg::buf)(m_buffer.data());
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

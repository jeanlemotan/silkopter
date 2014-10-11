#include "BrainStdAfx.h"
#include "i2c.h"

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


i2c::~i2c()
{
    close();
}

auto i2c::open(const std::string &device) -> bool
{
    m_fd = ::open(device.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }
    return true;
}
void i2c::close()
{
    if (m_fd >= 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }
}

void i2c::read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t size)
{
    QASSERT(m_fd >= 0);
    struct i2c_rdwr_ioctl_data io;
    struct i2c_msg msg[2];

    msg[0].addr = addr;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &reg;

    msg[1].addr = addr;
    msg[1].flags = I2C_M_RD;
    msg[1].len = size;
    msg[1].buf = data;

    io.msgs = msg;
    io.nmsgs = 2;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        SILK_WARNING("i2c read reg failed: {}", strerror(errno));
    }
}
void i2c::write(uint8_t addr, uint8_t reg, uint8_t const* data, uint32_t size)
{
    QASSERT(m_fd >= 0);
    struct i2c_rdwr_ioctl_data io;
    struct i2c_msg msg;

    m_buffer.resize(size + 1);

    m_buffer[0] = reg;
    if (data)
    {
        std::copy(data, data + size, m_buffer.begin() + 1);
    }

    msg.addr = addr;
    msg.flags = 0;
    msg.len = size + 1;
    msg.buf = m_buffer.data();
    io.msgs = &msg;
    io.nmsgs = 1;
    if (ioctl(m_fd, I2C_RDWR, &io) < 0)
    {
        SILK_WARNING("i2c write reg failed: {}", strerror(errno));
    }
}


}

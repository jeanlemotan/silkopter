#include "Phy.h"
#include "pigpio.h"
#include <array>
#include <iostream>
#include <chrono>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cassert>

static const size_t CHUNK_SIZE = 64;

enum SPI_Command : uint16_t
{
    SPI_CMD_SEND_PACKET = 1,
    SPI_CMD_GET_PACKET = 2,
    SPI_CMD_SET_RATE = 3,
    SPI_CMD_GET_RATE = 4,
    SPI_CMD_SET_CHANNEL = 5,
    SPI_CMD_GET_CHANNEL = 6,
    SPI_CMD_SET_POWER = 7,
    SPI_CMD_GET_POWER = 8,
    SPI_CMD_GET_STATS = 9,
};

const size_t Phy::MAX_PAYLOAD_SIZE;
static const size_t MAX_PACKET_SIZE = Phy::MAX_PAYLOAD_SIZE + 2; //crc

static const uint32_t COMMAND_DELAY_US = 5000;


static const uint16_t s_crc_table[256] =
{
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
    0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x0919, 0x1890, 0x2A0B, 0x3B82, 0x4F3D, 0x5EB4, 0x6C2F, 0x7DA6,
    0x8551, 0x94D8, 0xA643, 0xB7CA, 0xC375, 0xD2FC, 0xE067, 0xF1EE,
    0x1232, 0x03BB, 0x3120, 0x20A9, 0x5416, 0x459F, 0x7704, 0x668D,
    0x9E7A, 0x8FF3, 0xBD68, 0xACE1, 0xD85E, 0xC9D7, 0xFB4C, 0xEAC5,
    0x1B2B, 0x0AA2, 0x3839, 0x29B0, 0x5D0F, 0x4C86, 0x7E1D, 0x6F94,
    0x9763, 0x86EA, 0xB471, 0xA5F8, 0xD147, 0xC0CE, 0xF255, 0xE3DC,
    0x2464, 0x35ED, 0x0776, 0x16FF, 0x6240, 0x73C9, 0x4152, 0x50DB,
    0xA82C, 0xB9A5, 0x8B3E, 0x9AB7, 0xEE08, 0xFF81, 0xCD1A, 0xDC93,
    0x2D7D, 0x3CF4, 0x0E6F, 0x1FE6, 0x6B59, 0x7AD0, 0x484B, 0x59C2,
    0xA135, 0xB0BC, 0x8227, 0x93AE, 0xE711, 0xF698, 0xC403, 0xD58A,
    0x3656, 0x27DF, 0x1544, 0x04CD, 0x7072, 0x61FB, 0x5360, 0x42E9,
    0xBA1E, 0xAB97, 0x990C, 0x8885, 0xFC3A, 0xEDB3, 0xDF28, 0xCEA1,
    0x3F4F, 0x2EC6, 0x1C5D, 0x0DD4, 0x796B, 0x68E2, 0x5A79, 0x4BF0,
    0xB307, 0xA28E, 0x9015, 0x819C, 0xF523, 0xE4AA, 0xD631, 0xC7B8,
    0x48C8, 0x5941, 0x6BDA, 0x7A53, 0x0EEC, 0x1F65, 0x2DFE, 0x3C77,
    0xC480, 0xD509, 0xE792, 0xF61B, 0x82A4, 0x932D, 0xA1B6, 0xB03F,
    0x41D1, 0x5058, 0x62C3, 0x734A, 0x07F5, 0x167C, 0x24E7, 0x356E,
    0xCD99, 0xDC10, 0xEE8B, 0xFF02, 0x8BBD, 0x9A34, 0xA8AF, 0xB926,
    0x5AFA, 0x4B73, 0x79E8, 0x6861, 0x1CDE, 0x0D57, 0x3FCC, 0x2E45,
    0xD6B2, 0xC73B, 0xF5A0, 0xE429, 0x9096, 0x811F, 0xB384, 0xA20D,
    0x53E3, 0x426A, 0x70F1, 0x6178, 0x15C7, 0x044E, 0x36D5, 0x275C,
    0xDFAB, 0xCE22, 0xFCB9, 0xED30, 0x998F, 0x8806, 0xBA9D, 0xAB14,
    0x6CAC, 0x7D25, 0x4FBE, 0x5E37, 0x2A88, 0x3B01, 0x099A, 0x1813,
    0xE0E4, 0xF16D, 0xC3F6, 0xD27F, 0xA6C0, 0xB749, 0x85D2, 0x945B,
    0x65B5, 0x743C, 0x46A7, 0x572E, 0x2391, 0x3218, 0x0083, 0x110A,
    0xE9FD, 0xF874, 0xCAEF, 0xDB66, 0xAFD9, 0xBE50, 0x8CCB, 0x9D42,
    0x7E9E, 0x6F17, 0x5D8C, 0x4C05, 0x38BA, 0x2933, 0x1BA8, 0x0A21,
    0xF2D6, 0xE35F, 0xD1C4, 0xC04D, 0xB4F2, 0xA57B, 0x97E0, 0x8669,
    0x7787, 0x660E, 0x5495, 0x451C, 0x31A3, 0x202A, 0x12B1, 0x0338,
    0xFBCF, 0xEA46, 0xD8DD, 0xC954, 0xBDEB, 0xAC62, 0x9EF9, 0x8F70
};

static uint16_t crc16(uint16_t crc, const void *c_ptr, size_t len)
{
    const uint8_t* c = reinterpret_cast<const uint8_t*>(c_ptr);
    while (len--)
    {
        crc = (crc << 8) ^ s_crc_table[((crc >> 8) ^ *c++)];
    }
    return crc;
}


//////////////////////////////////////////////////////////////////////////////

Phy::Phy()
{
}

//////////////////////////////////////////////////////////////////////////////

Phy::Init_Result Phy::init_pigpio(size_t port, size_t channel, size_t speed, size_t comms_delay)
{
    if (m_pigpio_fd >= 0 || m_dev_fd >= 0)
    {
        std::cerr << "Already initialized\n";
        return Init_Result::ALREADY_INITIALIZED;
    }

    m_speed = speed;
    m_comms_delay = comms_delay;

    if (port > 1)
    {
        std::cerr << "Only SPI ports 0 (main) & 1 (aux) are allowed\n";
        return Init_Result::BAD_PARAMS;
    }
    if (port == 0 && channel > 1)
    {
        std::cerr << "For port 0, only SPI channels 0 & 1 are allowed\n";
        return Init_Result::BAD_PARAMS;
    }
    if (port == 1 && channel > 2)
    {
        std::cerr << "For port 1, only SPI channels 0 & 1 & 2 are allowed\n";
        return Init_Result::BAD_PARAMS;
    }

    uint32_t flags = 0;
    if (port == 1)
    {
        flags |= 1 << 8;
    }

    m_pigpio_fd = spiOpen(channel, m_speed, flags);
    if (m_pigpio_fd < 0)
    {
        std::cerr << "Error opening SPI port " << std::to_string(port) <<
                         ", channel " << std::to_string(channel) <<
                         ", speed " << std::to_string(m_speed) <<
                         ": "  << std::to_string(m_pigpio_fd) <<
                         "\n";
        return Init_Result::HW_FAILURE;
    }

    return Init_Result::OK;
}

//////////////////////////////////////////////////////////////////////////////

Phy::Init_Result Phy::init_dev(const char* device, size_t speed, size_t comms_delay)
{
    if (m_pigpio_fd >= 0 || m_dev_fd >= 0)
    {
        std::cerr << "Already initialized\n";
        return Init_Result::ALREADY_INITIALIZED;
    }

    if (speed == 0)
    {
        std::cerr << "Invalid speed " << std::to_string(speed) << "\n";
        return Init_Result::BAD_PARAMS;
    }

    m_dev_fd = ::open(device, O_RDWR);
    if (m_dev_fd < 0)
    {
        std::cerr << "Can't open '" << device << "': " << strerror(errno) << "\n";
        return Init_Result::HW_FAILURE;
    }

    int bits = 8;
    int ret = ioctl(m_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        ::close(m_dev_fd);
        m_dev_fd = -1;
        std::cerr << "Can't set bits per word\n";
        return Init_Result::HW_FAILURE;
    }

    ret = ioctl(m_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        ::close(m_dev_fd);
        m_dev_fd = -1;
        std::cerr << "Can't set speed\n";
        return Init_Result::HW_FAILURE;
    }

    m_comms_delay = comms_delay;
    m_speed = speed;

    memset(m_spi_transfers.data(), 0, sizeof(spi_ioc_transfer) * m_spi_transfers.size());
    for (std::vector<uint8_t>& transfer_data: m_spi_transfers_data)
    {
        transfer_data.resize(2 + CHUNK_SIZE);
        transfer_data[0] = 0x2;
        transfer_data[1] = 0x0;
    }

    return Init_Result::OK;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::transfer(void const* tx_data, void* rx_data, size_t size)
{
    assert(size > 0);
    if (size == 0)
    {
        return false;
    }

    if (m_pigpio_fd >= 0)
    {
        int result = 0;
        if (tx_data && rx_data)
        {
            result = spiXfer(m_pigpio_fd, (char*)tx_data, (char*)rx_data, size);
        }
        else if (tx_data)
        {
            result = spiWrite(m_pigpio_fd, (char*)tx_data, size);
        }
        else if (rx_data)
        {
            result = spiRead(m_pigpio_fd, (char*)rx_data, size);
        }
        if (result < 0)
        {
            return false;
        }
        if (m_comms_delay > 0)
        {
            gpioDelay(m_comms_delay);
        }
    }
    else
    {
        m_spi_transfers[0].tx_buf = (unsigned long)tx_data;
        m_spi_transfers[0].rx_buf = (unsigned long)rx_data;
        m_spi_transfers[0].len = size;
        m_spi_transfers[0].speed_hz = m_speed;
        m_spi_transfers[0].bits_per_word = 8;
        m_spi_transfers[0].delay_usecs = m_comms_delay;
        m_spi_transfers[0].cs_change = 0;

        int status = ioctl(m_dev_fd, SPI_IOC_MESSAGE(1), &m_spi_transfers[0]);
        if (status < 0)
        {
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////

uint32_t Phy::get_status()
{
    std::array<uint8_t, 5> tx = { 0x4, 0x0 };
    std::array<uint8_t, 5> rx = { 0x0 };

    transfer(tx.data(), rx.data(), tx.size());

    uint32_t res;
    memcpy(&res, rx.data() + 1, 4);

    return res;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::send_command(uint32_t command)
{
    std::array<uint8_t, 5> tx = { 0x1, 0x0 };

    memcpy(tx.data() + 1, &command, 4);

    return transfer(tx.data(), nullptr, tx.size());
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::send_data(void const* data, size_t size)
{
    if (size > MAX_PAYLOAD_SIZE)
    {
        assert(false);
        return false;
    }

    std::lock_guard<std::mutex> lg(m_mutex);

    uint32_t command = (SPI_Command::SPI_CMD_SEND_PACKET << 24) | (size + 2); //add the crc size
    if (!send_command(command))
    {
        return false;
    }

    uint16_t crc = crc16(0, data, size);

    if (m_pigpio_fd >= 0)
    {
        uint8_t tx[CHUNK_SIZE + 2];
        tx[0] = 0x2;
        tx[1] = 0x0;

        uint8_t const* data_ptr = reinterpret_cast<uint8_t const*>(data);
        size_t left = size;
        while (left > 0)
        {
            size_t chunk_size = std::min(CHUNK_SIZE, left);
            uint8_t* dst_ptr = tx + 2;//these 2 bytes are the spi commands
            if (left == size)
            {
                //first time add the crc
                chunk_size = std::min(CHUNK_SIZE - 2, left);
                memcpy(dst_ptr, &crc, 2);
                dst_ptr += 2;
            }
            memcpy(dst_ptr, data_ptr, chunk_size);
            if (!transfer(tx, nullptr, CHUNK_SIZE + 2)) //always do complete transactions
            {
                return false;
            }
            left -= chunk_size;
            data_ptr += chunk_size;
        }
    }
    else
    {
        size_t transfer_count = 0;
        uint8_t const* data_ptr = reinterpret_cast<uint8_t const*>(data);
        size_t left = size;
        while (left > 0)
        {
            std::vector<uint8_t>& tx = m_spi_transfers_data[transfer_count];
            size_t chunk_size = std::min(CHUNK_SIZE, left);
            uint8_t* dst_ptr = tx.data() + 2;//these 2 bytes are the spi commands
            if (left == size)
            {
                //first time add the crc
                chunk_size = std::min(CHUNK_SIZE - 2, left);
                memcpy(dst_ptr, &crc, 2);
                dst_ptr += 2;
            }

            memcpy(dst_ptr, data_ptr, chunk_size);

            left -= chunk_size;
            data_ptr += chunk_size;

            m_spi_transfers[transfer_count].tx_buf = (unsigned long)tx.data();
            m_spi_transfers[transfer_count].rx_buf = (unsigned long)0;
            m_spi_transfers[transfer_count].len = tx.size();
            m_spi_transfers[transfer_count].speed_hz = m_speed;
            m_spi_transfers[transfer_count].bits_per_word = 8;
            m_spi_transfers[transfer_count].delay_usecs = m_comms_delay;
            m_spi_transfers[transfer_count].cs_change = 0;
            transfer_count++;
        }

        int status = ioctl(m_dev_fd, SPI_IOC_MESSAGE(transfer_count), m_spi_transfers.data());
        if (status < 0)
        {
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::receive_data(void* data, size_t& size, int& rssi)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    assert(data);
    size = 0;
    rssi = 0;

    uint32_t command = SPI_Command::SPI_CMD_GET_PACKET << 24;
    if (!send_command(command))
    {
        return false;
    }

    uint32_t status = get_status();
    if ((status >> 24) != SPI_Command::SPI_CMD_GET_PACKET)
    {
        return false;
    }

    uint8_t urssi = (status >> 16) & 0xFF;
    rssi = reinterpret_cast<int8_t&>(urssi);
    size = std::min<uint32_t>(status & 0xFFFF, MAX_PACKET_SIZE);
    if (size < 2)
    {
        size = 0;
        return false;
    }

    uint32_t crc = 0;

    uint8_t tx[CHUNK_SIZE + 2];
    uint8_t rx[CHUNK_SIZE + 2];
    tx[0] = 0x3;
    tx[1] = 0x0;

    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(data);
    size_t left = size;
    while (left > 0)
    {
        size_t chunk_size = std::min(CHUNK_SIZE, left);
        if (!transfer(tx, rx, CHUNK_SIZE + 2)) //always do complete transactions
        {
            return false;
        }

        uint8_t const* src_ptr = rx + 2; //these 2 bytes are the spi commands
        if (left == size) //consume the crc first
        {
            memcpy(&crc, src_ptr, 2);
            src_ptr += 2;
            chunk_size = std::min(CHUNK_SIZE - 2, left);
        }
        memcpy(data_ptr, src_ptr, chunk_size);
        left -= chunk_size;
        data_ptr += chunk_size;
    }

    size -= 2; //ythe first 2 bytes are the crc

    //the first 2 bytes are the crc
    uint16_t computed_crc = crc16(0, data, size);
    if (crc != computed_crc)
    {
//        for (size_t i = 0; i < size; i++)
//        {
//            printf("%02X,", ((uint8_t const*)data)[i]);
//        }
        //std::cout << "CRC for size " << std::to_string(size) << " is " << std::to_string(crc) << " \n";
        size = 0;
        return false;
    }

//    std::cout << "Incoming " << std::to_string(status) << " " << std::to_string(size) << "\n";
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::set_rate(Rate rate)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint32_t command = (SPI_Command::SPI_CMD_SET_RATE << 24) | static_cast<uint32_t>(rate);
    bool res = send_command(command);
    gpioDelay(COMMAND_DELAY_US);
    return res;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_rate(Rate& rate)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint32_t command = (SPI_Command::SPI_CMD_GET_RATE << 24);
    if (!send_command(command))
    {
        return false;
    }
    gpioDelay(COMMAND_DELAY_US);

    uint32_t status = get_status();
    if ((status >> 24) != SPI_Command::SPI_CMD_GET_RATE)
    {
        return false;
    }
    if ((status & 0xFFFF) == 0xFFFF)
    {
        return false;//unknown rate
    }
    rate = static_cast<Rate>(status & 0xFFFF);
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::set_channel(uint8_t channel)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    if (channel < 1 || channel > 11)
    {
        return false;
    }
    uint32_t command = (SPI_Command::SPI_CMD_SET_CHANNEL << 24) | channel;
    bool res = send_command(command);
    gpioDelay(COMMAND_DELAY_US);
    return res;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_channel(uint8_t& channel)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint32_t command = (SPI_Command::SPI_CMD_GET_CHANNEL << 24);
    if (!send_command(command))
    {
        return false;
    }
    gpioDelay(COMMAND_DELAY_US);

    uint32_t status = get_status();
    if ((status >> 24) != SPI_Command::SPI_CMD_GET_CHANNEL)
    {
        return false;
    }
    channel = static_cast<uint8_t>(status & 0xFFFF);
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::set_power(float dBm)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint16_t power = static_cast<uint16_t>(std::max(std::min((dBm * 100.f), 32767.f), -32767.f) + 32767.f);
    uint32_t command = (SPI_Command::SPI_CMD_SET_POWER << 24) | power;
    bool res = send_command(command);
    gpioDelay(COMMAND_DELAY_US);
    return res;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_power(float& dBm)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint32_t command = (SPI_Command::SPI_CMD_GET_POWER << 24);
    if (!send_command(command))
    {
        return false;
    }
    gpioDelay(COMMAND_DELAY_US);

    uint32_t status = get_status();
    if ((status >> 24) != SPI_Command::SPI_CMD_GET_POWER)
    {
        return false;
    }
    dBm = (static_cast<float>(status & 0xFFFF) - 32768.f) / 100.f;
    if (dBm < 0.f || dBm > 20.5f)
    {
        dBm = 0.f;
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_stats(Stats& stats)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint32_t command = (SPI_Command::SPI_CMD_GET_STATS << 24);
    if (!send_command(command))
    {
        return false;
    }
    gpioDelay(COMMAND_DELAY_US);

    {
        std::array<uint8_t, 2 + CHUNK_SIZE> tx = { 0x3, 0x0 };
        std::array<uint8_t, 2 + CHUNK_SIZE> rx = { 0x0 };

        if (!transfer(tx.data(), rx.data(), tx.size())) //always do complete transactions
        {
            return false;
        }
        memcpy(&stats, rx.data() + 2, sizeof(Stats));
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////

void Phy::process()
{
//    std::chrono::high_resolution_clock::time_point start_tp = std::chrono::high_resolution_clock::now();

//    //set_status(0);

//    std::array<uint8_t, MAX_PACKET_SIZE> payload = { 0 };
//    for (size_t i = 0; i < MAX_PACKET_SIZE; i++)
//    {
//        payload[i] = ('A' + i % 26);
//    }

//    size_t count = 0;
//    while (true)
//    {
//        count++;
//        send_data(payload.data(), payload.size());
//        size_t size = 0;
//        size_t rssi = 0;
//        receive_data(payload.data(), size, rssi);

//        //gpioDelay(1000);

//        //exit(1);

//        if (std::chrono::high_resolution_clock::now() - start_tp >= std::chrono::milliseconds(1000))
//        {
//            start_tp = std::chrono::high_resolution_clock::now();
//            std::cout << std::to_string(count) + "\n";
//            std::flush(std::cout);
//            std::flush(std::cerr);
//            count = 0;
//        }
//    }
}

//////////////////////////////////////////////////////////////////////////////


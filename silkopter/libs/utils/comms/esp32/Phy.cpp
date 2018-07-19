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
#include <thread>
#include <stdio.h>
#include <stdarg.h>
#include "spi_comms.h"

static const size_t CHUNK_SIZE = 1024;

const size_t Phy::MAX_PAYLOAD_SIZE;
static const size_t MAX_PACKET_SIZE = Phy::MAX_PAYLOAD_SIZE + 2; //crc

static const uint32_t COMMAND_DELAY_US = 5000;


static const uint16_t s_crc16_table[256] =
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
        crc = (crc << 8) ^ s_crc16_table[((crc >> 8) ^ *c++)];
    }
    return crc;
}

static uint8_t s_crc8_table[256];     /* 8-bit table */
static void init_crc8_table()
{
    static constexpr uint8_t DI = 0x07;
    for (uint16_t i = 0; i < 256; i++)
    {
        uint8_t crc = (uint8_t)i;
        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
        }
        s_crc8_table[i] = crc & 0xFF;
    }
}

static uint8_t crc8(uint8_t crc, const void *c_ptr, size_t len)
{
    const uint8_t* c = reinterpret_cast<const uint8_t*>(c_ptr);
    size_t n = (len + 7) >> 3;
    switch (len & 7)
    {
    case 0: do { crc = s_crc8_table[crc ^ (*c++)];
    case 7:      crc = s_crc8_table[crc ^ (*c++)];
    case 6:      crc = s_crc8_table[crc ^ (*c++)];
    case 5:      crc = s_crc8_table[crc ^ (*c++)];
    case 4:      crc = s_crc8_table[crc ^ (*c++)];
    case 3:      crc = s_crc8_table[crc ^ (*c++)];
    case 2:      crc = s_crc8_table[crc ^ (*c++)];
    case 1:      crc = s_crc8_table[crc ^ (*c++)];
            } while (--n > 0);
    }
    return crc;
}

void log(const char* format, const char* file, int line, ...)
{
    char dest[1024 * 16];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    printf("%s(%d): %s\n", file, line, dest);
}

#define LOG(format, ...) log(format, __FILE__, __LINE__, ##__VA_ARGS__)
//#define LOG(format, ...)

//////////////////////////////////////////////////////////////////////////////

Phy::Phy()
{
    init_crc8_table();
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

    //speed = 10000000;

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

    for (spi_ioc_transfer& spi_transfer : m_spi_transfers)
    {
        memset(&spi_transfer, 0, sizeof(spi_ioc_transfer));
    }

    return Init_Result::OK;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::spi_transfer(void const* tx_data, void* rx_data, size_t size)
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
            LOG("Transfer error: %d", result);
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
            LOG("Transfer error: %d", status);
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////

void Phy::prepare_transfer_buffers(size_t size)
{
    size_t padding = size & 15;
    if (padding > 0)
    {
        size += 16 - padding;
    }
    m_tx_buffer.resize(size);
    m_rx_buffer.resize(size);
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::transfer(void const* data, size_t size)
{
    if (size > MAX_PAYLOAD_SIZE)
    {
        assert(false);
        LOG("bad arg");
        return false;
    }

    m_last_transfer_tp = std::chrono::high_resolution_clock::now();

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header) + size, sizeof(SPI_Res_Packet_Header) + m_next_packet_size));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = size;
        header.crc = crc8(0, &header, sizeof(header));
        if (size > 0 && data)
        {
            memcpy(m_tx_buffer.data() + sizeof(header), data, size);
        }
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        if (size > 0)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(200)); //needed to avoid spi errors due to the esp not being ready quickly enough
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(50)); //needed to avoid spi errors due to the esp not being ready quickly enough
        }

        SPI_Res_Packet_Header& response = *reinterpret_cast<SPI_Res_Packet_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.next_packet_size > MAX_PAYLOAD_SIZE)
        {
            LOG("invalid next_packet_size: got %d, expected <= %d", (int)response.next_packet_size, (int)MAX_PAYLOAD_SIZE);
            return false;
        }
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;

        if (m_rx_buffer.size() < response.packet_size + sizeof(SPI_Res_Packet_Header))
        {
            LOG("insuficient data: got %d, expected %d", (int)m_rx_buffer.size(), (int)response.packet_size + sizeof(SPI_Res_Packet_Header));
            return false;
        }
        if (response.packet_size > 0)
        {
            if (m_rx_packet_pool.empty())
            {
                m_rx_packets.emplace_back();
            }
            else
            {
                m_rx_packets.emplace_back(std::move(m_rx_packet_pool.back()));
                m_rx_packet_pool.pop_back();
            }
            RX_Packet& packet = m_rx_packets.back();
            packet.rssi = response.rssi;
            packet.data.resize(response.packet_size);
            if (response.packet_size > 0)
            {
                memcpy(packet.data.data(), m_rx_buffer.data() + sizeof(SPI_Res_Packet_Header), response.packet_size);
            }
            LOG("received packet id %d, size %d", (int)response.packet_id, (int)response.packet_size);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::send_data(void const* data, size_t size)
{
    if (!data || size > MAX_PAYLOAD_SIZE)
    {
        assert(false);
        LOG("bad arg");
        return false;
    }

    std::lock_guard<std::mutex> lg(m_mutex);

    return transfer(data, size);
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::receive_data(void* data, size_t& size, int& rssi)
{
    if (!data)
    {
        assert(false);
        LOG("bad arg");
        return false;
    }

    std::lock_guard<std::mutex> lg(m_mutex);

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    if (now - m_last_transfer_tp >= std::chrono::milliseconds(3))
    {
        size_t rounds = 5;
        do
        {
            transfer(nullptr, 0);
            rounds--;
        } while (m_pending_packets > 1 && rounds > 0);
    }

    if (!m_rx_packets.empty())
    {
        RX_Packet& packet = m_rx_packets.front();
        rssi = packet.rssi;
        size = packet.data.size();
        if (size > 0)
        {
            memcpy(data, packet.data.data(), size);
        }
        m_rx_packet_pool.push_back(std::move(packet));
        m_rx_packets.pop_front();
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::set_rate(Rate rate)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Set_Rate_Header));
        SPI_Req_Set_Rate_Header& header = *reinterpret_cast<SPI_Req_Set_Rate_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::SET_RATE);
        header.seq = seq;
        header.rate = static_cast<uint8_t>(rate);
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Set_Rate_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Set_Rate_Header& response = *reinterpret_cast<SPI_Res_Set_Rate_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        if (response.rate != static_cast<uint8_t>(rate))
        {
            LOG("command failed: got %d, expected %d", (int)response.rate, (int)rate);
            return false;
        }
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_rate(Rate& rate)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Get_Rate_Header));
        SPI_Req_Get_Rate_Header& header = *reinterpret_cast<SPI_Req_Get_Rate_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::GET_RATE);
        header.seq = seq;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Get_Rate_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Get_Rate_Header& response = *reinterpret_cast<SPI_Res_Get_Rate_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        rate = static_cast<Rate>(response.rate);
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::set_channel(uint8_t channel)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    if (channel < 1 || channel > 11)
    {
        LOG("bad arg");
        return false;
    }

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Set_Channel_Header));
        SPI_Req_Set_Channel_Header& header = *reinterpret_cast<SPI_Req_Set_Channel_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::SET_CHANNEL);
        header.seq = seq;
        header.channel = channel;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Set_Channel_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Set_Channel_Header& response = *reinterpret_cast<SPI_Res_Set_Channel_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        if (response.channel != channel)
        {
            LOG("command failed: got %d, expected %d", (int)response.channel, (int)channel);
            return false;
        }
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_channel(uint8_t& channel)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    if (channel < 1 || channel > 11)
    {
        return false;
    }

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Get_Channel_Header));
        SPI_Req_Get_Channel_Header& header = *reinterpret_cast<SPI_Req_Get_Channel_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::GET_CHANNEL);
        header.seq = seq;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Get_Channel_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Get_Channel_Header& response = *reinterpret_cast<SPI_Res_Get_Channel_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        channel = response.channel;
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::set_power(float dBm)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    dBm = std::max(std::min(dBm, 100.f), -100.f);

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Set_Power_Header));
        SPI_Req_Set_Power_Header& header = *reinterpret_cast<SPI_Req_Set_Power_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::SET_POWER);
        header.seq = seq;
        header.power = static_cast<int16_t>(dBm * 10.f);
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Set_Power_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Set_Power_Header& response = *reinterpret_cast<SPI_Res_Set_Power_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::get_power(float& dBm)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    dBm = 0;

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Get_Power_Header));
        SPI_Req_Get_Power_Header& header = *reinterpret_cast<SPI_Req_Get_Power_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::GET_POWER);
        header.seq = seq;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Get_Power_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Get_Power_Header& response = *reinterpret_cast<SPI_Res_Get_Power_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        dBm = static_cast<float>(response.power) / 10.f;
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

bool Phy::setup_fec_channel(size_t coding_k, size_t coding_n, size_t mtu)
{
    std::lock_guard<std::mutex> lg(m_mutex);

    uint8_t seq = (++m_seq) & 0x7F;
    {
        prepare_transfer_buffers(sizeof(SPI_Req_Setup_Fec_Codec_Header));
        SPI_Req_Setup_Fec_Codec_Header& header = *reinterpret_cast<SPI_Req_Setup_Fec_Codec_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::SETUP_FEC_CODEC);
        header.seq = seq;
        header.fec_coding_k = coding_k;
        header.fec_coding_n = coding_n;
        header.fec_mtu = mtu;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    {
        prepare_transfer_buffers(std::max(sizeof(SPI_Req_Packet_Header), sizeof(SPI_Res_Setup_Fec_Codec_Header)));
        SPI_Req_Packet_Header& header = *reinterpret_cast<SPI_Req_Packet_Header*>(m_tx_buffer.data());
        memset(&header, 0, sizeof(header));
        header.req = static_cast<uint8_t>(SPI_Req::PACKET);
        header.seq = (++m_seq) & 0x7F;
        header.packet_size = 0;
        header.crc = crc8(0, &header, sizeof(header));
        if (!spi_transfer(m_tx_buffer.data(), m_rx_buffer.data(), m_tx_buffer.size()))
        {
            LOG("transfer failed");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        SPI_Res_Setup_Fec_Codec_Header& response = *reinterpret_cast<SPI_Res_Setup_Fec_Codec_Header*>(m_rx_buffer.data());
        uint8_t response_crc = response.crc;
        response.crc = 0;
        uint8_t response_computed_crc = crc8(0, &response, sizeof(response));
        if (response_crc != response_computed_crc)
        {
            LOG("mismatched crc: got %d, expected %d", (int)response_crc, (int)response_computed_crc);
            return false;
        }
        if (response.seq != seq)
        {
            LOG("invalid seq: got %d, expected %d", (int)response.seq, (int)seq);
            return false;
        }
        if (response.fec_coding_k != coding_k ||
                response.fec_coding_n != coding_n ||
                response.fec_mtu != mtu)
        {
            LOG("command failed");
            return false;
        }
        m_pending_packets = response.pending_packets;
        m_next_packet_size = response.next_packet_size;
    }
    return true;
}

void Phy::process()
{
    //    static std::chrono::high_resolution_clock::time_point start_tp = std::chrono::high_resolution_clock::now();

    //    set_rate(Rate::RATE_G_54M_ODFM);
    //    //set_channel(7);
    //    set_power(21.f);
    //    setup_fec_channel(4, 6, MAX_PAYLOAD_SIZE);

    //    std::array<uint8_t, MAX_PAYLOAD_SIZE> tx_data;
    ////    send_data(tx_data.data(), tx_data.size());

    //    int accumulated_rssi = 0;
    //    size_t accumulated_rx_size = 0;
    //    size_t rx_packets = 0;
    //    std::array<uint8_t, MAX_PAYLOAD_SIZE> rx_data;

    //    size_t count = 0;
    //    while (true)
    //    {
    //        count++;

    //        send_data(tx_data.data(), tx_data.size());
    //        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    //        if (now - m_last_transfer_tp >= std::chrono::milliseconds(2))
    //        {
    //            std::lock_guard<std::mutex> lg(m_mutex);
    //            size_t rounds = 5;
    //            do
    //            {
    //                transfer(nullptr, 0);
    //                rounds--;
    //            } while (m_pending_packets > 1 && rounds > 0);
    //        }

    //        size_t rx_size = 0;
    //        int rssi = 0;
    //        if (receive_data(rx_data.data(), rx_size, rssi))
    //        {
    //            rx_packets++;
    //            accumulated_rx_size += rx_size;
    //            accumulated_rssi += rssi;
    //        }

    //        //gpioDelay(2000);
    //        //std::this_thread::sleep_for(std::chrono::milliseconds(200));

    //        //exit(1);

    //        if (std::chrono::high_resolution_clock::now() - start_tp >= std::chrono::milliseconds(1000))
    //        {
    //            start_tp = std::chrono::high_resolution_clock::now();
    //            std::cout << "TX: " << std::to_string(count) <<
    //                         ", RX: " << std::to_string(rx_packets) <<
    //                         ", RX size: " << std::to_string(accumulated_rx_size / std::max(rx_packets, 1u)) <<
    //                         ", RX rssi: " << std::to_string(accumulated_rssi / int(std::max(rx_packets, 1u))) <<
    //                         "\n";
    //            std::flush(std::cout);
    //            std::flush(std::cerr);
    //            count = 0;
    //            rx_packets = 0;
    //            accumulated_rx_size = 0;
    //            accumulated_rssi = 0;
    //        }
    //    }
}

//////////////////////////////////////////////////////////////////////////////

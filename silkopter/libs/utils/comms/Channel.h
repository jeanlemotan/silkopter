#pragma once

#include <vector>
#include <deque>
#include <mutex>
#include <cassert>
#include "utils/Crc.h"

namespace util
{
namespace comms
{

template<class MESSAGE_T, class SOCKET_T>
class Channel
{
    Channel(Channel<MESSAGE_T, SOCKET_T> const&) = delete;
    Channel<MESSAGE_T, SOCKET_T>& operator=(Channel<MESSAGE_T, SOCKET_T> const&) = delete;
public:
    typedef MESSAGE_T Message_t;
    typedef SOCKET_T Socket_t;

    Channel(Socket_t& socket) : m_socket(socket) {}

    void process()  { Message_t message; get_next_message(message); }

    //////////////////////////////////////////////////////////////////////////

    void send(Message_t message, void const* data, size_t size)
    {
        return _send(message, data, size);
    }

    bool get_next_message(Message_t& message)  { return _get_next_message(message); }

    enum class Unpack_Result : uint8_t
    {
        OK = 0,				//all good
        FAILED,				//a generic error
    };

    //decodes the next message
    template<class Dst>
    Unpack_Result unpack(Dst& dst) { return _unpack(dst); }

    //////////////////////////////////////////////////////////////////////////

    size_t get_pending_data_size() const { return m_rx_buffer.size(); }
    size_t get_error_count() const { return m_error_count; }

private:
    typedef uint8_t Magic_t;
    typedef uint8_t Message_Size_t;
    typedef uint8_t Header_Crc_t;
    typedef uint16_t Data_Crc_t;

    static const uint8_t MAGIC = 0x3F;

    static const size_t MAGIC_OFFSET = 0;
    static const size_t MESSAGE_OFFSET = MAGIC_OFFSET + sizeof(Magic_t);
    static const size_t SIZE_OFFSET = MESSAGE_OFFSET + sizeof(Message_t);
    static const size_t HEADER_CRC_OFFSET = SIZE_OFFSET + sizeof(Message_Size_t);
    static const size_t DATA_CRC_OFFSET = HEADER_CRC_OFFSET + sizeof(Header_Crc_t);
    static const size_t HEADER_SIZE = DATA_CRC_OFFSET + sizeof(Data_Crc_t);

    typedef std::vector<uint8_t> RX_Buffer_t;
    typedef std::vector<uint8_t> TX_Buffer_t;
    typedef Channel<MESSAGE_T, SOCKET_T> This_t;


    struct Decoded
    {
        Magic_t magic = 0;
        Message_t message;
        Message_Size_t data_size = 0;
        Header_Crc_t header_crc = 0;
        Data_Crc_t data_crc = 0;
    } m_decoded;

    template<class T> T get_value_fixed(RX_Buffer_t const& t, size_t off)
    {
        assert(off + sizeof(T) <= t.size());
        T val;
        uint8_t* dst = reinterpret_cast<uint8_t*>(&val);
        for (uint8_t i = 0, sz = sizeof(T); i < sz; i++)
        {
            *dst++ = t[off + i];
        }
        return val;
    }
    template<class Container, class T> void set_value_fixed(Container& t, T const& val, size_t off)
    {
        assert(off + sizeof(T) <= t.size());
        uint8_t const* src = reinterpret_cast<uint8_t const*>(&val);
        for (size_t i = 0, sz = sizeof(T); i < sz; i++)
        {
            t[off + i] = *src++;
        }
    }

    void pop_front(size_t size)
    {
        assert(size <= m_rx_buffer.size());
        m_rx_buffer.erase(m_rx_buffer.begin(), m_rx_buffer.begin() + size);
    }

    //returns the nest message or nothing.
    //the message, if any, has to be decoded with decode_next_message(...)
    bool _get_next_message(Message_t& message)
    {
        if (m_decoded.data_size > 0)
        {
            assert(m_decoded.data_size <= m_rx_buffer.size());
            pop_front(m_decoded.data_size);
            m_decoded.data_size = 0;
        }

        //check for incoming messages
        while (decode_message()) {}
        if (m_decoded.magic == 0)
        {
            //read from the socket
            m_socket.read(m_rx_buffer);
            return false;
        }

        message = m_decoded.message;
        return true;
    }

    //decodes the next message
    template<typename Dst>
    Unpack_Result _unpack(Dst& dst)
    {
        assert(m_decoded.data_size <= m_rx_buffer.size());
        if (m_decoded.data_size == 0)
        {
            return Unpack_Result::FAILED;
        }
        size_t offset = dst.size();
        dst.resize(offset + m_decoded.data_size);
        std::copy(m_rx_buffer.begin(), m_rx_buffer.begin() + m_decoded.data_size, dst.begin() + offset);
        return Unpack_Result::OK;
    }

    bool decode_message()
    {
        m_decoded.magic = 0;
        m_decoded.data_size = 0;

        //check if we have enough data
        if (m_rx_buffer.size() < HEADER_SIZE)
        {
            //read from the socket and check again
            m_socket.read(m_rx_buffer);
            if (m_rx_buffer.size() < HEADER_SIZE)
            {
                return false;
            }
        }

        //try to decode a message HEADER
        Magic_t magic = get_value_fixed<Magic_t>(m_rx_buffer, MAGIC_OFFSET);
        if (magic != MAGIC)
        {
            m_error_count++;
            assert(0 && "malformed package magic");
            pop_front(1);
            return true;
        }
        Message_t message = get_value_fixed<Message_t>(m_rx_buffer, MESSAGE_OFFSET);
        Message_Size_t size = get_value_fixed<Message_Size_t>(m_rx_buffer, SIZE_OFFSET);
        Header_Crc_t header_crc = get_value_fixed<Header_Crc_t>(m_rx_buffer, HEADER_CRC_OFFSET);

        //verify header crc
        {
            Header_Crc_t computed_header_crc = util::compute_crc<Header_Crc_t>(m_rx_buffer, HEADER_CRC_OFFSET);
            if (header_crc != computed_header_crc)
            {
                m_error_count++;
                assert(0 && "header crc failed");
                pop_front(1);
                return true;
            }
        }

        if (m_rx_buffer.size() < HEADER_SIZE + size)
        {
            //read from the socket and check again
            m_socket.read(m_rx_buffer);
            if (m_rx_buffer.size() < HEADER_SIZE + size)
            {
                return false;
            }
        }

        Data_Crc_t data_crc = get_value_fixed<Data_Crc_t>(m_rx_buffer, DATA_CRC_OFFSET);

        //clear crc bytes and compute crc
        set_value_fixed(m_rx_buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
        Data_Crc_t computed_data_crc = util::compute_crc<Data_Crc_t>(m_rx_buffer, HEADER_SIZE + size);
        if (data_crc != computed_data_crc)
        {
            m_error_count++;
            assert(0 && "data crc failed");
            set_value_fixed(m_rx_buffer, data_crc, DATA_CRC_OFFSET);
            pop_front(1);
            return true;
        }
        pop_front(HEADER_SIZE);

        m_decoded.magic = magic;
        m_decoded.message = message;
        m_decoded.data_size = size;
        m_decoded.header_crc = header_crc;
        m_decoded.data_crc = data_crc;

        assert(m_decoded.data_size <= m_rx_buffer.size());

        return false;
    }

    void _send(Message_t message, size_t total_size)
    {
        assert(total_size < 255);
        assert(total_size >= HEADER_SIZE);
        size_t data_size = total_size - HEADER_SIZE;
        //header
        Magic_t magic = MAGIC;
        set_value_fixed(m_tx_buffer, magic, MAGIC_OFFSET);
        set_value_fixed(m_tx_buffer, message, MESSAGE_OFFSET);
        set_value_fixed(m_tx_buffer, Message_Size_t(data_size), SIZE_OFFSET);
        set_value_fixed(m_tx_buffer, Header_Crc_t(0), HEADER_CRC_OFFSET);

        //header crc
        Header_Crc_t header_crc = util::compute_crc<Header_Crc_t>(m_tx_buffer, HEADER_CRC_OFFSET);
        set_value_fixed(m_tx_buffer, header_crc, HEADER_CRC_OFFSET);

        //data crc
        set_value_fixed(m_tx_buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
        Data_Crc_t data_crc = util::compute_crc<Data_Crc_t>(m_tx_buffer, total_size);
        set_value_fixed(m_tx_buffer, data_crc, DATA_CRC_OFFSET);

        //send
        m_socket.write(m_tx_buffer, total_size);
    }

    //sends a message with confirmation
    void _send(Message_t message, void const* data, size_t size)
    {
        m_tx_buffer.resize(HEADER_SIZE + size);
        std::copy(reinterpret_cast<uint8_t const*>(data), reinterpret_cast<uint8_t const*>(data) + size, m_tx_buffer.begin() + HEADER_SIZE);
        _send(message, m_tx_buffer.size());
    }

    //////////////////////////////////////////////////////////////////////////

    Socket_t& m_socket;
    RX_Buffer_t m_rx_buffer;
    TX_Buffer_t m_tx_buffer;
    size_t m_error_count = 0;
};

}
}

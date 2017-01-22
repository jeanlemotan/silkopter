#pragma once

#include <boost/optional.hpp>
#include "utils/Serialization.h"
#include "RCP.h"

namespace util
{
namespace comms
{

template<class MESSAGE_SIZE_T>
class RCP_Channel : q::util::Noncopyable
{
    static const size_t SIZE_OFFSET = 0;
    static const size_t HEADER_SIZE = SIZE_OFFSET + sizeof(MESSAGE_SIZE_T);

public:
    typedef MESSAGE_SIZE_T Message_Size_t;

    typedef std::vector<uint8_t> TX_Buffer_t;
    typedef std::vector<uint8_t> RX_Buffer_t;

    RCP_Channel(uint8_t channel_idx) : m_channel_idx(channel_idx) {}

    //////////////////////////////////////////////////////////////////////////

    //sends a message with confirmation
    template<typename... Params>
    void pack_all(Params&&... params)
    {
        begin_pack();
        _pack(std::forward<Params>(params)...);
        end_pack();
    }

    void begin_pack()
    {
        auto off = m_tx_buffer.size();
        m_size_off = off;
        serialization::serialize(m_tx_buffer, Message_Size_t(0), off);
        m_data_start_off = off;
    }
    template<class Param> void pack_param_at(size_t off, Param const& p)
    {
        QASSERT(m_tx_buffer.size() >= m_data_start_off + off + sizeof(p));
        QASSERT(m_data_start_off > m_size_off);
        serialization::serialize(m_tx_buffer, p, m_data_start_off + off);
    }
    template<class Param> void pack_param(Param const& p)
    {
        QASSERT(m_tx_buffer.size() >= m_data_start_off);
        QASSERT(m_data_start_off > m_size_off);
        auto off = m_tx_buffer.size();
        serialization::serialize(m_tx_buffer, p, off);
    }
    void pack_data(uint8_t const* src, size_t size)
    {
        QASSERT(src);
        QASSERT(m_tx_buffer.size() >= m_data_start_off);
        QASSERT(m_data_start_off > m_size_off);
        auto off = m_tx_buffer.size();
        m_tx_buffer.resize(off + size);
        std::copy(src, src + size, m_tx_buffer.begin() + off);
    }
    void end_pack()
    {
        QASSERT(m_tx_buffer.size() >= m_data_start_off);
        QASSERT(m_data_start_off > m_size_off);
        size_t data_size = m_tx_buffer.size() - m_data_start_off;
        //header
        serialization::serialize(m_tx_buffer, Message_Size_t(data_size), m_size_off);

        //q::quick_logf("sending msg {}, size {}, hcrc {}, dcrc {}", message, data_size, header_crc, data_crc);
        m_size_off = 0;
        m_data_start_off = 0;
    }

    auto has_tx_data() const -> bool
    {
        return !m_tx_buffer.empty();
    }
    auto has_rx_data() const -> bool
    {
        return !m_rx_buffer.empty();
    }

    void send(RCP& rcp)
    {
        if (!m_tx_buffer.empty())
        {
            //q::quick_logf("Sending {} bytes", m_tx_buffer.size());
            if (rcp.is_connected())
            {
                rcp.send(m_channel_idx, m_tx_buffer.data(), m_tx_buffer.size());
            }
            m_tx_buffer.clear();
        }
    }
    void try_sending(RCP& rcp)
    {
        if (!m_tx_buffer.empty())
        {
            //q::quick_logf("Sending {} bytes", m_tx_buffer.size());
            if (rcp.is_connected())
            {
                rcp.try_sending(m_channel_idx, m_tx_buffer.data(), m_tx_buffer.size());
            }
            m_tx_buffer.clear();
        }
    }

    auto get_tx_buffer() -> TX_Buffer_t const&
    {
        return m_tx_buffer;
    }
    void clear_tx_buffer()
    {
        m_tx_buffer.clear();
    }

    //////////////////////////////////////////////////////////////////////////

    //returns the nest message or nothing.
    //the message, if any, has to be decoded with decode_next_message(...)
    auto get_next_message(RCP& rcp) -> bool  { return _get_next_message(rcp); }

    //////////////////////////////////////////////////////////////////////////

    auto begin_unpack() -> bool
    {
        QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size() && m_decoded.offset == HEADER_SIZE, "{}, {}", m_decoded.data_size, m_rx_buffer.size());
        //q::quick_logf("begin_decode: {}, {}", m_decoded.data_size, m_rx_buffer.size());
        return (m_decoded.data_size > HEADER_SIZE && m_decoded.offset == HEADER_SIZE);
    }
    template<class Param> auto unpack_param(Param& p) -> bool
    {
        //            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
        //            //q::quick_logf("unpack_param: {}, {}", m_decoded.data_size, m_rx_buffer.size());
        //            constexpr auto sz = sizeof(Param);
        //            if (m_decoded.offset + sz > m_decoded.data_size || m_decoded.offset + sz > m_rx_buffer.size())
        //            {
        //                return false;
        //            }
        return serialization::deserialize(m_rx_buffer, p, m_decoded.offset);
    }
    inline auto unpack_data(uint8_t* dst, size_t size) -> bool
    {
        QASSERT(dst);
        if (size == 0)
        {
            return true;
        }
        QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
        if (m_decoded.offset + size > m_decoded.data_size || m_decoded.offset + size > m_rx_buffer.size())
        {
            return false;
        }
        std::copy(m_rx_buffer.begin() + m_decoded.offset, m_rx_buffer.begin() + m_decoded.offset + size, dst);
        m_decoded.offset += size;
        return true;
    }
    inline auto unpack_remaining_data(std::vector<uint8_t>& dst) -> bool
    {
        QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
        if (m_decoded.offset > m_decoded.data_size || m_decoded.offset > m_rx_buffer.size())
        {
            return false;
        }
        dst.reserve(dst.size() + (m_decoded.data_size - m_decoded.offset));
        std::copy(m_rx_buffer.begin() + m_decoded.offset, m_rx_buffer.begin() + m_decoded.data_size, std::back_inserter(dst));
        m_decoded.offset = m_decoded.data_size;
        return true;
    }
    void end_unpack()
    {
        QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size() && m_decoded.offset == m_decoded.data_size, "{}, {}", m_decoded.data_size, m_rx_buffer.size());
        if (m_decoded.data_size > 0)
        {
            pop_front(m_decoded.data_size);
            m_decoded.data_size = 0;
        }
    }

    //decodes the next message
    template<typename... Params>
    auto unpack_all(Params&... params) -> bool
    {
        auto res = begin_unpack() && _unpack(params...);
        end_unpack();
        return res;
    }


    auto get_remaining_message_size() const -> size_t { return m_decoded.data_size - m_decoded.offset; }
    auto get_message_size() const -> size_t { return m_decoded.data_size - HEADER_SIZE; }

    //////////////////////////////////////////////////////////////////////////

private:
    struct Decoded
    {
        bool is_valid = false;
        Message_Size_t data_size = 0;
        size_t offset = 0;
    } m_decoded;

    void pop_front(size_t size)
    {
        QASSERT(size <= m_rx_buffer.size());
        m_rx_buffer.erase(m_rx_buffer.begin(), m_rx_buffer.begin() + size);
    }

    //returns the nest message or nothing.
    //the message, if any, has to be decoded with decode_next_message(...)
    auto _get_next_message(RCP& rcp) -> bool
    {
        if (m_decoded.data_size > 0)
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            pop_front(m_decoded.data_size);
            m_decoded.data_size = 0;
            m_decoded.is_valid = false;
        }

        rcp.receive(m_channel_idx, m_temp_rx_buffer);
        if (!m_temp_rx_buffer.empty())
        {
            //q::quick_logf("Received {} bytes", m_temp_rx_buffer.size());
            auto off = m_rx_buffer.size();
            m_rx_buffer.resize(off + m_temp_rx_buffer.size());
            std::copy(m_temp_rx_buffer.begin(), m_temp_rx_buffer.end(), m_rx_buffer.begin() + off);
            m_temp_rx_buffer.clear();
        }

        return decode_message();
    }

    //decodes the next message
    auto _unpack() -> bool
    {
        return true;
    }

    //decodes the next message
    template<typename Param, typename... Params>
    auto _unpack(Param& p, Params&... params) -> bool
    {
        //            QASSERT(m_decoded.offset + sizeof(p) <= m_decoded.data_size && m_decoded.data_size <= m_rx_buffer.size());
        //            if (m_decoded.offset + sizeof(p) > m_decoded.data_size || m_decoded.data_size > m_rx_buffer.size())
        //            {
        //                return false;
        //            }
        if (!serialization::deserialize(m_rx_buffer, p, m_decoded.offset))
        {
            return false;
        }
        return _unpack(params...);
    }

    bool decode_message()
    {
        m_decoded.is_valid = false;
        m_decoded.data_size = 0;
        m_decoded.offset = 0;

        //check if we have enough data
        if (m_rx_buffer.size() < HEADER_SIZE)
        {
            return false;
        }

        //try to decode a message HEADER
        Message_Size_t size;
        size_t size_offset = SIZE_OFFSET;
        if (!serialization::deserialize(m_rx_buffer, size, size_offset))
        {
            return false;
        }
        if (m_rx_buffer.size() < HEADER_SIZE + size)
        {
            return false;
        }

        //pop_front(HEADER_SIZE);

        m_decoded.offset = HEADER_SIZE;
        m_decoded.data_size = m_decoded.offset + size;

        m_decoded.is_valid = true;

        QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
        return true;
    }

    //sends a message with confirmation
    void _pack()
    {
    }

    //sends a message with confirmation
    template<typename Param, typename... Params>
    void _pack(Param const& param, Params&&... params)
    {
        auto off = m_tx_buffer.size();
        serialization::serialize(m_tx_buffer, param, off);
        return _pack(std::forward<Params>(params)...);
    }

    //////////////////////////////////////////////////////////////////////////

    uint8_t m_channel_idx = 0;
    RX_Buffer_t m_rx_buffer;
    std::vector<uint8_t> m_temp_rx_buffer;
    TX_Buffer_t m_tx_buffer;
    size_t m_size_off = 0;
    size_t m_data_start_off = 0;
};

}
}

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <boost/optional.hpp>

namespace util
{
    namespace detail
    {
        typedef std::vector<uint8_t> TX_Buffer_t;
        typedef std::vector<uint8_t> RX_Buffer_t;

        template<class T> auto get_value_fixed(T& val, RX_Buffer_t const& t, size_t off) -> bool
        {
            static_assert(std::is_standard_layout<T>::value, "you need a specialized packer/unpacker for T");
            if (off + sizeof(T) > t.size())
            {
                return false;
            }
            std::copy(t.begin() + off, t.begin() + off + sizeof(T), reinterpret_cast<uint8_t*>(&val));
            return true;
        }
        template<> inline auto get_value_fixed(std::string& val, RX_Buffer_t const& t, size_t off) -> bool
        {
            uint32_t size;
            if (!get_value_fixed(size, t, off))
            {
                return false;
            }
            if (off + sizeof(uint32_t) + size > t.size())
            {
                return false;
            }
            off += sizeof(uint32_t);
            val.resize(size);
            std::copy(t.begin() + off, t.begin() + off + size, val.begin());
            return true;
        }
        template<class T> auto get_value(T& val, RX_Buffer_t const& t, size_t& off) -> bool
        {
            static_assert(std::is_standard_layout<T>::value, "you need a specialized packer/unpacker for T");
            if (!get_value_fixed(val, t, off))
            {
                return false;
            }
            off += sizeof(T);
            return true;
        }
        template<> inline auto get_value(std::string& val, RX_Buffer_t const& t, size_t& off) -> bool
        {
            if (!get_value_fixed(val, t, off))
            {
                return false;
            }
            off += sizeof(uint32_t) + val.size();
            return true;
        }
        template<class T> inline auto get_value(std::vector<T>& val, RX_Buffer_t const& t, size_t& off) -> bool
        {
            val.clear();
            uint32_t size = 0;
            if (!get_value(size, t, off))
            {
                return false;
            }
            //sanity check
            if (off + size > t.size())
            {
                return false;
            }
            val.resize(size);
            for (auto& i: val)
            {
                if (!get_value(i, t, off))
                {
                    return false;
                }
            }
            return true;
        }
        template<class T> void set_value_fixed(TX_Buffer_t& t, T const& val, size_t off)
        {
            static_assert(std::is_standard_layout<T>::value, "you need a specialized packer/unpacker for T");
            QASSERT_MSG(off + sizeof(T) <= t.size(), "off {}, sizet {}, t.size {}, t.capacity {}", off, sizeof(T), t.size(), t.capacity());
            auto const* src = reinterpret_cast<uint8_t const*>(&val);
            std::copy(src, src + sizeof(T), t.data() + off);
        }
        template<> inline void set_value_fixed(TX_Buffer_t& t, std::string const& val, size_t off)
        {
            set_value_fixed(t, static_cast<uint32_t>(val.size()), off);
            off += sizeof(uint32_t);
            QASSERT_MSG(off + val.size() <= t.size(), "off {}, sizet {}, t.size {}, t.capacity {}", off, val.size(), t.size(), t.capacity());
            std::copy(val.begin(), val.end(), t.data() + off);
        }
        template<class T> void set_value(TX_Buffer_t& t, T const& val, size_t& off)
        {
            static_assert(std::is_standard_layout<T>::value, "you need a specialized packer/unpacker for T");
            if (off + sizeof(T) > t.size())
            {
                t.resize(off + sizeof(T));
            }
            set_value_fixed(t, val, off);
            off += sizeof(T);
        }
        template<> inline void set_value(TX_Buffer_t& t, std::string const& val, size_t& off)
        {
            if (off + sizeof(uint32_t) + val.size() > t.size())
            {
                t.resize(off + sizeof(uint32_t) + val.size());
            }
            set_value_fixed(t, val, off);
            off += val.size();
        }
        template<class T> inline void set_value(TX_Buffer_t& t, std::vector<T> const& val, size_t& off)
        {
            set_value(t, static_cast<uint32_t>(val.size()), off);
            for (auto const& i: val)
            {
                set_value(t, i, off);
            }
        }
    }

    template<class MESSAGE_T, class MESSAGE_SIZE_T>
    class Channel : q::util::Noncopyable
	{
        static const size_t MESSAGE_OFFSET = 0;
        static const size_t SIZE_OFFSET = MESSAGE_OFFSET + sizeof(MESSAGE_T);
        static const size_t HEADER_SIZE = SIZE_OFFSET + sizeof(MESSAGE_SIZE_T);

	public:
		typedef MESSAGE_T Message_t;
        typedef MESSAGE_SIZE_T Message_Size_t;
        typedef Channel<MESSAGE_T, MESSAGE_SIZE_T> This_t;

        Channel(uint8_t channel_idx) : m_channel_idx(channel_idx) {}

		//////////////////////////////////////////////////////////////////////////

		//sends a message with confirmation
		template<typename... Params>
        void pack_all(Message_t message, Params&&... params)
        {
            begin_pack(message);
            _pack(std::forward<Params>(params)...);
            end_pack();
        }

        void begin_pack(Message_t message)
        {
            auto off = m_tx_buffer.size();
            detail::set_value(m_tx_buffer, message, off);
            m_size_off = off;
            detail::set_value(m_tx_buffer, Message_Size_t(0), off);
            m_data_start_off = off;
        }
        template<class Param> void pack_param_at(size_t off, Param const& p)
        {
            QASSERT(m_tx_buffer.size() >= m_data_start_off + off + sizeof(p));
            QASSERT(m_data_start_off > m_size_off);
            detail::set_value_fixed(m_tx_buffer, p, m_data_start_off + off);
        }
        template<class Param> void pack_param(Param const& p)
        {
            QASSERT(m_tx_buffer.size() >= m_data_start_off);
            QASSERT(m_data_start_off > m_size_off);
            auto off = m_tx_buffer.size();
            detail::set_value(m_tx_buffer, p, off);
        }
        void pack_param(rapidjson::Document const& json)
        {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            json.Accept(writer);
            std::string str(buffer.GetString(), buffer.GetSize());
            pack_param(str);
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
            detail::set_value_fixed(m_tx_buffer, Message_Size_t(data_size), m_size_off);

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

        void send(util::RCP& rcp)
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
        void try_sending(util::RCP& rcp)
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

        auto get_tx_buffer() -> detail::TX_Buffer_t const&
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
        auto get_next_message(util::RCP& rcp) -> boost::optional<Message_t>  { return _get_next_message(rcp); }
        auto get_next_message(Message_t& message, util::RCP& rcp) -> bool {  auto res = _get_next_message(rcp); message = res ? *res : message; return res.is_initialized(); }

		//decodes the next message
		template<typename... Params>
        auto unpack_all(Params&... params) -> bool
        {
            return _unpack(size_t(0), params...);
        }

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
            return detail::get_value(p, m_rx_buffer, m_decoded.offset);
        }
        auto unpack_param(rapidjson::Document& json) -> bool
        {
            std::string str;
            if (!unpack_param(str))
            {
                return false;
            }
            json.SetObject();
            if (!str.empty() && json.Parse(str.c_str()).HasParseError())
            {
                QLOGE("Failed to parse json: {}:{}", json.GetParseError(), json.GetErrorOffset());
                return false;
            }
            return true;
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

        auto get_remaining_message_size() const -> size_t { return m_decoded.data_size - m_decoded.offset; }
        auto get_message_size() const -> size_t { return m_decoded.data_size - HEADER_SIZE; }

		//////////////////////////////////////////////////////////////////////////

	private:
        struct Decoded
		{
            bool is_valid = false;
            Message_t message;
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
        auto _get_next_message(util::RCP& rcp) -> boost::optional<Message_t>
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

            return decode_message() ? boost::optional<Message_t>(m_decoded.message) : boost::none;
		}

		//decodes the next message
        auto _unpack(size_t /*off*/) -> bool
		{
            if (m_decoded.data_size > 0)
			{
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
                m_decoded.is_valid = false;
			}

            return true;
		}

		//decodes the next message
		template<typename Param, typename... Params>
        auto _unpack(size_t off, Param& p, Params&... params) -> bool
		{
//            QASSERT(m_decoded.offset + sizeof(p) <= m_decoded.data_size && m_decoded.data_size <= m_rx_buffer.size());
//            if (m_decoded.offset + sizeof(p) > m_decoded.data_size || m_decoded.data_size > m_rx_buffer.size())
//            {
//                return false;
//            }
            if (!detail::get_value(p, m_rx_buffer, m_decoded.offset))
            {
                return false;
            }
            return _unpack(off, params...);
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
            Message_t message;
            Message_Size_t size;
            if (!detail::get_value_fixed(message, m_rx_buffer, MESSAGE_OFFSET) ||
                !detail::get_value_fixed(size, m_rx_buffer, SIZE_OFFSET))
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
            m_decoded.message = message;

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
            detail::set_value(m_tx_buffer, param, off);
            return _pack(std::forward<Params>(params)...);
		}

        //////////////////////////////////////////////////////////////////////////

        uint8_t m_channel_idx = 0;
        detail::RX_Buffer_t m_rx_buffer;
        std::vector<uint8_t> m_temp_rx_buffer;
        detail::TX_Buffer_t m_tx_buffer;
        size_t m_size_off = 0;
        size_t m_data_start_off = 0;
    };

}

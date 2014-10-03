#pragma once

#include <boost/optional.hpp>

namespace util
{

	template<
		class MESSAGE_T,
		class MESSAGE_SIZE_T,
		class SOCKET_T>
    class Channel : q::util::Noncopyable
	{
	public:
		typedef MESSAGE_T Message_t;
		typedef MESSAGE_SIZE_T Message_Size_t;
		typedef SOCKET_T Socket_t;
        typedef Channel<MESSAGE_T, MESSAGE_SIZE_T, SOCKET_T> This_t;

        Channel(Socket_t& socket) : m_socket(socket) {}

        //starts receiving
        void start() { _start(); }

		//////////////////////////////////////////////////////////////////////////

		//sends a message with confirmation
		template<typename... Params>
        void send(Message_t message, Params&&... params) { _send(message, get_tx_buffer(), HEADER_SIZE, std::forward<Params>(params)...); }

        void cancel_send() {}

        template<typename... Params>
        void stream(Message_t message, Params&&... params) { _send(message, get_tx_buffer(), HEADER_SIZE, std::forward<Params>(params)...); }

        void begin_stream()
        {
            m_crt_tx_buffer = get_tx_buffer();
            m_crt_tx_buffer->resize(HEADER_SIZE);
        }
        template<class Param> void add_to_stream_at(size_t off, Param const& p)
        {
            QASSERT(m_crt_tx_buffer);
            set_value_fixed(*m_crt_tx_buffer, p, HEADER_SIZE + off);
        }
        template<class Param> void add_to_stream(Param const& p)
        {
            QASSERT(m_crt_tx_buffer);
            auto off = m_crt_tx_buffer->size();
            set_value(*m_crt_tx_buffer, p, off);
        }
        void end_stream(Message_t message)
        {
            QASSERT(m_crt_tx_buffer);
            _send(message, m_crt_tx_buffer, m_crt_tx_buffer->size());
        }

        //////////////////////////////////////////////////////////////////////////

		//returns the nest message or nothing.
		//the message, if any, has to be decoded with decode_next_message(...)
        auto get_next_message() -> boost::optional<Message_t>  { return _get_next_message(); }
        auto get_next_message(Message_t& message) -> bool {  auto res = _get_next_message(); message = res ? *res : message; return res.is_initialized(); }

		enum class Unpack_Result : uint8_t
		{
			OK = 0,				//all good
			FAILED,				//a generic error
		};

		//decodes the next message
		template<typename... Params>
        auto unpack(Params&... params) -> Unpack_Result { return _unpack(size_t(0), params...); }

        //////////////////////////////////////////////////////////////////////////

        Unpack_Result begin_unpack()
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            //q::quick_logf("begin_decode: {}, {}", m_decoded.data_size, m_rx_buffer.size());
            if (m_decoded.data_size == 0)
            {
                return Unpack_Result::FAILED;
            }
            return Unpack_Result::OK;
        }
        template<class Param> Unpack_Result unpack_param(Param& p)
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            //q::quick_logf("unpack_param: {}, {}", m_decoded.data_size, m_rx_buffer.size());
            constexpr auto sz = sizeof(Param);
            if (m_decoded.data_size < sz || m_rx_buffer.size() < sz)
            {
                m_error_count++;
                return Unpack_Result::FAILED;
            }
            p = get_value_fixed<Param>(m_rx_buffer, 0);
            m_decoded.data_size -= sz;
            pop_front(sz);
            return Unpack_Result::OK;
        }
        void end_unpack()
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            if (m_decoded.data_size > 0)
            {
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
            }
        }

        auto get_message_size() const -> Message_Size_t { return m_decoded.data_size; }
        size_t get_pending_data_size() const { return m_rx_buffer.size(); }
        size_t get_error_count() const { return m_error_count; }

		//////////////////////////////////////////////////////////////////////////

	private:
        typedef std::vector<uint8_t> TX_Buffer_t;
        typedef std::vector<uint8_t> RX_Buffer_t;

        typedef uint8_t Magic_t;
        typedef uint16_t Header_Crc_t;
        typedef uint16_t Data_Crc_t;

        static const uint8_t MAGIC = 0x3F;
        static const uint8_t MAGIC_MASK = 0xFF;

        static const size_t MAGIC_OFFSET = 0;
        static const size_t MESSAGE_OFFSET = MAGIC_OFFSET + sizeof(Magic_t);
        static const size_t SIZE_OFFSET = MESSAGE_OFFSET + sizeof(Message_t);
        static const size_t HEADER_CRC_OFFSET = SIZE_OFFSET + sizeof(Message_Size_t);
        static const size_t DATA_CRC_OFFSET = HEADER_CRC_OFFSET + sizeof(Header_Crc_t);
        static const size_t HEADER_SIZE = DATA_CRC_OFFSET + sizeof(Data_Crc_t);

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
            QASSERT(off + sizeof(T) <= t.size());
            T val = *reinterpret_cast<T const*>(t.data() + off);
            return val;
        }
        template<class T> T get_value(RX_Buffer_t const& t, size_t& off)
        {
            auto val = get_value_fixed<T>(t, off);
            off += sizeof(T);
            return val;
        }
        template<class Container, class T> void set_value_fixed(Container& t, T const& val, size_t off)
        {
            QASSERT_MSG(off + sizeof(T) <= t.size(), "off {}, sizet {}, t.size {}, t.capacity {}", off, sizeof(T), t.size(), t.capacity());
            *reinterpret_cast<T*>(t.data() + off) = val;
        }
        template<class Container, class T> void set_value(Container& t, T const& val, size_t& off)
        {
            if (off + sizeof(T) > t.size())
            {
                t.resize(off + sizeof(T));
            }
            set_value_fixed(t, val, off);
            off += sizeof(T);
        }
        void pop_front(size_t size)
        {
            QASSERT(size <= m_rx_buffer.size());
            m_rx_buffer.erase(m_rx_buffer.begin(), m_rx_buffer.begin() + size);
        }

        void _start()
        {
            m_socket.async_read_some(boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
                                    boost::bind(&This_t::handle_receive, this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
//            m_socket.async_receive(
//                boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
//                boost::bind(&Asio_Socket_Adapter<Socket_t>::handle_receive, this,
//				boost::asio::placeholders::error,
//				boost::asio::placeholders::bytes_transferred));
        }

		//returns the nest message or nothing.
		//the message, if any, has to be decoded with decode_next_message(...)
        auto _get_next_message() -> boost::optional<Message_t>
		{
            if (m_decoded.data_size > 0)
            {
                QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
                std::lock_guard<std::mutex> lg(m_rx_mutex);
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
                m_decoded.magic = 0;
            }

            //check for incoming messages
            while (decode_message()) {}
            if (m_decoded.magic == 0)
            {
                return boost::optional<Message_t>();
            }

            return boost::optional<Message_t>(m_decoded.message);
		}

		//decodes the next message
        Unpack_Result _unpack(size_t /*off*/)
		{
            if (m_decoded.data_size > 0)
			{
                std::lock_guard<std::mutex> lg(m_rx_mutex);
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
                m_decoded.magic = 0;
			}

			return Unpack_Result::OK;
		}

		//decodes the next message
		template<typename Param, typename... Params>
        Unpack_Result _unpack(size_t off, Param& p, Params&... params)
		{
            {
                std::lock_guard<std::mutex> lg(m_rx_mutex);
                QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
                if (m_rx_buffer.size() < m_decoded.data_size || m_rx_buffer.size() < off + sizeof(Param))
                {
                    m_error_count++;
                    return Unpack_Result::FAILED;
                }
                p = get_value<Param>(m_rx_buffer, off);
            }

            return _unpack(off, params...);
		}

		bool decode_message()
		{
            std::lock_guard<std::mutex> lg(m_rx_mutex);

            m_decoded.magic = 0;
            m_decoded.data_size = 0;

            //check if we have enough data
            if (m_rx_buffer.size() < HEADER_SIZE)
            {
                return false;
            }

            //try to decode a message HEADER
            auto magic = get_value_fixed<Magic_t>(m_rx_buffer, MAGIC_OFFSET);
            if ((magic & MAGIC_MASK) != MAGIC)
            {
                m_error_count++;
                q::quick_logf("malformed package {}: magic {}", magic, magic & MAGIC_MASK);
                pop_front(1);
                return true;
            }
            auto message = get_value_fixed<Message_t>(m_rx_buffer, MESSAGE_OFFSET);
            auto size = get_value_fixed<Message_Size_t>(m_rx_buffer, SIZE_OFFSET);
            auto header_crc = get_value_fixed<Header_Crc_t>(m_rx_buffer, HEADER_CRC_OFFSET);

            //verify header crc
            {
                Header_Crc_t computed_header_crc = q::util::compute_crc<Header_Crc_t>(m_rx_buffer, HEADER_CRC_OFFSET);
                if (header_crc != computed_header_crc)
                {
                    m_error_count++;
                    q::quick_logf("header crc failed {} / {} for msg {} size {}", header_crc, computed_header_crc, static_cast<int>(message), size);
                    pop_front(1);
                    return true;
                }
            }

            if (m_rx_buffer.size() < HEADER_SIZE + size)
            {
                return false;
            }

            auto data_crc = get_value_fixed<Data_Crc_t>(m_rx_buffer, DATA_CRC_OFFSET);

            //clear crc bytes and compute crc
            set_value_fixed(m_rx_buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
            auto computed_data_crc = q::util::compute_crc<Data_Crc_t>(m_rx_buffer, HEADER_SIZE + size);
            if (data_crc != computed_data_crc)
            {
                m_error_count++;
                q::quick_logf("data crc failed {} / {} for msg {} size {}", data_crc, computed_data_crc, static_cast<int>(message), size);
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

            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());

            return false;
		}

		//sends a message with confirmation
        void _send(Message_t message, std::shared_ptr<std::vector<uint8_t>> const& buffer, size_t total_size)
		{
            QASSERT(buffer);
            QASSERT(total_size <= buffer->size());
            QASSERT(total_size >= HEADER_SIZE);
            size_t data_size = total_size - HEADER_SIZE;
            //header
            Magic_t magic = MAGIC;
            set_value_fixed(*buffer, magic, MAGIC_OFFSET);
            set_value_fixed(*buffer, message, MESSAGE_OFFSET);
            set_value_fixed(*buffer, Message_Size_t(data_size), SIZE_OFFSET);
            set_value_fixed(*buffer, Header_Crc_t(0), HEADER_CRC_OFFSET);

            //header crc
            auto header_crc = q::util::compute_crc<Header_Crc_t>(*buffer, HEADER_CRC_OFFSET);
            set_value_fixed(*buffer, header_crc, HEADER_CRC_OFFSET);

            //data crc
            set_value_fixed(*buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
            auto data_crc = q::util::compute_crc<Data_Crc_t>(*buffer, total_size);
            set_value_fixed(*buffer, data_crc, DATA_CRC_OFFSET);

            //q::quick_logf("sending msg {}, size {}, hcrc {}, dcrc {}", static_cast<int>(message), data_size, header_crc, data_crc);

            //send
            send_to_socket(buffer, total_size);
		}

		//sends a message with confirmation
		template<typename Param, typename... Params>
        void _send(Message_t message, std::shared_ptr<std::vector<uint8_t>> const& buffer, size_t off, Param const& param, Params&&... params)
		{
            set_value(*buffer, param, off);
            return _send(message, buffer, off, std::forward<Params>(params)...);
		}

        void handle_send(std::shared_ptr<TX_Buffer_t> buffer_ptr,
                         const boost::system::error_code& /*error*/,
                         std::size_t /*bytes_transferred*/)
        {
            std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
            m_tx_buffer_pool.push_back(buffer_ptr);
        }

        void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
        {
            if (error)
            {
                m_socket.close();
            }
            else
            {
                if (bytes_transferred > 0)
                {
                    std::lock_guard<std::mutex> lg(m_rx_mutex);
                    auto off = m_rx_buffer.size();
                    m_rx_buffer.resize(off + bytes_transferred);
                    std::copy(m_rx_temp_buffer.begin(), m_rx_temp_buffer.begin() + bytes_transferred, m_rx_buffer.begin() + off);
                }
                start();
            }
        }

        void send_to_socket(std::shared_ptr<std::vector<uint8_t>> buffer, size_t size)
        {
            if (!buffer->empty())
            {
                m_socket.async_write_some(boost::asio::buffer(*buffer, size),
                    boost::bind(&This_t::handle_send, this,
                    buffer,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }
        }

        //////////////////////////////////////////////////////////////////////////

        std::shared_ptr<TX_Buffer_t> get_tx_buffer()
        {
            std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
            std::shared_ptr<TX_Buffer_t> buffer_ptr;
            if (!m_tx_buffer_pool.empty())
            {
                buffer_ptr = m_tx_buffer_pool.back();
                m_tx_buffer_pool.pop_back();
            }
            else
            {
                buffer_ptr = std::make_shared<TX_Buffer_t>();
            }
            buffer_ptr->reserve(32);
            if (buffer_ptr->size() < HEADER_SIZE)
            {
                buffer_ptr->resize(HEADER_SIZE);
            }
            return buffer_ptr;
        }

		Socket_t& m_socket;
        RX_Buffer_t m_rx_buffer;
        std::array<uint8_t, 512> m_rx_temp_buffer;
        std::mutex m_rx_mutex;

        std::vector<std::shared_ptr<TX_Buffer_t>> m_tx_buffer_pool;
        std::mutex m_tx_buffer_pool_mutex;
        std::shared_ptr<TX_Buffer_t> m_crt_tx_buffer;

        size_t m_error_count = 0;
	};

}
